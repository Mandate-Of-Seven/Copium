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
#include "Scripting/scriptWrappers.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"

#define SECONDS_TO_RECOMPILE 5



namespace
{
	Copium::Message::MessageSystem* messageSystem{ Copium::Message::MessageSystem::Instance() };
	enum class CompilingState
	{
		Compiling,
		SwapAssembly,
		Wait
	};
	CompilingState compilingState{ CompilingState::Wait};
	MonoDomain* mRootDomain{ nullptr };		//JIT RUNTIME DOMAIN
	MonoDomain* mAppDomain{ nullptr };		//APP DOMAIN
	MonoAssembly* mCoreAssembly{ nullptr };	//ASSEMBLY OF SCRIPTS.DLL
	MonoImage* mAssemblyImage{ nullptr };	//LOADED IMAGE OF SCRIPTS.DLL
	MonoClass* mGameObject{ nullptr };
	MonoClass* mCopiumScript{ nullptr };

}

namespace Copium::Utils
{
	static char* readBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream) return nullptr;
		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();
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

	Scripting::FieldType monoTypeToFieldType(MonoType* monoType)
	{
		std::string typeName = mono_type_get_name(monoType);
		using namespace Scripting;
		auto it = fieldTypeMap.find(typeName);
		COPIUM_ASSERT(it == fieldTypeMap.end(), "Invalid monoType")
		return it->second;
	}
}

namespace Copium::Scripting
{
	ScriptingSystem& sS{ *ScriptingSystem::Instance() };
	bool scriptIsLoaded(const std::filesystem::path&);
	bool scriptPathExists(const std::filesystem::path& filePath);
	//ScriptingEngine Namespace Functions

