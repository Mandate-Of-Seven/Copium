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
#include <GameObject/Components/script-component.h>
#include <SceneManager/scene-manager.h>
#include <Events/events-system.h>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include <mono/metadata/mono-config.h>
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
		std::string typeName = mono_type_get_name(monoType);
		auto it = fieldTypeMap.find(typeName);
		if (it == fieldTypeMap.end())
			return FieldType::None;
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
					if (typeName.starts_with("System.Collections.Generic.List<"))
					{
						typeName = typeName.substr(32);
						typeName.pop_back();
					}

					auto it = fieldTypeMap.find(typeName);
					//Type that is in the fieldTypeMap
					if (it != fieldTypeMap.end())
					{
						fieldType = (*it).second;
					}

					size_t offset = typeName.rfind(".");
					if (offset == std::string::npos)
						offset = 0;
					else
						++offset;
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

	void ScriptingSystem::init()
	{
		initMono();
		registerScriptWrappers();
		systemFlags |= FLAG_RUN_ON_EDITOR;
		ThreadSystem::Instance()->addThread(new std::thread(&ScriptingSystem::recompileThreadWork, this));
		messageSystem.subscribe(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT, this);
		MyEventSystem->subscribe(this,&ScriptingSystem::CallbackSceneOpened);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackReflectComponent);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptInvokeMethod);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptGetMethodNames);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptSetField);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptGetField);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptSetFieldReference<Component>);
		MyEventSystem->subscribe(this, &ScriptingSystem::CallbackScriptSetFieldReference<GameObject>);
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
			registerComponents();
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
			return nullptr;
		MonoObject* tmp = mono_object_new(mAppDomain, mClass);
		mono_runtime_object_init(tmp);
		return tmp;
	}

	ScriptClass& ScriptingSystem::GetScriptClass(const std::string& _name)
	{
		auto namePScriptClassPair = scriptClassMap.find(_name);
		if (namePScriptClassPair != scriptClassMap.end())
		{
			return namePScriptClassPair->second;
		}
		MonoClass* mClass = mono_class_from_name(mAssemblyImage, "", _name.c_str());
		COPIUM_ASSERT(!mClass, std::string("SCRIPT ") + _name + std::string(" COULD NOT BE LOADED"));
		ScriptClass scriptClass{ _name, mClass };
		scriptClassMap.emplace(_name, std::move(scriptClass));
		return scriptClassMap[_name];
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
			}
			else if(mono_class_get_parent(_class) == mScriptableObject)
			{
				scriptableObjectClassMap[name] = ScriptClass{ name,_class };
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

	void ScriptingSystem::swapDll()
	{
		PRINT("SWAPPING DLL_____________________________________");
		unloadAppDomain();
		createAppDomain();
		mCoreAssembly = Utils::loadAssembly(Paths::scriptsAssemblyPath);
		mAssemblyImage = mono_assembly_get_image(mCoreAssembly);
		//Update scriptClasses
		mGameObject = mono_class_from_name(mAssemblyImage, "CopiumEngine", "GameObject");
		mCopiumScript = mono_class_from_name(mAssemblyImage, "CopiumEngine", "CopiumScript");
		mCollision2D = mono_class_from_name(mAssemblyImage, "CopiumEngine", "Collision2D");
		mScriptableObject = mono_class_from_name(mAssemblyImage, "CopiumEngine", "ScriptableObject");
		klassScene = mono_class_from_name(mAssemblyImage, "CopiumEngine", "Scene");
		updateScriptClasses();
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
					if (scriptFile && *scriptFile == pathRef)
					{
						maskScriptFiles.remove(scriptFile);
						break;
					}
				}
				++scriptFilesIt;
			}
		}
		// Remove deleted scripts using mask
		for (File* scriptFile : maskScriptFiles)
		{
			if (scriptFile != nullptr)
			{
				scriptFiles.remove(*scriptFile);
			}
		}
		maskScriptFiles.clear();
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
			if (*it == filePath) return true;
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
			case MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT:
			{
				if (!mAssemblyImage)
					return;
				MonoMethod* mSetID = mono_class_get_method_from_name(mGameObject, "setID", 1);
				MonoObject* mInstance = instantiateClass(mGameObject);
				void* param = &MESSAGE_CONTAINER::reflectCsGameObject.gameObjID;
				mono_runtime_invoke(mSetID, mInstance, &param, nullptr);
				//MonoMethod* mAttachComponentByID = mono_class_get_method_from_name(mGameObject, "AttachComponentByID", 1);
				//for (uint64_t componentID : MESSAGE_CONTAINER::reflectCsGameObject.componentIDs)
				//{
				//	void* param = &componentID;
				//	mono_runtime_invoke(mAttachComponentByID, mInstance,&param,nullptr);
				//}
				break;
			}
			case MESSAGE_TYPE::MT_SCENE_DESERIALIZED:
			{
				compilingState = CompilingState::Wait;
				break;
			}
			case MESSAGE_TYPE::MT_ENGINE_INITIALIZED:
			{
				if (!mAssemblyImage)
					return;
				//MonoObject* mGameObj = monoGameObjects[MESSAGE_CONTAINER::addOrDeleteComponent.gameObjID];
				//MonoMethod* mAttachComponentByID = mono_class_get_method_from_name(mGameObject, "AttachComponentByID", 1);
				//void* param = &MESSAGE_CONTAINER::addOrDeleteComponent.componentID;
				//mono_runtime_invoke(mAttachComponentByID, mGameObj, &param, nullptr);
				break;
			}
			case MESSAGE_TYPE::MT_START_PREVIEW:
			{
				//while (compilingState == CompilingState::Compiling);
				//if (compilingState == CompilingState::SwapAssembly)
				//	swapDll();
				compilingState = CompilingState::Previewing;
				//swapDll();
				break;
			}
			case MESSAGE_TYPE::MT_STOP_PREVIEW:
			{
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

	/*******************************************************************************
	/*!
	*
	\brief
		Sets a field from a C# field using its name
	\param name
		Name of the field
	\param value
		Value to write into C# memory space
	\return
		False if operation failed, true if it was successful
	*/
	/*******************************************************************************/
	void ScriptingSystem::SetFieldValue(MonoObject* instance, MonoClassField* mClassFiend, Field& field, const void* value)
	{
		if (field.fType == FieldType::String)
		{
			MonoString* mono_string = sS.createMonoString(reinterpret_cast<const char*>(value));
			mono_field_set_value(instance, mClassFiend, mono_string);
			return;
		}
		mono_field_set_value(instance, mClassFiend, (void*)value);
		return;
	}


	MonoObject* ScriptingSystem::ReflectGameObject(GameObjectID id)
	{
		auto pairIt = mGameObjects.find(id);
		if (mGameObjects.find(id) == mGameObjects.end())
		{
			void* param = &id;
			MonoMethod* reflectGameObject = mono_class_get_method_from_name(klassScene, "ReflectGameObject", 1);
			MonoObject* mGameObj= invoke(mCurrentScene, reflectGameObject, &param);
			mGameObjects.emplace(id, mGameObj);
			return mGameObj;
		}
		return (*pairIt).second;
	}

	MonoObject* ScriptingSystem::ReflectComponent(Component& component)
	{
		auto pairIt = mComponents.find(component.id);
		if (pairIt == mComponents.end())
		{
			ScriptClass& scriptClass = GetScriptClass(component.Name());
			ComponentID cid{ component.id };
			GameObjectID gid{ component.gameObj.id };
			void* params[3] = { instantiateClass(scriptClass.mClass) ,&cid,&gid };
			MonoMethod* reflectComponent = mono_class_get_method_from_name(klassScene, "ReflectComponent", 3);
			MonoObject* mComponent = invoke(mCurrentScene, reflectComponent, params);
			mComponents.emplace(gid, mComponent);
			//Check fields, dont remove fields, but change them if their type is different

			if (component.componentType == ComponentType::Script)
			{
				Script& script{ reinterpret_cast<Script&>(component) };
				for (auto pair : scriptClass.mFields)
				{
					MonoClassField* mField = pair.second;
					MonoType* type = mono_field_get_type(mField);
					FieldType fieldType = Utils::monoTypeToFieldType(type);
					const char* fieldName = mono_field_get_name(mField);
					auto nameField{ script.fieldDataReferences.find(fieldName) };
					int fieldSize = mono_type_size(type, NULL);
					if (nameField == script.fieldDataReferences.end())
					{
						mono_type_size(type, NULL);
						Field 
					}
					else
					{
						Field& field = nameField->second;
					}
				}
			}
			return mComponent;
		}
		return (*pairIt).second;
	}

	template<>
	MonoObject* ScriptingSystem::CreateReference<GameObject>(GameObject* object)
	{
		if (!object)
			return nullptr;
		return ReflectGameObject(object->id);
	}

	template<>
	MonoObject* ScriptingSystem::CreateReference<Component>(Component* object)
	{
		if (!object)
			return nullptr;
		ScriptClass& scriptClass = GetScriptClass(object->Name());
		ReflectGameObject(object->gameObj.id);
		return ReflectComponent(*object);
	}

	void ScriptingSystem::CallbackSceneOpened(SceneOpenedEvent* pEvent)
	{
		CompilingState state = compilingState;
		while (compilingState == CompilingState::Compiling);
		//If thread was Recompiling and is ready to swap
		if (state == CompilingState::Compiling && compilingState == CompilingState::SwapAssembly)
		{
			swapDll();
			registerComponents();
		}
		compilingState = CompilingState::Deserializing;
		mCurrentScene = instantiateClass(klassScene);
		scenes[pEvent->sceneName] = mCurrentScene;
	}

	void ScriptingSystem::CallbackReflectComponent(ReflectComponentEvent* pEvent)
	{
		ReflectGameObject(pEvent->component.gameObj.id);
		ReflectComponent(pEvent->component);
	}

	void ScriptingSystem::CallbackScriptInvokeMethod(ScriptInvokeMethodEvent* pEvent)
	{
		MonoObject* mScript = mComponents[pEvent->script.id];
		COPIUM_ASSERT(!mScript, std::string("MONO OBJECT OF ") + pEvent->script.name + std::string(" NOT LOADED"));
		ScriptClass& scriptClass{ GetScriptClass(pEvent->script.name) };
		MonoMethod* mMethod{ mono_class_get_method_from_name (scriptClass.mClass,pEvent->methodName.c_str(),pEvent->paramCount)};
		COPIUM_ASSERT(!mMethod, std::string("MONO METHOD ") + pEvent->methodName + std::string(" IN SCRIPT ") + pEvent->script.name + std::string(" NOT FOUND"));
		invoke(mScript,mMethod,pEvent->params);
	}

	void ScriptingSystem::CallbackScriptGetField(ScriptGetFieldEvent* pEvent)
	{
		MonoObject* mScript = mComponents[pEvent->script.id];
		COPIUM_ASSERT(!mScript, std::string("MONO OBJECT OF ") + pEvent->script.name + std::string(" NOT LOADED"));
		ScriptClass& scriptClass{ GetScriptClass(pEvent->script.name) };
		//Reset fieldtype and buffer if the type was different
		MonoClassField* mClassField{scriptClass.mFields[pEvent->fieldName]};
		COPIUM_ASSERT(!mClassField, std::string("FIELD ") + pEvent->fieldName + "COULD NOT BE FOUND IN SCRIPT " + pEvent->script.name);
		GetFieldValue(mScript,mClassField,pEvent->script.fieldDataReferences[pEvent->fieldName],pEvent->container);
	}

	void ScriptingSystem::CallbackScriptSetField(ScriptSetFieldEvent* pEvent)
	{
		MonoObject* mScript = mComponents[pEvent->script.id];
		COPIUM_ASSERT(!mScript, std::string("MONO OBJECT OF ") + pEvent->script.name + std::string(" NOT LOADED"));
		ScriptClass& scriptClass{ GetScriptClass(pEvent->script.name) };
		MonoClassField* mClassField{ scriptClass.mFields[pEvent->fieldName] };
		COPIUM_ASSERT(!mClassField, std::string("FIELD ") + pEvent->fieldName + "COULD NOT BE FOUND IN SCRIPT " + pEvent->script.name);
		SetFieldValue(mScript, mClassField, pEvent->script.fieldDataReferences[pEvent->fieldName], pEvent->data);
	}


	void ScriptingSystem::CallbackScriptGetMethodNames(ScriptGetMethodNamesEvent* pEvent)
	{
		static std::vector<const char*> functionNames{};
		functionNames.clear();
		ScriptClass& scriptClass{ GetScriptClass(pEvent->script.name) };
		for (auto pair : scriptClass.mFields)
		{
			functionNames.push_back(pair.first.c_str());
		}
		pEvent->arraySize = functionNames.size();
		pEvent->namesArray = functionNames.data();
	}
}