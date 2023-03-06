/*!***************************************************************************************
\file			compiler.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definition of functions for compiler

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once

#include <filesystem>
#include "Files/file-system.h"
#include "Events/events-system.h"


namespace Copium::Utils
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
		std::string command {Paths::roslynCompilerPath + " -target:library -out:scripts.dll"};
		for (fs::directory_entry p : fs::recursive_directory_iterator(Paths::projectPath))
		{
			if (p.path().extension() == ".cs")
				command += " " + p.path().string();
		}
		for (fs::directory_entry p : fs::recursive_directory_iterator(Paths::coreScriptsPath))
		{
			if (p.path().extension() == ".cs")
				command += " " + p.path().string();
		}


		command += " > output.txt";

		system(command.c_str());
		std::ifstream myfile("output.txt"); // this is equivalent to the above 
		std::string buffer;
		char mychar;
		if (!myfile.is_open())
			return;
		while (myfile) 
		{
			mychar = myfile.get();
			buffer += mychar;
		}
		MyEventSystem->publish(new EditorConsoleLogEvent(buffer));
		myfile.close();

	}
}
