#include "pch.h"
#include "scripting-system.h"
#include "compiler.h"
#include "scripting.h"
#include "file-path.h"
#include "system-interface.h"


#include <Windows.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/threads.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>

namespace 
{
	void initMono();
	void shutdownMono();
	void loadScriptFiles();
	void loadClasses();
	void tryRecompileDll();
	std::list<Copium::Scripting::ScriptFile*> scriptFiles;
}

namespace Mono
{
	MonoDomain* rootDomain{ nullptr };		//JIT RUNTIME DOMAIN
	MonoAssembly* coreAssembly{ nullptr };	//ASSEMBLY OF SCRIPTS.DLL
	MonoImage* assemblyImage{ nullptr };	//LOADED IMAGE OF SCRIPTS.DLL
	std::vector<MonoClass*> monoClasses;	//CLASSES INSIDE IMAGE OF SCRIPTS.DLL
}

namespace Copium::Scripting
{
	//ScriptingEngine Namespace Variables
	std::list<std::filesystem::path> scriptPaths;
	bool scriptIsLoaded(const std::filesystem::path&);
	bool scriptPathExists(const std::filesystem::path& filePath);
	//ScriptingEngine Namespace Functions
	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);

	#pragma region Class ScriptFile Definitions
		ScriptFile::ScriptFile(const std::filesystem::path& _filePath)
		{
			filePath = _filePath;
		}

		void ScriptFile::Modified(bool _modified)
		{
			modified = _modified;
		}

		bool ScriptFile::Modified() const
		{
			return modified;
		}

		void ScriptFile::updateModificationTiming()
		{
			struct _stat64i32 statsBuffer;
			_stat(filePath.string().c_str(), &statsBuffer);
			if (lastModifiedTime != statsBuffer.st_mtime)
			{
				modified = true;
				lastModifiedTime = statsBuffer.st_mtime;
			}
		}

		const std::filesystem::path& ScriptFile::FilePath() const
		{
			return filePath;
		}


		void ScriptFile::FilePath(const std::filesystem::path& _filePath)
		{
			filePath = _filePath;
		}

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

	void recompileThreadWork()
	{
		while (true)
		{
			Sleep(5 * 1000);
			PRINT("Recompiler Thread looking for new files!");
			tryRecompileDll();
		}
	}

	void ScriptingSystem::init()
	{
		initMono();
		//static std::thread compilerThread(recompileThreadWork);
	}

	void ScriptingSystem::update()
	{
		//Every few seconds check if there was a scriptfile that was added removed or changed
		//If yes, recompile
		//One done recompiling, reload DLL
		//if ()
		//trySwapDll(recompileThread);
	}

	void ScriptingSystem::exit()
	{
		shutdownMono();
		for (Scripting::ScriptFile* scriptFile : scriptFiles)
		{
			delete scriptFile;
		}
		scriptFiles.clear();
	}

	void trySwapDll(std::thread& compilerThread)
	{
		if (!compilerThread.joinable())
		{
			PRINT("WAITING FOR COMPILER");
			return;
		}
		Mono::coreAssembly = LoadCSharpAssembly(
			std::filesystem::current_path().string() + "\\..\\x64\\Debug\\scripts.dll");
		Mono::assemblyImage = mono_assembly_get_image(Mono::coreAssembly);
		loadClasses();
	}


	// Create cs file and make it inherit from newScript.cs, then create mono class from it so that ScriptComponents can use it