	#pragma region Struct ScriptMethods
		ScriptClass::ScriptClass(const std::string& _name, MonoClass* _mClass) : 
		mClass{ _mClass},
		name{_name},
		mAwake{ mono_class_get_method_from_name(mClass, "Awake", 0) },
		mStart{ mono_class_get_method_from_name(mClass, "Start", 0) },
		mUpdate{ mono_class_get_method_from_name(mClass, "Update", 0) },
		mLateUpdate{ mono_class_get_method_from_name(mClass, "LateUpdate", 0) },
		mOnCollisionEnter{ mono_class_get_method_from_name(mClass, "OnCollisionEnter", 0) },
		mOnCreate{ mono_class_get_method_from_name(mCopiumScript, "OnCreate", 1) }
		{
			int fieldCount = mono_class_num_fields(mClass);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(mClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					FieldType fieldType = Utils::monoTypeToFieldType(type);
					m_Fields[fieldName] = { fieldType, fieldName, field };
				}
			}
		}
	#pragma endregion

	// Gets the accessibility level of the given field
	/*unsigned char GetFieldAccessibility(MonoClassField* field)
	{
		unsigned char accessibility = (unsigned char)Accessibility::None;
		unsigned int accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

		switch (accessFlag)
		{
			case MONO_FIELD_ATTR_PRIVATE:
			{
				accessibility = (unsigned char)Accessibility::Private;
				break;
			}
			case MONO_FIELD_ATTR_FAM_AND_ASSEM:
			{
				accessibility |= (unsigned char)Accessibility::Protected;
				accessibility |= (unsigned char)Accessibility::Internal;
				break;
			}
			case MONO_FIELD_ATTR_ASSEMBLY:
			{
				accessibility = (unsigned char)Accessibility::Internal;
				break;
			}
			case MONO_FIELD_ATTR_FAMILY:
			{
				accessibility = (unsigned char)Accessibility::Protected;
				break;
			}
			case MONO_FIELD_ATTR_FAM_OR_ASSEM:
			{
				accessibility |= (unsigned char)Accessibility::Private;
				accessibility |= (unsigned char)Accessibility::Protected;
				break;
			}
			case MONO_FIELD_ATTR_PUBLIC:
			{
				accessibility = (unsigned char)Accessibility::Public;
				break;
			}
		}
		return accessibility;
	}*/

	void ScriptingSystem::recompileThreadWork()
	{
		Thread::ThreadSystem& tSys = *Thread::ThreadSystem::Instance();
		while (!tSys.Quit())
		{
			while (compilingState != CompilingState::Wait);
			compilingState = CompilingState::Compiling;
			//Critical section
			updateScriptFiles();
			tryRecompileDll();
			//Critical section End
			Sleep(SECONDS_TO_RECOMPILE*1000);
		}

	}

	ScriptingSystem::ScriptingSystem() :
		scriptFiles{ Copium::Files::FileSystem::Instance()->get_files_with_extension(".cs") }
	{

	}

	void ScriptingSystem::init()
	{
		initMono();
		registerScriptWrappers();
		Thread::ThreadSystem::Instance()->addThread(new std::thread(&ScriptingSystem::recompileThreadWork,this));
		messageSystem->subscribe(Message::MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT, this);
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
			return nullptr;
		auto namePScriptClassPair = scriptClassMap.find(_name);
		ScriptClass* pScriptClass{nullptr};
		if (namePScriptClassPair != scriptClassMap.end())
		{
			pScriptClass = (* namePScriptClassPair).second;
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

	void ScriptingSystem::updateScriptClasses()
	{
		using nameToClassIt = std::unordered_map<std::string, ScriptClass*>::iterator;
		nameToClassIt it = scriptClassMap.begin();
		static std::vector<nameToClassIt> keyMask;
		for (auto& keyVal : scriptClassMap)
		{
			MonoClass* mClass = mono_class_from_name(mAssemblyImage,"", keyVal.first.c_str());
			//If the class was successfully created, meaning it exists in the assembly
			delete keyVal.second;
			if (mClass)
			{
				keyVal.second = new ScriptClass(keyVal.first,mClass);
			}
			//If the class was not found in the assembly, meaning it doesnt exist in the assembly
			else
			{
				keyMask.push_back(it);
			}
			++it;
		}
		for (nameToClassIt& it : keyMask)
		{

			scriptClassMap.erase(it);
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
		unloadAppDomain();
		createAppDomain();
		mCoreAssembly = Utils::loadAssembly(Files::Paths::scriptsAssemblyPath);
		mAssemblyImage = mono_assembly_get_image(mCoreAssembly);
		//Update scriptClasses
		mGameObject = mono_class_from_name(mAssemblyImage, "CopiumEngine", "GameObject");
		mCopiumScript = mono_class_from_name(mAssemblyImage, "CopiumEngine", "CopiumScript");
		updateScriptClasses();
		if (!mCopiumScript)
			PRINT("COPIUM SCRIPT CANT BE FOUND");
		messageSystem->dispatch(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED);
	}

	void ScriptingSystem::invoke(MonoObject* mObj, MonoMethod* mMethod, void** params)
	{
		if (mObj && mMethod)
		{
			mono_runtime_invoke(mMethod, mObj, params, nullptr);
		}
	}

	

	void ScriptingSystem::updateScriptFiles()
	{
		//Check for new files
		namespace fs = std::filesystem;
		using scriptFileListIter = std::list<Files::File>::iterator;
		scriptFileListIter scriptFilesIt = scriptFiles.begin();
		static std::list<Files::File*> maskScriptFiles;
		maskScriptFiles.resize(scriptFiles.size());
		for (Files::File& file : scriptFiles)
		{
			maskScriptFiles.push_back(&file);
		}
		for (fs::directory_entry p : fs::recursive_directory_iterator(Files::Paths::projectPath))
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
				for (Files::File* scriptFile : maskScriptFiles)
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
		for (Files::File* scriptFile : maskScriptFiles)
		{
			if (scriptFile != nullptr)
			{
				scriptFiles.remove(*scriptFile);
			}
		}
		maskScriptFiles.clear();
	}

	void ScriptingSystem::tryRecompileDll()
	{
		compilingState = CompilingState::Wait;
		bool startCompiling = false;
		for (Files::File& scriptFile : scriptFiles)
		{
			scriptFile.update_modification_timing();
			if (scriptFile.is_modified() && !startCompiling)
			{
				startCompiling = true;
				Compiler::compileDll();
				compilingState = CompilingState::SwapAssembly;
			}
		}
	}

	bool ScriptingSystem::scriptIsLoaded(const std::filesystem::path& filePath)
	{
		using scriptFileListIter = std::list<Files::File>::iterator;
		for (scriptFileListIter it = scriptFiles.begin(); it != scriptFiles.end(); ++it)
		{
			if (*it == filePath) return true;
		}
		return false;
	}

	void ScriptingSystem::reflectGameObject(uint64_t _ID)
	{
		MonoMethod* mSetID = mono_class_get_method_from_name(mGameObject, "setID", 1);
		MonoObject* mInstance = instantiateClass(mGameObject);
		void* param = &_ID;
		mono_runtime_object_init(mInstance);
		mono_runtime_invoke(mSetID, mInstance, &param, nullptr);
	}

	void ScriptingSystem::handleMessage(Message::MESSAGE_TYPE mType)
	{
		//MT_REFLECT_CS_GAMEOBJECT
		reflectGameObject(Message::MESSAGE_CONTAINER::reflectCsGameObject.ID);
	}
}