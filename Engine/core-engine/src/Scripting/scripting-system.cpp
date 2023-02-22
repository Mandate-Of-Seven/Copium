/*!***************************************************************************************
****
\file			scripting-system.cpp
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file contains the function definitions for the scripting system.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#include "pch.h"
#include "Scripting/scripting-system.h"
#include "Scripting/compiler.h"
#include "Utilities/thread-system.h"
#include "Files/file-system.h"
#include "CopiumCore/system-interface.h"
#include "Messaging/message-system.h"
#include "Messaging/message-types.h"
#include "Scripting/script-wrappers.h"
#include <SceneManager/scene-manager.h>
#include <Events/events-system.h>
#include <GameObject/components.h>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>

#define SECONDS_TO_RECOMPILE 5
namespace
{
	Copium::MessageSystem* messageSystem{ Copium::MessageSystem::Instance() };
	MonoDomain* mRootDomain{ nullptr };		//JIT RUNTIME DOMAIN
	MonoDomain* mAppDomain{ nullptr };		//APP DOMAIN
	MonoAssembly* mCoreAssembly{ nullptr };	//ASSEMBLY OF SCRIPTS.DLL
	MonoImage* mAssemblyImage{ nullptr };	//LOADED IMAGE OF SCRIPTS.DLL
	MonoClass* mGameObject{ nullptr };
	MonoClass* mCopiumScript{ nullptr };
	MonoClass* mCollision2D{ nullptr };
	MonoClass* mScriptableObject{};
}

namespace Copium::Utils
{
	static char* readBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream) return nullptr;
		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = (uint32_t)(end - stream.tellg());
		if (size == 0) return nullptr;
		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();
		*outSize = size;
		return buffer;
	}

	static MonoAssembly* loadAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = readBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			// const char* errorMessage = mono_image_strerror(status);
			// Log some error message using the errorMessage data
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);

		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	FieldType monoTypeToFieldType(MonoType* monoType)
	{
		std::string typeName = mono_type_get_name_full(monoType, MONO_TYPE_NAME_FORMAT_FULL_NAME);
		auto it = fieldTypeMap.find(typeName);
		if (it == fieldTypeMap.end())
		{
			if (typeName.find_first_of("CopiumEngine.") == 0)
			{
				std::string componentName = typeName.substr(typeName.find_last_of('.') + 1);
				auto it{ NAME_TO_CTYPE.find(componentName) };
				if (it == NAME_TO_CTYPE.end())
				{
					//PRINT(componentName << " is a script");
					return (FieldType)ComponentType::Script;
				}
				//PRINT(componentName << " is a component");
				return (FieldType)it->second;
			}
			typeName = mono_class_get_name(mono_class_get_parent(mono_class_from_mono_type(monoType)));
			if (typeName == "CopiumScript")
			{
				return (FieldType)ComponentType::Script;
			}
			return FieldType::None;
		}
		return it->second;
	}
}

namespace Copium
{
	ScriptingSystem& sS{ *ScriptingSystem::Instance() };
	bool scriptIsLoaded(const std::filesystem::path&);
	bool scriptPathExists(const std::filesystem::path& filePath);
	//ScriptingEngine Namespace Functions

#pragma region Struct ScriptMethods
	ScriptClass::ScriptClass(const std::string& _name, MonoClass* _mClass) :
		mClass{ _mClass }
	{
		void* methodIterator = nullptr;
		while (MonoMethod* method = mono_class_get_methods(_mClass, &methodIterator))
		{
			mMethods[mono_method_get_name(method)] = method;
		}

		void* iterator = nullptr;
		while (MonoClassField* field = mono_class_get_fields(mClass, &iterator))
		{
			std::string fieldName = mono_field_get_name(field);
			uint32_t flags = mono_field_get_flags(field);
			if (flags & FIELD_ATTRIBUTE_PUBLIC)
			{
				MonoType* type = mono_field_get_type(field);
				FieldType fieldType = Utils::monoTypeToFieldType(type);
				//PRINT(mono_type_get_name(type));
				if (fieldType != FieldType::None)
				{
					mFields[fieldName] = field;
				}
				else
				{
					static std::string typeName;
					typeName = mono_type_get_name(type);
					fieldType = FieldType::Component;
					//C# List
					//if (typeName.find_first_of("System.Collections.Generic.List<") == 0)
					//{
					//	typeName = typeName.substr(32);
					//	typeName.pop_back();
					//}

					auto it = fieldTypeMap.find(typeName);
					//Type that is in the fieldTypeMap
					if (it != fieldTypeMap.end())
					{
						fieldType = (*it).second;
					}

					PRINT("COMPONENT TYPE: " << fieldName);
					mFields[fieldName] = field;
				}
			}
		}
	}

#pragma endregion

	bool ScriptingSystem::isScriptableObject(const std::string& name)
	{
		return scriptableObjectClassMap[name].mClass;
	}

	bool ScriptingSystem::isScript(const std::string& name)
	{

		return scriptClassMap[name].mClass;
	}


	const std::unordered_map<std::string, ScriptClass>& ScriptingSystem::getScriptableObjectClassMap()
	{
		return scriptableObjectClassMap;
	}

	void ScriptingSystem::addEmptyScript(const std::string& _name)
	{
		std::ofstream file(Paths::assetPath + "\\Scripts\\" + _name + ".cs");
		file << "using CopiumEngine;\n";
		file << "using System;\n\n";
		file << "public class " << _name << ": CopiumScript\n{\n";
		file << "\tvoid Start()\n\t{\n\n\t}\n";
		file << "\tvoid Update()\n\t{\n\n\t}\n";
		file << "}\n";
		file.close();
	}

	MonoType* ScriptingSystem::getMonoTypeFromName(std::string& name)
	{
		return mono_reflection_type_from_name(name.data(), mAssemblyImage);
	}

	void ScriptingSystem::recompileThreadWork()
	{
		ThreadSystem& tSys = *ThreadSystem::Instance();
		while (!tSys.Quit())
		{
			while (compilingState != CompilingState::Wait);
			compilingState = CompilingState::Compiling;
			//Critical section
			while (!tSys.acquireMutex(MutexType::FileSystem));
			updateScriptFiles();
			tryRecompileDll();
			tSys.returnMutex(MutexType::FileSystem);
			//Critical section End
			Sleep(SECONDS_TO_RECOMPILE * 1000);
		}
	}

	ScriptingSystem::ScriptingSystem() :
		scriptFiles{ FileSystem::Instance()->get_files_with_extension(".cs") }
	{

	}

	const std::list<Copium::File>& ScriptingSystem::getScriptFiles()
	{
		return scriptFiles;
	}


	template<typename T, typename... Ts>
	void ScriptingSystem::SubscribeComponentBasedCallbacks(TemplatePack<T, Ts...> pack)
	{
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptSetFieldReference<T>);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackReflectComponent<T>);
		if constexpr (sizeof...(Ts) != 0)
		{
			SubscribeComponentBasedCallbacks(TemplatePack<Ts...>());
		}
	}

	void ScriptingSystem::init()
	{
		initMono();
		registerScriptWrappers();
		systemFlags |= FLAG_RUN_ON_EDITOR;
		ThreadSystem::Instance()->addThread(new std::thread(&ScriptingSystem::recompileThreadWork, this));
		messageSystem.subscribe(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT, this);
		MyEventSystem->subscribe(this,&ScriptingSystem::CallbackSceneChanging);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptInvokeMethod);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptGetMethodNames);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptSetField);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptGetField);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptGetNames);
		SubscribeComponentBasedCallbacks(ComponentTypes());
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptSetFieldReference<GameObject>);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackStartPreview);
		messageSystem.subscribe(MESSAGE_TYPE::MT_SCENE_DESERIALIZED, this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_ENGINE_INITIALIZED, this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_START_PREVIEW, this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_COLLISION_ENTER, this);
	}

	void ScriptingSystem::update()
	{
		if (compilingState == CompilingState::SwapAssembly)
		{
			swapDll();
			compilingState = CompilingState::Wait;
		}
	}

	void ScriptingSystem::exit()
	{
		shutdownMono();
	}

	MonoObject* ScriptingSystem::instantiateClass(MonoClass* mClass)
	{
		if (mAppDomain == nullptr || mClass == nullptr)
		{
			COPIUM_ASSERT(mAppDomain == nullptr, "MONO APP DOMAIN NOT LOADED");
			COPIUM_ASSERT(mClass == nullptr, "MONO CLASS NOT LOADED");
			return nullptr;
		}
			
		MonoObject* tmp = mono_object_new(mAppDomain, mClass);
		mono_runtime_object_init(tmp);
		return tmp;
	}

	const std::unordered_map<std::string, ScriptClass>& ScriptingSystem::getScriptClassMap()
	{
		return scriptClassMap;
	}

	void ScriptingSystem::updateScriptClasses()
	{
		using nameToClassIt = std::unordered_map<std::string, ScriptClass>::iterator;
		nameToClassIt it = scriptClassMap.begin();
		//static std::vector<nameToClassIt> keyMask;
		scriptClassMap.clear();

		const MonoTableInfo* table_info = mono_image_get_table_info(mAssemblyImage, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(table_info);

		/* For each row, get some of its values */
		for (int i = 0; i < rows; i++)
		{
			MonoClass* _class = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(mAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			const char* name_space = mono_metadata_string_heap(mAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			_class = mono_class_from_name(mAssemblyImage, name_space, name);
			if (mono_class_get_parent(_class) == mCopiumScript)
			{
				scriptClassMap[name] = ScriptClass{ name,_class };
				reflectionMap[mono_class_get_type(_class)] = ComponentType::Script;
			}
			else if(mono_class_get_parent(_class) == mScriptableObject)
			{
				scriptableObjectClassMap[name] = ScriptClass{ name,_class };
			}
			else if (mono_class_get_parent(_class) == mono_class_from_name(mAssemblyImage, name_space, "Component"))
			{
				if (_class == mCopiumScript)
					continue;
				scriptClassMap[name] = ScriptClass{ name,_class };
				reflectionMap[mono_class_get_type(_class)] = NAME_TO_CTYPE[name];
			}
		}
	}

	void ScriptingSystem::initMono()
	{
		mono_set_assemblies_path("mono");
		mRootDomain = mono_jit_init("CopiumJITRuntime");
	}

	void ScriptingSystem::shutdownMono()
	{
		mono_jit_cleanup(mRootDomain);
		mRootDomain = nullptr;
	}

	void ScriptingSystem::createAppDomain()
	{
		static char appName[] = "CopiumAppDomain";
		mAppDomain = mono_domain_create_appdomain(appName, nullptr);
		mono_domain_set(mAppDomain, false);
	}

	void ScriptingSystem::unloadAppDomain()
	{
		if (mAppDomain)
		{
			mono_domain_set(mRootDomain, false);
			mono_domain_unload(mAppDomain);
			mAppDomain = nullptr;
		}
	}

	template <typename T, typename... Ts>
	struct ReflectExistingStruct
	{
		ReflectExistingStruct(TemplatePack<T,Ts...> pack) {};
		ReflectExistingStruct() 
		{
			Scene* pScene{ MySceneManager.get_current_scene() };
			if (pScene)
			{
				for (GameObject& gameObject : pScene->gameObjects)
				{
					MyScriptingSystem.ReflectGameObject(gameObject);
					Reflect<T, Ts...>(gameObject);
				}
			}
		}
		template <typename T1, typename... T1s>
		void Reflect(GameObject& gameObj)
		{
			for (T1* pComponent : gameObj.GetComponents<T1>())
			{
				MyScriptingSystem.ReflectComponent(*pComponent);
			}
			if constexpr (sizeof...(T1s) != 0)
				Reflect<T1s...>(gameObj);
		}
	};

	using ReflectAll = decltype(ReflectExistingStruct(ComponentTypes()));

	void ScriptingSystem::swapDll()
	{
		PRINT("SWAPPING DLL_____________________________________");
		registerScriptWrappers();
		unloadAppDomain();
		createAppDomain();
		mGameObjects.clear();
		mComponents.clear();
		mCoreAssembly = Utils::loadAssembly(Paths::scriptsAssemblyPath);
		mAssemblyImage = mono_assembly_get_image(mCoreAssembly);
		mGameObject = mono_class_from_name(mAssemblyImage, "CopiumEngine", "GameObject");
		mCopiumScript = mono_class_from_name(mAssemblyImage, "CopiumEngine", "CopiumScript");
		mCollision2D = mono_class_from_name(mAssemblyImage, "CopiumEngine", "Collision2D");
		mScriptableObject = mono_class_from_name(mAssemblyImage, "CopiumEngine", "ScriptableObject");
		klassScene = mono_class_from_name(mAssemblyImage, "CopiumEngine", "Scene");
		updateScriptClasses();

		/* For each row, get some of its values */
		mCurrentScene = instantiateClass(klassScene);

		ReflectAll();

		
		COPIUM_ASSERT(!mGameObject, "GameObject C# script could not be loaded");
		COPIUM_ASSERT(!mCopiumScript, "CopiumScript C# script could not be loaded");
		COPIUM_ASSERT(!mCollision2D, "Collision2D C# script could not be loaded");
		COPIUM_ASSERT(!mScriptableObject, "ScriptableObject C# script could not be loaded");
		COPIUM_ASSERT(!mScriptableObject, "Scene C# script could not be loaded");
		messageSystem.dispatch(MESSAGE_TYPE::MT_CREATE_CS_GAMEOBJECT);
		messageSystem.dispatch(MESSAGE_TYPE::MT_SCRIPTING_UPDATED);
		PRINT("END SWAP DLL_____________________________________");
	}

	MonoObject* ScriptingSystem::invoke(MonoObject* mObj, MonoMethod* mMethod, void** params)
	{
		if (mObj && mMethod && mAppDomain)
		{
			return mono_runtime_invoke(mMethod, mObj, params, nullptr);
		}
		return nullptr;
	}

	MonoObject* ScriptingSystem::getFieldMonoObject(MonoClassField* mField, MonoObject* mObject)
	{
		if (mAppDomain == nullptr)
		{
			PRINT("APP DOMAIN WAS NULL");
			return nullptr;
		}
		return mono_field_get_value_object(mAppDomain, mField, mObject);
	}

	void ScriptingSystem::updateScriptFiles()
	{
		//Check for new files
		namespace fs = std::filesystem;
		using scriptFileListIter = std::list<File>::iterator;
		scriptFileListIter scriptFilesIt = scriptFiles.begin();
		static std::list<File*> maskScriptFiles;
		maskScriptFiles.resize(scriptFiles.size());
		for (File& file : scriptFiles)
		{
			maskScriptFiles.push_back(&file);
		}
		for (const fs::directory_entry& p : fs::recursive_directory_iterator(Paths::projectPath))
		{
			const fs::path& pathRef{ p.path() };
			if (pathRef.extension() != ".cs")
				continue;

			//Detect new scripts
			if (!scriptIsLoaded(pathRef))
			{
				scriptFiles.emplace(scriptFilesIt, File(pathRef));
			}
			//Script was already loaded
			else
			{
				//Set scripts to be masked
				for (File* scriptFile : maskScriptFiles)
				{
					if (scriptFile && scriptFile->filePath == pathRef)
					{
						maskScriptFiles.remove(scriptFile);
						break;
					}
				}
				++scriptFilesIt;
			}
		}
		// Remove deleted scripts using mask
		//for (File* scriptFile : maskScriptFiles)
		//{
		//	if (scriptFile != nullptr)
		//	{
		//		scriptFiles.remove(*scriptFile);
		//	}
		//}
		//maskScriptFiles.clear();
	}

	MonoObject* ScriptingSystem::cloneInstance(MonoObject* _instance)
	{
		if (!_instance)
			return nullptr;
		return mono_object_clone(_instance);
	}

	MonoObject* ScriptingSystem::createInstance(MonoClass* _mClass)
	{
		return mono_object_new(mAppDomain,_mClass);
	}

	void ScriptingSystem::tryRecompileDll()
	{
		bool startCompiling = false;
		for (File& scriptFile : scriptFiles)
		{
			scriptFile.update_modification_timing();
			if (scriptFile.is_modified() && !startCompiling)
			{
				startCompiling = true;
				Utils::compileDll();
				compilingState = CompilingState::SwapAssembly;
			}
		}
		if (!startCompiling)
			compilingState = CompilingState::Wait;
	}

	bool ScriptingSystem::scriptIsLoaded(const std::filesystem::path& filePath)
	{
		using scriptFileListIter = std::list<File>::iterator;
		for (scriptFileListIter it = scriptFiles.begin(); it != scriptFiles.end(); ++it)
		{
			if (it->filePath == filePath) return true;
		}
		return false;
	}

	MonoString* ScriptingSystem::createMonoString(const char* str)
	{
		if (!mAppDomain)
		{
			PRINT("APP DOMAIN NOT LOADED");
		}
		return mono_string_new(mAppDomain, str);
	}

	void ScriptingSystem::handleMessage(MESSAGE_TYPE mType)
	{
		switch (mType)
		{
			case MESSAGE_TYPE::MT_SCENE_DESERIALIZED:
			{
				compilingState = CompilingState::Wait;
				break;
			}
			case MESSAGE_TYPE::MT_ENGINE_INITIALIZED:
			{
				if (!mAssemblyImage)
					return;
				break;
			}
			case MESSAGE_TYPE::MT_START_PREVIEW:
			{
				break;
			}
			case MESSAGE_TYPE::MT_STOP_PREVIEW:
			{
				MonoGameObjects& gameObjects{mGameObjects[mCurrentScene]};
				MonoComponents& components{ mComponents[mCurrentScene] };

				for (auto& pair : gameObjects)
				{
					pair.second = nullptr;
				}

				for (auto& pair : components)
				{
					pair.second = nullptr;
				}

				mGameObjects.erase(mCurrentScene);
				mComponents.erase(mCurrentScene);

				mCurrentScene = mPreviousScene;
				mPreviousScene = nullptr;
				compilingState = CompilingState::Wait;
				break;
			}
			case MESSAGE_TYPE::MT_COLLISION_ENTER:
			{
				break;
			}
		}
	}

	void ScriptingSystem::GetFieldValue(MonoObject* instance, MonoClassField* mClassFiend ,Field& field, void* container)
	{
		if (field.fType == FieldType::String)
		{
			MonoString* mono_string = sS.createMonoString("");
			mono_field_get_value(instance, mClassFiend, &mono_string);
			char* str = mono_string_to_utf8(mono_string);
			strcpy((char*)container, str);
			return;
		}
		mono_field_get_value(instance, mClassFiend, container);
		return;
	}

	void ScriptingSystem::SetFieldValue(MonoObject* instance, MonoClassField* mClassFiend, Field& field, const void* value)
	{
		//THIS FUNCTION ONLY WORKS FOR BASIC TYPES
		field = value;
		//If its a string, its a C# string so create one
		if (field.fType == FieldType::String)
		{
			MonoString* mono_string = sS.createMonoString(reinterpret_cast<const char*>(value));
			mono_field_set_value(instance, mClassFiend, mono_string);
			return;
		}
		mono_field_set_value(instance, mClassFiend, (void*)value);
		return;
	}

	template<typename T>
	void ScriptingSystem::SetFieldReference(MonoObject* instance, MonoClassField* mClassFiend, Field& field, T* reference)
	{
		//When you set a reference, you need to create a MonoObject of it first

		//ZACH: If setting to nullptr, no point checking
		if (reference == nullptr)
		{
			mono_field_set_value(instance, mClassFiend, nullptr);
			return;
		}
		//ZACH: Trying to set a component reference
		if constexpr (ComponentTypes::has<T>())
		{
			mono_field_set_value(instance, mClassFiend, ReflectComponent(*reference));
		}
		//ZACH: Trying to set a gameobject reference
		else if (std::is_same<T,GameObject>())
		{
			mono_field_set_value(instance, mClassFiend, ReflectGameObject(*reference));
		}
		else
		{
			static_assert(true);
		}
	}


	MonoObject* ScriptingSystem::ReflectGameObject(GameObject& gameObj)
	{
		auto pairIt = mGameObjects[mCurrentScene].find(gameObj.uuid);
		if (pairIt == mGameObjects[mCurrentScene].end())
		{
			void* param = &gameObj.uuid;
			MonoMethod* reflectGameObject = mono_class_get_method_from_name(klassScene, "ReflectGameObject", 1);
			MonoObject* mGameObj= invoke(mCurrentScene, reflectGameObject, &param);
			mGameObjects[mCurrentScene].emplace(gameObj.uuid, mGameObj);
			return mGameObj;
		}
		return (*pairIt).second;
	}

	template <typename T>
	MonoObject* ScriptingSystem::ReflectComponent(T& component)
	{
		auto pairIt = mComponents[mCurrentScene].find(component.uuid);
		if (pairIt == mComponents[mCurrentScene].end())
		{
			PRINT(GetComponentType<T>::name);
			ScriptClass& scriptClass = scriptClassMap[GetComponentType<T>::name];
			UUID cid{ component.uuid };
			UUID gid{ component.gameObj.uuid };
			MonoObject* mComponent = instantiateClass(scriptClass.mClass);
			void* params[3] = { mComponent ,&cid,&gid };
			MonoMethod* reflectComponent = mono_class_get_method_from_name(klassScene, "ReflectComponent", 3);
			COPIUM_ASSERT(!mComponent, "MonoObject was null");
			invoke(mCurrentScene, reflectComponent, params);
			mComponents[mCurrentScene].emplace(cid, mComponent);
			//Check fields, dont remove fields, but change them if their type is different
			return mComponent;
		}
		return (*pairIt).second;
	}

	template <>
	MonoObject* ScriptingSystem::ReflectComponent(Script& component)
	{
		auto pairIt = mComponents[mCurrentScene].find(component.uuid);
		if (pairIt == mComponents[mCurrentScene].end())
		{
			PRINT(component.Name());
			ScriptClass& scriptClass = scriptClassMap[component.Name()];
			UUID cid{ component.uuid };
			UUID gid{ component.gameObj.uuid };
			MonoObject* mComponent = instantiateClass(scriptClass.mClass);
			void* params[3] = { mComponent ,&cid,&gid };
			MonoMethod* reflectComponent = mono_class_get_method_from_name(klassScene, "ReflectComponent", 3);
			COPIUM_ASSERT(!mComponent, "MonoObject was null");
			invoke(mCurrentScene, reflectComponent, params);
			mComponents[mCurrentScene].emplace(cid, mComponent);
			//Check fields, dont remove fields, but change them if their type is different

			//PRINT("Creating: " << component.Name() << " of id: " << component.uuid);
			Script& script{ *reinterpret_cast<Script*>(&component) };
			for (auto& pair : scriptClass.mFields)
			{
				MonoClassField* mField = pair.second;
				MonoType* type = mono_field_get_type(mField);
				FieldType fieldType = Utils::monoTypeToFieldType(type);
				const char* fieldName = pair.first.c_str();
				std::string typeName = mono_type_get_name(type);
				PRINT(typeName << " " << (int)fieldType);
				auto nameField{ script.fieldDataReferences.find(fieldName) };
				int alignment{};
				int fieldSize = mono_type_size(type, &alignment);
				//POINTER
				if (fieldType == FieldType::Component || fieldType == FieldType::GameObject)
				{
					fieldSize = sizeof(uint64_t);
				}
				//Field has not been created onto script yet
				if (nameField == script.fieldDataReferences.end())
				{
					Field newField = Field(fieldType, fieldSize, nullptr);
					size_t offset = typeName.find_last_of(".");
					if (offset != std::string::npos)
						newField.typeName = typeName.substr(offset + 1);
					else
						newField.typeName = typeName;
					if (fieldType == FieldType::Component || fieldType == FieldType::GameObject)
					{
						newField = std::numeric_limits<uint64_t>::max();
					}
					else
					{
						mono_field_get_value(mComponent, mField, newField.data);
					}
					script.fieldDataReferences[fieldName] = std::move(newField);
				}
				//Field exists
				else
				{
					Field& field = nameField->second;
					//If the field type is not the same
					if (field.fType != fieldType)
					{
						field.Resize(fieldSize);
						mono_field_get_value(mComponent, mField, field.data);
					}
					//Field exists, setback the values first
					else
					{
						if (field.fType == FieldType::GameObject)
						{
							GameObject* reference = script.fieldGameObjReferences[fieldName];
							if (reference)
							{
								MonoObject* monoReference{};
								monoReference = ReflectGameObject(*reference);
								mono_field_set_value(mComponent, mField, monoReference);
							}
						}
						else if (field.fType == FieldType::Component)
						{
							Component* reference = script.fieldComponentReferences[fieldName];
							if (reference)
							{
								MonoObject* monoReference{};
								switch ((ComponentType)field.fType)
								{
								case(ComponentType::Animator):
								{
									monoReference = ReflectComponent(*(Animator*)reference);
									break;
								}
								case(ComponentType::AudioSource):
								{
									monoReference = ReflectComponent(*(AudioSource*)reference);
									break;
								}
								case(ComponentType::BoxCollider2D):
								{
									monoReference = ReflectComponent(*(BoxCollider2D*)reference);
									break;
								}
								case(ComponentType::Button):
								{
									monoReference = ReflectComponent(*(Button*)reference);
									break;
								}
								case(ComponentType::Camera):
								{
									monoReference = ReflectComponent(*(Camera*)reference);
									break;
								}
								case(ComponentType::Image):
								{
									monoReference = ReflectComponent(*(Image*)reference);
									break;
								}
								case(ComponentType::Rigidbody2D):
								{
									monoReference = ReflectComponent(*(Rigidbody2D*)reference);
									break;
								}
								case(ComponentType::SpriteRenderer):
								{
									monoReference = ReflectComponent(*(SpriteRenderer*)reference);
									break;
								}
								case(ComponentType::Script):
								{
									monoReference = ReflectComponent(*(Script*)reference);
									break;
								}
								case(ComponentType::Text):
								{
									monoReference = ReflectComponent(*(Text*)reference);
									break;
								}
								case(ComponentType::SortingGroup):
								{
									monoReference = ReflectComponent(*(SortingGroup*)reference);
									break;
								}
								}
								mono_field_set_value(mComponent, mField, monoReference);
							}
						}
						else
						{
							SetFieldValue(mComponent, mField, field,field.data);
						}
					}
					//CHECK TYPENAME
					//else if (field.typeName)
				}
			}
			return mComponent;
		}
		return (*pairIt).second;
	}

	void ScriptingSystem::CallbackSceneChanging(SceneChangingEvent* pEvent)
	{
		//If there is no assembly loaded at all
		if (mAssemblyImage == nullptr)
		{
			//Wait if it is still compiling
			while (compilingState == CompilingState::Compiling) {};
			//If it finished compiling and needs to swap
			if (compilingState == CompilingState::SwapAssembly)
			{
				//Swap dll and set back to wait for compiling
				swapDll();
				compilingState = CompilingState::Wait;
			}
		}
		if (scenes.find(pEvent->scene.name) == scenes.end())
			scenes[pEvent->scene.name] = mCurrentScene;
		else
		{
			mGameObjects.clear();
			mComponents.clear();
		}
	}

	template <typename T>
	void ScriptingSystem::CallbackReflectComponent(ReflectComponentEvent<T>* pEvent)
	{
		ReflectGameObject(pEvent->component.gameObj);
		ReflectComponent(pEvent->component);
	}

	void ScriptingSystem::CallbackScriptInvokeMethod(ScriptInvokeMethodEvent* pEvent)
	{
		auto it = mComponents[mCurrentScene].find(pEvent->script.uuid);
		MonoObject* mScript = mComponents[mCurrentScene][pEvent->script.uuid];
		//PRINT("Script Invoking " << pEvent->script.Name() << " " << pEvent->methodName << " ,ID: " << pEvent->script.uuid);
		COPIUM_ASSERT(!mScript, std::string("MONO OBJECT OF ") + pEvent->script.name + std::string(" NOT LOADED"));
		ScriptClass& scriptClass{ scriptClassMap[pEvent->script.name] };
		MonoMethod* mMethod{ mono_class_get_method_from_name (scriptClass.mClass,pEvent->methodName.c_str(),(int)pEvent->paramCount)};
		if (!mMethod && mono_class_get_parent(scriptClass.mClass) == mCopiumScript)
		{
			mMethod = mono_class_get_method_from_name(mCopiumScript, pEvent->methodName.c_str(), (int)pEvent->paramCount);
			if (!mMethod)
				return;
		}
		COPIUM_ASSERT(!mMethod, std::string("MONO METHOD ") + pEvent->methodName + std::string(" IN SCRIPT ") + pEvent->script.name + std::string(" NOT FOUND"));
		invoke(mScript,mMethod,pEvent->params);
	}

	void ScriptingSystem::CallbackScriptGetField(ScriptGetFieldEvent* pEvent)
	{
		MonoObject* mScript = mComponents[mCurrentScene][pEvent->script.uuid];
		COPIUM_ASSERT(!mScript, std::string("MONO OBJECT OF ") + pEvent->script.name + std::string(" NOT LOADED"));
		ScriptClass& scriptClass{ scriptClassMap[pEvent->script.name] };
		//Reset fieldtype and buffer if the type was different
		MonoClassField* mClassField{scriptClass.mFields[pEvent->fieldName]};
		COPIUM_ASSERT(!mClassField, std::string("FIELD ") + pEvent->fieldName + "COULD NOT BE FOUND IN SCRIPT " + pEvent->script.name);
		GetFieldValue(mScript,mClassField,pEvent->script.fieldDataReferences[pEvent->fieldName],pEvent->container);
	}

	void ScriptingSystem::CallbackScriptSetField(ScriptSetFieldEvent* pEvent)
	{
		MonoObject* mScript = mComponents[mCurrentScene][pEvent->script.uuid];
		COPIUM_ASSERT(!mScript, std::string("MONO OBJECT OF ") + pEvent->script.name + std::string(" NOT LOADED"));
		ScriptClass& scriptClass{ scriptClassMap[pEvent->script.name] };
		MonoClassField* mClassField{ scriptClass.mFields[pEvent->fieldName] };
		COPIUM_ASSERT(!mClassField, std::string("FIELD ") + pEvent->fieldName + "COULD NOT BE FOUND IN SCRIPT " + pEvent->script.name);
		SetFieldValue(mScript, mClassField, pEvent->script.fieldDataReferences[pEvent->fieldName], pEvent->data);
	}


	void ScriptingSystem::CallbackScriptGetMethodNames(ScriptGetMethodNamesEvent* pEvent)
	{
		static std::vector<const char*> functionNames{};
		functionNames.clear();
		ScriptClass& scriptClass{ scriptClassMap[pEvent->script.name] };
		for (auto& pair : scriptClass.mFields)
		{
			functionNames.push_back(pair.first.c_str());
		}
		pEvent->arraySize = functionNames.size();
		pEvent->namesArray = functionNames.data();
	}


	void ScriptingSystem::CallbackStartPreview(StartPreviewEvent* pEvent)
	{
		compilingState = CompilingState::Previewing;
		mPreviousScene = mCurrentScene;
		mCurrentScene = instantiateClass(klassScene);
		PRINT("SCENE IN SCRIPTING SWAPPED!");
	}

	void ScriptingSystem::CallbackScriptGetNames(ScriptGetNamesEvent* pEvent)
	{
		for (auto& pair : scriptClassMap)
		{
			if (NAME_TO_CTYPE[pair.first] == ComponentType::None)
				pEvent->names.push_back(pair.first.c_str());
		}
	}
}