#pragma once

#include <filesystem>
#include "Scripting/file-path.h"

#define RECOMPILE_SCRIPTS_COOLDOWN 2.f

namespace Copium::Scripting::Compiler
{

	namespace fs = std::filesystem;
	//static void compileObject(const fs::path& scriptPath)
	//{
	//	if (!fs::exists(scriptsCachePath))
	//		fs::create_directory(scriptsCachePath);
	//	std::string command =
	//		"..\\tools\\Roslyn\\csc -target:module -out:" +
	//		scriptsCachePath + scriptPath.stem().string() + ".o " + // ..\\cachepath\\script.o
	//		scriptPath.string();									// Script to compile
	//	system(command.c_str());
	//}

	//static void compileDll()
	//{
	//	std::string command = "..\\tools\\Roslyn\\csc -target:library -out:scripts.dll " + FilePath::projectPath + "\\Z.poop";
	//	for (fs::directory_entry p : fs::recursive_directory_iterator(scriptsCachePath))
	//	{
	//		if (p.path().extension() != ".o")
	//			continue;
	//		command += " /addmodule:" + p.path().string();
	//	}
	//	PRINT(command);
	//	system(command.c_str());
	//}

	static void compileDll()
	{
		std::string command {FilePath::roslynCompilerPath + " -target:library -out:scripts.dll"};
		for (fs::directory_entry p : fs::recursive_directory_iterator(FilePath::projectPath))
		{
			if (p.path().extension() == ".cs")
				command += " " + p.path().string();
		}
		system(command.c_str());
	}
}
