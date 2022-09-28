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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#include "pch.h"
#include "scripting-system.h"
#include "compiler.h"
#include "scripting.h"
#include "file-system.h"
#include "system-interface.h"
#include "thread-system.h"

#include <Windows.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/threads.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>

namespace Copium::Scripting
{
	//ScriptingEngine Namespace Variables
	Copium::Scripting::ScriptingSystem& sS{ *Copium::Scripting::ScriptingSystem::Instance() };
	bool scriptIsLoaded(const std::filesystem::path&);
	bool scriptPathExists(const std::filesystem::path& filePath);
	//ScriptingEngine Namespace Functions
	MonoAssembly* loadAssembly(const std::string& assemblyPath);

	#pragma region Struct ScriptMethods
		ScriptClass::ScriptClass(MonoClass* _mClass) : mClass{_mClass}, 
		mAwake{ mono_class_get_method_from_name(_mClass, "Awake", 0) },
		mStart{ mono_class_get_method_from_name(_mClass, "Start", 0) },
		mUpdate{ mono_class_get_method_from_name(_mClass, "Update", 0) },
		mLateUpdate{ mono_class_get_method_from_name(_mClass, "LateUpdate", 0) },
		mOnCollisionEnter{ mono_class_get_method_from_name(_mClass, "OnCollisionEnter", 0) }{}
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
		
		sS.initMono();
		while (!tSys.Quit())
		{
			updateScriptFiles();
			tryRecompileDll();
			//MonoMethod* printMsgFunc = mono_class_get_method_from_name(mmClasses[2], "PrintMessage", 0);
			////// Allocate an instance of our class
			//MonoObject* classInstance = mono_object_new(mAppDomain, mmClasses[2]);
			//if (classInstance == nullptr)
			//{
			//	PRINT("Unable to create class instance");
			//	// Log error here and abort
			//}
			//// Call the parameterless (default) constructor
			//mono_runtime_object_init(classInstance);
			//mono_runtime_invoke(printMsgFunc, classInstance, nullptr, nullptr);
			PRINT("Recompiler Thread looking for new files!");
			Sleep(5 * 1000);
		}
		sS.shutdownMono();
	}

	ScriptingSystem::ScriptingSystem() :
		scriptFiles{ Copium::Files::FileSystem::Instance()->getFilesWithExtension(".cs") }
	{

	}

	void ScriptingSystem::init()
	{
		Thread::ThreadSystem::Instance()->addThread("Compiler Thread",
			new std::thread(&ScriptingSystem::recompileThreadWork,this));
	}

	void ScriptingSystem::update()
	{

	}

	void ScriptingSystem::exit()
	{
	}

	MonoObject* ScriptingSystem::createMonoObject(MonoClass* mClass)
	{
		return mono_object_new(mAppDomain, mClass);
	}

	std::shared_ptr<ScriptClass> ScriptingSystem::getScriptClass(const char* _name)
	{
		if (mAssemblyImage == nullptr)
			return nullptr;
		for (auto spScriptClass : scriptClasses)
		{
			if (spScriptClass.use_count() == 1)
			{
				spScriptClass.reset();
				spScriptClass = std::make_shared<ScriptClass>(mono_class_from_name(mAssemblyImage, "", _name));
				return spScriptClass;
			}
		}
		scriptClasses.push_back(std::make_shared<ScriptClass>(mono_class_from_name(mAssemblyImage, "", _name)));
		return scriptClasses.back();
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

	MonoAssembly* loadAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
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
		bool compiling = false;
		for (Files::File& scriptFile : scriptFiles)
		{
			scriptFile.updateModificationTiming();
			if (scriptFile.Modified() && !compiling)
			{
				PRINT(scriptFile.string() << " Changed! ");
				PRINT("Compiling DLL....");
				compiling = true;
				Compiler::compileDll();
				swapDll();
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

	static void CppFunc()
	{
		std::cout << "This is a cpp function" << std::endl;
	}

	static void NativeLog()
	{
		std::cout << "This is a cpp function" << std::endl;
	}
}