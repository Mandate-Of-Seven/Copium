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
#include "Scripting/scripting.h"
#include "Utilities/thread-system.h"
#include "Files/file-system.h"
#include "CopiumCore/system-interface.h"
#include "Messaging/message-system.h"
#include "Messaging/message-types.h"

#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/threads.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>

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
}

namespace Copium::Scripting
{
	//ScriptingEngine Namespace Variables
	Copium::Scripting::ScriptingSystem& sS{ *Copium::Scripting::ScriptingSystem::Instance() };
	bool scriptIsLoaded(const std::filesystem::path&);
	bool scriptPathExists(const std::filesystem::path& filePath);
	//ScriptingEngine Namespace Functions
	MonoAssembly* loadAssembly(const std::string& assemblyPath);

	#pragma region Struct ScriptMethods
		ScriptClass::ScriptClass(const std::string& _name) : 
		mClass{ mono_class_from_name(mAssemblyImage,"",_name.c_str())},
		name{_name},
		mAwake{ mono_class_get_method_from_name(mClass, "Awake", 0) },
		mStart{ mono_class_get_method_from_name(mClass, "Start", 0) },
		mUpdate{ mono_class_get_method_from_name(mClass, "Update", 0) },
		mLateUpdate{ mono_class_get_method_from_name(mClass, "LateUpdate", 0) },
		mOnCollisionEnter{ mono_class_get_method_from_name(mClass, "OnCollisionEnter", 0) }{}
	#pragma endregion

	// Gets the accessibility level of the given field
	unsigned char GetFieldAccessibility(MonoClassField* field)
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
	}

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
		scriptFiles{ Copium::Files::FileSystem::Instance()->getFilesWithExtension(".cs") }
	{

	}

	void ScriptingSystem::init()
	{
		initMono();
		Thread::ThreadSystem::Instance()->addThread(new std::thread(&ScriptingSystem::recompileThreadWork,this));
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

	void ScriptingSystem::handleMessage(Message::MESSAGE_TYPE mType)
	{

	}

	MonoObject* ScriptingSystem::createMonoObject(MonoClass* mClass)
	{
		return mono_object_new(mAppDomain, mClass);
	}

	std::shared_ptr<ScriptClass> ScriptingSystem::getScriptClass(const std::string& _name)
	{
		if (mAssemblyImage == nullptr)
			return nullptr;
		auto nameSpScriptClassPair = scriptClassMap.find(_name);
		std::shared_ptr<ScriptClass>* sp;
		if (nameSpScriptClassPair != scriptClassMap.end())
		{
			sp = &(*nameSpScriptClassPair).second;
			if (*sp == nullptr)
			{
				*sp = std::make_shared<ScriptClass>(_name);
				return *sp;
			}
			return *sp;
		}
		scriptClassMap.emplace(_name, std::make_shared<ScriptClass>(_name));
		return scriptClassMap[_name];
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
		mono_domain_set(mRootDomain, false);
		mono_domain_unload(mAppDomain);
		PRINT("DOMAIN UNLOADED");
		mAppDomain = nullptr;
	}

	void ScriptingSystem::swapDll()
	{
		PRINT("SWAPPING AND LOADING ASSEMBLY...");
		if (mAppDomain)
			unloadAppDomain();
		createAppDomain();
		mCoreAssembly = loadAssembly(Files::Paths::scriptsAssemblyPath);
		//mono_assemblies_init();
		mAssemblyImage = mono_assembly_get_image(mCoreAssembly);
		messageSystem->dispatch(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED);
		//void* propIter = nullptr;
		//MonoClassField* raw_property = NULL;
		//raw_property = mono_class_get_fields(mmClasses[0], &propIter);
		//while (raw_property = mono_class_get_fields(mmClasses[0], &propIter)) {
		//	std::string name = mono_field_get_name(raw_property);
		//	if (GetFieldAccessibility(raw_property) & (unsigned char)Accessibility::Public)
		//	{
		//		if (GetType(mono_field_get_type(raw_property)) == FieldType::String)
		//			PRINT("Name: " << name << " Type: String Access: Public");
		//	}
		//}
	}

	void ScriptingSystem::invoke(MonoObject* mObj, MonoMethod* mMethod)
	{
		if (mObj && mMethod)
		{
			mono_runtime_invoke(mMethod, mObj, nullptr, nullptr);
		}
	}

	MonoAssembly* loadAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

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

	void ScriptingSystem::updateScriptFiles()
	{
		//Check for new files
		namespace fs = std::filesystem;
		using scriptFileListIter = std::list<Files::File>::iterator;
		scriptFileListIter scriptFilesIt = scriptFiles.begin();
		std::list<Files::File*> maskScriptFiles;
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
				PRINT("SCRIPT WAS NOT LOADED: " << pathRef.stem().string());
				scriptFiles.emplace(scriptFilesIt, pathRef);
			}
			//Script was already loaded
			else
			{
				PRINT("SCRIPTS ALREADY LOADED: " << pathRef.stem().string());
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
	}

	void ScriptingSystem::tryRecompileDll()
	{
		compilingState = CompilingState::Wait;
		bool startCompiling = false;
		for (Files::File& scriptFile : scriptFiles)
		{
			scriptFile.updateModificationTiming();
			if (scriptFile.Modified() && !startCompiling)
			{
				PRINT(scriptFile.string() << " Changed! ");
				PRINT("Compiling DLL....");
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

	/*static void CppFunc()
	{
		std::cout << "This is a cpp function" << std::endl;
	}

	static void NativeLog()
	{
		std::cout << "This is a cpp function" << std::endl;
	}*/
}