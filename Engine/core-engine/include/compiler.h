/*!***************************************************************************************
\file			compiler.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definition of functions for compiler

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once

#include <filesystem>
#include "file-system.h"


namespace Copium::Scripting::Compiler
{
	namespace fs = std::filesystem;

	/**************************************************************************/
	/*!
	  \brief
		Compiles all CS scripts in project path using Roslyn Compiler
	*/
	/**************************************************************************/
	static void compileDll()
	{
		std::string command {Files::Paths::roslynCompilerPath + " -target:library -out:scripts.dll"};
		for (fs::directory_entry p : fs::recursive_directory_iterator(Files::Paths::projectPath))
		{
			if (p.path().extension() == ".cs")
				command += " " + p.path().string();
		}
		PRINT(command);
		system(command.c_str());
	}
}
