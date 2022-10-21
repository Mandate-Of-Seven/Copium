/*!***************************************************************************************
\file			file-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
	Contains function declarations for the FileSystem

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "CopiumCore/system-interface.h"
#include <filesystem>
#include <string>
#include <list>
#include <map>

namespace Copium::Files
{
	/**************************************************************************/
	/*!
		\brief
			Reads bytes from a file
		\param filepath
			File path to read bytes from
		\param outSize
			Amount of bytes read

		\return
			String of bytes read
	*/
	/**************************************************************************/
	static char* ReadBytes(const std::string& filepath, uint32_t* outSize)
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

	namespace Paths
	{
		static const std::string roslynCompilerPath{ "..\\tools\\Roslyn\\csc" };
		static const std::string scriptsAssemblyPath{ "scripts.dll" };
		static const std::string projectName = { "ScriptingSandbox" };
		static const std::string projectPath{ "..\\" + projectName };
	}

	class File;

	CLASS_SYSTEM(FileSystem)
	{
	public:
		/*******************************************************************************
		/*!
		*
			\brief
				Runs at the start of the engine
		*/
		/*******************************************************************************/
		void init();

		/*******************************************************************************
		/*!
		*
			\brief
				Runs at every frame of the engine
		*/
		/*******************************************************************************/
		void update();


		/*******************************************************************************
		/*!
		*
			\brief
				Runs at end of the engine
		*/
		/*******************************************************************************/
		void exit();

		/*******************************************************************************
		/*!
		*
			\brief
				Gets a list of files with an extension in its name

			\param _extension
				Extension in file to look for

			\return
				Reference to list of files with the extension
		*/
		/*******************************************************************************/
		std::list<File>& getFilesWithExtension(const char* _extension);
	private:
		std::map<const char*, std::list<File>> extensionTrackedFiles;
	};

	class File final : public std::filesystem::path
	{
	public:
		/*******************************************************************************
		/*!
		*
			\brief
				Constructor of a file to use std::filesystem constructor
		*/
		/*******************************************************************************/
		File();
		/*******************************************************************************
		/*!
		*
			\brief
				Conversion constructor of a file

			\param pathRef
				std::filesystem::path to convert into a file
		*/
		/*******************************************************************************/
		File(const std::filesystem::path& pathRef);

		/*******************************************************************************
		/*!
		*
			\brief
				Uses winAPI to track for modification timings and set modified back to
				false if it were true

			\return
				If file was modified, return true, else return false
		*/
		/*******************************************************************************/
		bool Modified();

		/*******************************************************************************
		/*!
		*
			\brief
				Queries the system whether the modification timings changed
		*/
		/*******************************************************************************/
		void updateModificationTiming();
	private:
		bool modified;
		time_t lastModifiedTime;
	};
}

#endif // !FILE_SYSTEM_H