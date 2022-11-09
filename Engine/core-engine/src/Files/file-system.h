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

namespace Copium
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

	namespace Paths
	{
		static const std::string roslynCompilerPath{ "..\\tools\\Roslyn\\csc" };
		static const std::string scriptsAssemblyPath{ "scripts.dll" };
		static const std::string projectName = { "PackedTracks\\" };
		static const std::string projectPath{ "..\\" + projectName };
		static const std::string assetPath{ projectPath + "Assets" };
		static const std::string coreScriptsPath{ "..\\CopiumScriptCore"};
	}

	enum FILE_TYPE
	{
		AUDIO,
		FONT,
		SCENE,
		SCRIPT,
		SHADER,
		SPRITE,
		TEXT,

		NUM_TYPES
	};

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

		std::list<std::string>& get_filepath_in_directory(const char* _path, const char* _extension);
		std::list<std::string>& get_filepath_in_directory(const char* _path, const char* _extension1, const char* _extension2);

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
		std::list<File>& get_files_with_extension(const char* _extension);
	private:
		std::map<const char*, std::list<File>> extensionTrackedFiles;
		std::list<std::string> assetsPath;
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
		bool is_modified();

		/*******************************************************************************
		/*!
		*
			\brief
				Queries the system whether the modification timings changed
		*/
		/*******************************************************************************/
		void update_modification_timing();

		const unsigned int& get_id() const { return instanceID; }
		const std::string& get_name() const { return name; }
		const FILE_TYPE& get_file_type() const { return fileType; }

	private:
		unsigned int instanceID; // The id to reference for the asset
		std::string name; // Name of the asset
		
		bool modified;
		time_t lastModifiedTime;

		FILE_TYPE fileType; // The type of file
	};

	class Directory
	{
	private:
		unsigned int instanceID; // The id to reference for the asset
		std::string name; // The name of the asset
		std::vector<File> files; // Files within the directory
		std::vector<Directory> folders; // Folders within the directory
	};
}

#endif // !FILE_SYSTEM_H