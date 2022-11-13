/*!***************************************************************************************
\file			file-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
	Contains function declarations for the FileSystem

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "CopiumCore/system-interface.h"
#include "Files/file-directory.h"
#include "Messaging/message-system.h"
#include <string>
#include <list>
#include <map>
#include <memory>

#define DEFAULT_INSTANCE_ID 1000

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

	CLASS_SYSTEM(FileSystem), public IReceiver
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

		void handleMessage(MESSAGE_TYPE mType);

		void init_file_types();

		// Bean: This should be in the directory class
		void generate_directories(Directory* _directory, std::filesystem::path const& _path);

		// Bean: This should be in the directory class
		void print_directories(Directory& _directory, int level);

		void update_directories(Directory* _directory);

		Directory* get_directory(std::filesystem::path const& _path);
		Directory* get_directory(std::string const& _directoryName);

		// Bean: This should be in the directory class
		void delete_directories(Directory* _directory);

		void update_file_references();

		Directory& get_asset_directory() { return assetsDirectory; }
		void set_asset_directory(Directory& _directory) { assetsDirectory = _directory; }

		const unsigned int& get_indexes() const { return indexes; }

		const FileType& get_file_type(std::string const& _ext) { return fileTypes[_ext]; }
		std::unordered_map<FILE_TYPE, std::list<File*>>& get_file_references() { return files; }

		File* get_selected_file() { return selectedFile; }
		void set_selected_file(File* _file) { selectedFile = _file; }

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
		Directory* get_directory(std::filesystem::path const& _path, Directory* _currentDir);
		Directory* get_directory(std::string const& _directoryName, Directory* _currentDir);
		void store_file_references(Directory* _directory);

	private:
		std::map<const char*, std::list<File>> extensionTrackedFiles;
		std::list<std::string> assetsPath;
		Directory assetsDirectory;
		unsigned int indexes = 0; // Number of file & directory instances
		std::unordered_map<std::string, FileType> fileTypes;
		std::unordered_map<FILE_TYPE, std::list<File*>> files; // A list of files in their categories
		File* selectedFile;
		Directory* selectedDirectory;
	};
}

#endif // !FILE_SYSTEM_H