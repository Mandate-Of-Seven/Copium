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
#include <SceneManager/sm.h>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"

#define SECONDS_TO_RECOMPILE 5
namespace
{
	Copium::MessageSystem& messageSystem{ Copium::MessageSystem::Instance() };
	MonoDomain* mRootDomain{ nullptr };		//JIT RUNTIME DOMAIN
	MonoDomain* mAppDomain{ nullptr };		//APP DOMAIN
	MonoAssembly* mCoreAssembly{ nullptr };	//ASSEMBLY OF SCRIPTS.DLL
	MonoImage* mAssemblyImage{ nullptr };	//LOADED IMAGE OF SCRIPTS.DLL
	MonoClass* mGameObject{ nullptr };
	MonoClass* mCopiumScript{ nullptr };
	MonoClass* mCollision2D{ nullptr };
	std::unordered_map<uint64_t,MonoObject*> monoGameObjects{};
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
	ScriptingSystem& sS{ ScriptingSystem::Instance() };
	bool scriptIsLoaded(const std::filesystem::path&);
	bool scriptPathExists(const std::filesystem::path& filePath);
	//ScriptingEngine Namespace Functions

#pragma region Struct ScriptMethods
	ScriptClass::ScriptClass(const std::string& _name, MonoClass* _mClass) :
		mClass{ _mClass },
		name{ _name }
	{
		mMethods["OnCreate"] = mono_class_get_method_from_name(mCopiumScript, "OnCreate", 1);
		mMethods["FindComponentByID"] = mono_class_get_method_from_name(mCopiumScript, "FindComponentByID", 2);
		mMethods["FindGameObjectByID"] = mono_class_get_method_from_name(mCopiumScript, "FindGameObjectByID", 1);
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
				if (fieldType != FieldType::None)
				{
					mFields[fieldName] = { fieldType, "", field, 0 };
				}
				else
				{
					static std::string typeName;
					typeName = mono_type_get_name(type);
					FieldFlag flag = 0;
					fieldType = FieldType::Component;
					//C# List
					if (typeName.starts_with("System.Collections.Generic.List<"))
					{
						typeName = typeName.substr(32);
						typeName.pop_back();
						flag |= FieldFlagList;
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
					mFields[fieldName] = { fieldType ,typeName.substr(offset),field, flag };
				}
			}
		}
	}