// to create an instance of the MonoClass
	MonoClass* loadMonoClass(const std::string& name)
	{
		for (MonoClass* script : Mono::monoClasses)
		{
			if (mono_class_get_name(script) == name)
			{
				PRINT(name + " already exists!");
				return script;
			}
			else
			{
				PRINT(mono_class_get_name(script));
			}
		}

		const std::string scriptPath = FilePath::projectPath + "\\" + name + ".cs";
		std::ofstream scriptFile(scriptPath);

		scriptFile << "public class " << name << " : NewScript" << std::endl;
		scriptFile << "{\n\tvoid Start()\n\t{\n\n\t}\n\n\tvoid Update()\n\t{\n\n\t}\n}" << std::endl;
		scriptFile.close();
		MonoClass* script = mono_class_from_name(Mono::assemblyImage, "", name.c_str());
		Mono::monoClasses.push_back(script);
		return script;
	}

	MonoObject* newScriptInstance(MonoClass*& monoClass)
	{
		return mono_object_new(Mono::rootDomain, monoClass);
	}

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
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

	

	bool scriptIsLoaded(const std::filesystem::path& filePath)
	{
		using scriptFileListIter = std::list<ScriptFile*>::iterator;
		for (scriptFileListIter it = scriptFiles.begin(); it != scriptFiles.end(); ++it)
		{
			if ((*it)->FilePath() == filePath) return true;
		}
		return false;
	}

	bool scriptPathExists(const std::filesystem::path& filePath)
	{
		using scriptPathIter = std::list<std::filesystem::path>::const_iterator;
		for (scriptPathIter it = scriptPaths.begin(); it != scriptPaths.end(); ++it)
		{
			if (*it == filePath)
				return true;
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




	////Function
	//MonoMethod* printMsgFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
	//mono_runtime_invoke(printMsgFunc, instance, nullptr, nullptr);

	////Function with params
	//MonoMethod* printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
	//int value = 5; int value1 = 69;

	//void* params[2]{ &value,&value1 };
	//mono_runtime_invoke(printIntsFunc, instance, params, nullptr);

	//MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
	//MonoString* str = mono_string_new(Mono::AppDomain, "Hello world from C++!");

	//void* param{str};
	//mono_runtime_invoke(printCustomMessageFunc, instance, &param, nullptr);
}

namespace
{
	using namespace Copium;
	using namespace Scripting;
	void initMono()
	{
		loadScriptFiles();
		mono_set_assemblies_path("mono");
		Mono::rootDomain = mono_jit_init("CopiumJITRuntime");
		//tryRecompileDll();

		//void* propIter = nullptr;
		//MonoClassField* raw_property = NULL;
		//raw_property = mono_class_get_fields(Mono::monoClasses[0], &propIter);
		//while (raw_property = mono_class_get_fields(Mono::monoClasses[0], &propIter)) {
		//	std::string name = mono_field_get_name(raw_property);
		//	if (GetFieldAccessibility(raw_property) & (unsigned char)Accessibility::Public)
		//	{
		//		if (GetType(mono_field_get_type(raw_property)) == FieldType::String)
		//			PRINT("Name: " << name << " Type: String Access: Public");
		//	}
		//}


		////Function
		//MonoMethod* printMsgFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		//mono_runtime_invoke(printMsgFunc, instance, nullptr, nullptr);

		////Function with params
		//MonoMethod* printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		//int value = 5; int value1 = 69;

		//void* params[2]{ &value,&value1 };
		//mono_runtime_invoke(printIntsFunc, instance, params, nullptr);

		//MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		//MonoString* str = mono_string_new(Mono::AppDomain, "Hello world from C++!");

		//void* param{str};
		//mono_runtime_invoke(printCustomMessageFunc, instance, &param, nullptr);
	}

	void loadScriptFiles()
	{
		scriptFiles.clear();
		namespace fs = std::filesystem;
		for (fs::directory_entry p : fs::recursive_directory_iterator(FilePath::projectPath))
		{
			if (p.path().extension() == ".cs")
				scriptFiles.push_back(new ScriptFile(p.path()));
		}
	}

	void loadClasses()
	{
		Mono::monoClasses.clear();
		for (const ScriptFile* scriptFile : scriptFiles)
		{
			std::string scriptPath{ scriptFile->FilePath().stem().string() };
			Mono::monoClasses.push_back(mono_class_from_name(Mono::assemblyImage, "", scriptPath.c_str()));
		}
	}

	void shutdownMono()
	{
		for (ScriptFile* scriptFile : scriptFiles)
		{
			delete scriptFile;
		}
		scriptFiles.clear();
		mono_jit_cleanup(Mono::rootDomain);
		Mono::rootDomain = nullptr;
	}

	void tryRecompileDll()
	{
		namespace fs = std::filesystem;
		using scriptFileListIter = std::list<ScriptFile*>::iterator;
		scriptFileListIter scriptFilesIt = scriptFiles.begin();
		for (fs::directory_entry p : fs::recursive_directory_iterator(FilePath::projectPath))
		{
			const fs::path pathRef{ p.path() };
			if (pathRef.extension() != "cs")
				scriptPaths.push_back(pathRef);
			if (!scriptIsLoaded(pathRef))
			{
				scriptFiles.emplace(scriptFilesIt);
			}
			++scriptFilesIt;
		}

		scriptFilesIt = scriptFiles.begin();
		scriptFileListIter scriptFilesEnd = scriptFiles.end();
		while (scriptFilesIt != scriptFilesEnd)
		{
			if (!scriptPathExists((*scriptFilesIt)->FilePath()))
			{
				delete* scriptFilesIt;
				scriptFiles.erase(scriptFilesIt);
				scriptFilesEnd = scriptFiles.end();
				PRINT("Recompiler: Changes Found!");
			}
			++scriptFilesIt;
		}

		//Clear remaining in buffer and files;
		for (ScriptFile* scriptFile : scriptFiles)
		{
			scriptFile->updateModificationTiming();
		}
		bool modified = false;
		for (ScriptFile* scriptFile : scriptFiles)
		{
			scriptFile->updateModificationTiming();
			if (modified)
			{
				scriptFile->Modified(false);
				continue;
			}
			if (scriptFile->Modified())
			{
				PRINT("RECOMPILING");
				scriptFile->Modified(false);
				Compiler::compileDll();
			}
		}
	}
}