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
#include "CopiumCore/system-interface.h"
//#include "Scripting/script-wrappers.h"
#include <GameObject/components.h>
#include <Events/events-system.h>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"

#define SECONDS_TO_RECOMPILE 5
namespace
{
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

	void InitMono()
	{
		mono_set_assemblies_path("mono");
		mRootDomain = mono_jit_init("CopiumJITRuntime");
	}

	void ShutdownMono()
	{
		mono_jit_cleanup(mRootDomain);
		mRootDomain = nullptr;
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

	void ScriptingSystem::Init()
	{
		Utils::InitMono();
		//registerScriptWrappers();
		systemFlags |= FLAG_RUN_ON_EDITOR;
		PRINT("Starting Thread: " << std::this_thread::get_id());
		MyEventSystem.publish(new CreateThreadEvent(std::thread(&ScriptingSystem::ThreadRecompile,this)));
		//messageSystem.subscribe(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT, this);
		//messageSystem.subscribe(MESSAGE_TYPE::MT_SCENE_OPENED, this);
		//messageSystem.subscribe(MESSAGE_TYPE::MT_SCENE_DESERIALIZED, this);
		//messageSystem.subscribe(MESSAGE_TYPE::MT_ENGINE_INITIALIZED, this);
		//messageSystem.subscribe(MESSAGE_TYPE::MT_START_PREVIEW, this);
		//messageSystem.subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
		//messageSystem.subscribe(MESSAGE_TYPE::MT_COLLISION_ENTER, this);
	}

	void ScriptingSystem::Update()
	{
		//Waits for compiler thread to finish
		if (compilingState == CompilingState::SwapAssembly)
		{
			PRINT("SWAP PLS");
		}
	}

	void ScriptingSystem::Exit()
	{
		Utils::ShutdownMono();
	}

	void ScriptingSystem::ThreadRecompile()
	{
		bool running{true};
		while (running)
		{
			MyEventSystem.publish(new GetThreadStateEvent{std::this_thread::get_id(),running});
			Sleep(1000);
			AcquireMutex(FILESYSTEM_MUTEX);
			//Critical section
			//Critical section End
			ReturnMutex(FILESYSTEM_MUTEX);
		}
		PRINT("ThreadRecompile STOPPED");
	}


	void CallbackScriptFilesChanged(FilesChangedEvent<FileType::SCRIPT>* pEvent)
	{
		PRINT("SCRIPT FILES HAVE BEEN CHANGED");
	}
}