#pragma endregion
	void ScriptingSystem::instantiateCollision2D(GameObject& collided, GameObject& collidee)
	{
		MonoObject* collisionData = mono_object_new(mAppDomain, mCollision2D);
		MonoMethod* constructor = mono_class_get_method_from_name(mCollision2D, ".ctor", 1);
		GameObjectID gameObjID = collidee.id;
		void* data = mono_object_unbox(collisionData);
		void* param = &gameObjID;
		mono_runtime_invoke(constructor, collisionData, &param, nullptr);
		for (Script* script : collided.getComponents<Script>())
		{
			auto pair = script->pScriptClass->mMethods.find("OnCollisionEnter2D");
			if (pair != script->pScriptClass->mMethods.end())
			{

				mono_runtime_invoke((*pair).second, script->mObject, &data, nullptr);
			}
		}
	}

	void ScriptingSystem::addEmptyScript(const std::string& _name)
	{
		std::ofstream file(Paths::projectPath + _name + ".cs");
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
		ThreadSystem& tSys = ThreadSystem::Instance();
		while (!tSys.Quit())
		{
			while (compilingState != CompilingState::Wait);
			compilingState = CompilingState::Compiling;
			//Critical section
			while (!tSys.acquireMutex(MutexType::FileSystem));
			updateScriptFiles();
			tSys.returnMutex(MutexType::FileSystem);
			tryRecompileDll();
			//Critical section End
			Sleep(SECONDS_TO_RECOMPILE * 1000);
		}
	}

	ScriptingSystem::ScriptingSystem() :
		scriptFiles{ FileSystem::Instance().get_files_with_extension(".cs") }
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
		ThreadSystem::Instance().addThread(new std::thread(&ScriptingSystem::recompileThreadWork, this));
		messageSystem.subscribe(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT, this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_SCENE_OPENED, this);
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
		for (auto& it : scriptClassMap)
		{

			delete it.second;
		}
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

	ScriptClass* ScriptingSystem::getScriptClass(const std::string& _name)
	{
		if (mAssemblyImage == nullptr)
		{
			PRINT("ASSEMBLY IMAGE NOT LOADED!");
			return nullptr;
		}
		auto namePScriptClassPair = scriptClassMap.find(_name);
		ScriptClass* pScriptClass{ nullptr };
		if (namePScriptClassPair != scriptClassMap.end())
		{
			pScriptClass = (*namePScriptClassPair).second;
			return pScriptClass;
		}
		MonoClass* mClass = mono_class_from_name(mAssemblyImage, "", _name.c_str());
		if (mClass)
		{
			pScriptClass = new ScriptClass(_name, mClass);
			scriptClassMap.emplace(_name, pScriptClass);
		}
		return pScriptClass;
	}

	const std::unordered_map<std::string, ScriptClass*>& ScriptingSystem::getScriptClassMap()
	{
		return scriptClassMap;
	}

	void ScriptingSystem::updateScriptClasses()
	{
		using nameToClassIt = std::unordered_map<std::string, ScriptClass*>::iterator;
		nameToClassIt it = scriptClassMap.begin();
		static std::vector<nameToClassIt> keyMask;
		for (auto& keyVal : scriptClassMap)
		{
			MonoClass* mClass = mono_class_from_name(mAssemblyImage, "", keyVal.first.c_str());
			//If the class was successfully created, meaning it exists in the assembly
			delete keyVal.second;
			if (mClass)
			{
				keyVal.second = new ScriptClass(keyVal.first, mClass);
			}
			//If the class was not found in the assembly, meaning it doesnt exist in the assembly
			else
			{
				keyMask.push_back(it);
			}
			++it;
		}
		for (nameToClassIt& itMask : keyMask)
		{

			scriptClassMap.erase(itMask);
		}
		keyMask.clear();
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
		updateScriptClasses();
		if (!mCopiumScript)
			PRINT("COPIUM SCRIPT CANT BE FOUND");
		messageSystem.dispatch(MESSAGE_TYPE::MT_CREATE_CS_GAMEOBJECT);
		messageSystem.dispatch(MESSAGE_TYPE::MT_SCRIPTING_UPDATED);
		PRINT("END SWAP DLL_____________________________________");
	}

	void ScriptingSystem::invoke(MonoObject* mObj, MonoMethod* mMethod, void** params)
	{
		if (mObj && mMethod && mAppDomain)
		{
			mono_runtime_invoke(mMethod, mObj, params, nullptr);
		}
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
				scriptFiles.emplace(scriptFilesIt, pathRef);
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
				monoGameObjects[MESSAGE_CONTAINER::reflectCsGameObject.gameObjID] = mInstance;
				void* param = &MESSAGE_CONTAINER::reflectCsGameObject.gameObjID;
				mono_runtime_object_init(mInstance);
				mono_runtime_invoke(mSetID, mInstance, &param, nullptr);
				//MonoMethod* mAttachComponentByID = mono_class_get_method_from_name(mGameObject, "AttachComponentByID", 1);
				//for (uint64_t componentID : MESSAGE_CONTAINER::reflectCsGameObject.componentIDs)
				//{
				//	void* param = &componentID;
				//	mono_runtime_invoke(mAttachComponentByID, mInstance,&param,nullptr);
				//}
				break;
			}
			case MESSAGE_TYPE::MT_SCENE_OPENED:
			{
				while (compilingState == CompilingState::Compiling);
				compilingState = CompilingState::Deserializing;
				swapDll();
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
				GameObject* collidee = MESSAGE_CONTAINER::collisionEnter.collidee;
				GameObject* collided = MESSAGE_CONTAINER::collisionEnter.collided;
				instantiateCollision2D(*collided, *collidee);
				break;
			}
		}
	}
}