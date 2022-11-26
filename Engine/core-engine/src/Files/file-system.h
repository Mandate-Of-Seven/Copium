/*!***************************************************************************************
\file			file-system.h
\project
\author			Zacharie Hong
\co-authors		Sean Ngo

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
	Contains function declarations for the File system to load file and folders into the
	engine.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "CopiumCore/system-interface.h"
#include "Files/file-directory.h"
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
		static const std::string dataPath{ "Data" };
	}

	CLASS_SYSTEM(FileSystem)
	{
	public:
		/*******************************************************************************
		/*!
		\brief
			Runs at the start of the engine
		*/
		/*******************************************************************************/
		void init();

		/*******************************************************************************
		/*!
		\brief
			Runs at every frame of the engine
		*/
		/*******************************************************************************/
		void update();


		/*******************************************************************************
		/*!
		\brief
			Runs at end of the engine
		*/
		/*******************************************************************************/
		void exit();

		/*******************************************************************************
		/*!
		\brief
			Initializes all file types that was retrieve from the configuration files
		*/
		/*******************************************************************************/
		void init_file_types();

		/*******************************************************************************
		/*!
		\brief
			Accept dropped file from external folders into the content browser and pass
			the information into this function to generate a file
		\param _pathCount
			The number of files or folders to accept
		\param _paths[]
			The string path of the files to accept 
		*/
		/*******************************************************************************/
		void accept_dropped_files(int _pathCount, const char* _paths[]);

		// Bean: This should be in the directory class
		/*******************************************************************************
		/*!
		\brief
			Generate directories within the path given
		\param _directory
			The directory to store the files and sub directories
		\param _path
			The path to generate the directories
		*/
		/*******************************************************************************/
		void generate_directories(Directory* _directory, std::filesystem::path const& _path);

		// Bean: This should be in the directory class
		/*******************************************************************************
		/*!
		*
		\brief
			Print the files and folders within the directory
		\param _directory
			The directory to print
		\param _level
			The current sub directory from the main directory accessed in levels
		*/
		/*******************************************************************************/
		void print_directories(Directory& _directory, int _level);

		/*******************************************************************************
		/*!
		\brief
			Updates the directory specified by the parameter
		\param _directory
			The directory to update
		\param _recursive
			The option to either update this directory or all directories within this 
			directory
		*/
		/*******************************************************************************/
		void update_directories(Directory* _directory, bool _recursive = true);

		/*******************************************************************************
		/*!
		\brief
			Check the current number of files and sub directories within the specified 
			directory
		\param _directory
			The directory to check
		\param _recursive
			The option to either check this directory or all directories within this
			directory
		*/
		/*******************************************************************************/
		void check_directory_count(Directory* _directory, bool _recursive = true);

		/*******************************************************************************
		/*!
		\brief
			Get the directory using the path
		\param _path
			The path that is related to the directory
		\return
			The directory
		*/
		/*******************************************************************************/
		Directory* get_directory(std::filesystem::path const& _path);

		/*******************************************************************************
		/*!
		\brief
			Get the directory using the name of the directory (Only gets the first 
			instance of the directory with said name)
		\param _directoryName
			The name that is related to the directory
		\return
			The directory
		*/
		/*******************************************************************************/
		Directory* get_directory(std::string const& _directoryName);

		/*******************************************************************************
		/*!
		\brief
			Get the file using the path
		\param _path
			The path that is equivalent to the file stored in the directories
		\return
			The file
		*/
		/*******************************************************************************/
		File* get_file(std::filesystem::path const& _path);

		// Bean: This should be in the directory class
		/*******************************************************************************
		/*!
		\brief
			Delete the directories specified in the parameter
		\param _directory
			The directory to delete
		*/
		/*******************************************************************************/
		void delete_directories(Directory* _directory);

		/*******************************************************************************
		/*!
		\brief
			Updates the relevant file references and the file types
		*/
		/*******************************************************************************/
		void update_file_references();

		Directory& get_asset_directory() { return assetsDirectory; }
		void set_asset_directory(Directory& _directory) { assetsDirectory = _directory; }

		const unsigned int& get_indexes() const { return indexes; }

		const FileType& get_file_type(std::string const& _ext) { return fileTypes[_ext]; }
		std::unordered_map<FILE_TYPE, std::list<File*>>& get_file_references() { return files; }

		File* get_selected_file() { return selectedFile; }
		void set_selected_file(File* _file) { selectedFile = _file; }

		/*******************************************************************************
		/*!
		*
		\brief
			Get all file paths with the said extension in the path
		\param _path
			The path to retrieve the file paths
		\param _extension
			Extension in file to look for
		\return
			Reference to list of files with the extension
		*/
		/*******************************************************************************/
		std::list<std::string>& get_filepath_in_directory(const char* _path, const char* _extension);
		
		/*******************************************************************************
		/*!
		*
		\brief
			Get all file paths with the said extension in the path
		\param _path
			The path to retrieve the file paths
		\param _extension1
			Extension in file to look for
		\param _extension2
			Extension in file to look for
		\return
			Reference to list of files with the extension
		*/
		/*******************************************************************************/
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
		/*******************************************************************************
		/*!
		\brief
			Get the directory using the path
		\param _path
			The path that is related to the directory
		\param _currentDir
			The current directory that the function is in
		\param _withinDirectory
			To check if the function is recursive to search within the sub directories
		\return
			The directory
		*/
		/*******************************************************************************/
		Directory* get_directory(std::filesystem::path const& _path, Directory* _currentDir, bool _withinDirectory = false);
		
		/*******************************************************************************
		/*!
		\brief
			Get the directory using the name of the directory (Only gets the first
			instance of the directory with said name)
		\param _directoryName
			The name that is related to the directory
		\param _currentDir
			The current directory that the function is in
		\return
			The directory
		*/
		/*******************************************************************************/
		Directory* get_directory(std::string const& _directoryName, Directory* _currentDir);
		
		/*******************************************************************************
		/*!
		\brief
			Get the file using the path
		\param _path
			The path that is equivalent to the file stored in the directories
		\param _currentDir
			The current directory that the function is in
		\param _withinDirectory
			To check if the function is recursive to search within the sub directories
		\return
			The file
		*/
		/*******************************************************************************/
		File* get_file(std::filesystem::path const& _path, Directory* _currentDir, bool _withinDirectory = false);
		
		/*******************************************************************************
		/*!
		\brief
			Store the file references from the files in the directory
		\param _directory
			The directory to search the files to store the references
		*/
		/*******************************************************************************/
		void store_file_references(Directory* _directory);
		
		/*******************************************************************************
		/*!
		\brief
			Add the file reference into the files
		\param _file
			The file to add the file reference to
		*/
		/*******************************************************************************/
		void add_file_reference(File* _file);
		
		/*******************************************************************************
		/*!
		\brief
			Remove the file reference from the files
		\param _file
			The file to remove the file reference 
		*/
		/*******************************************************************************/
		void remove_file_reference(File* _file);

	private:
		std::map<const char*, std::list<File>> extensionTrackedFiles;
		std::list<std::string> assetsPath;
		Directory assetsDirectory;
		unsigned int indexes = 0; // Number of file & directory instances
		std::unordered_map<std::string, FileType> fileTypes;
		std::unordered_map<FILE_TYPE, std::list<File*>> files; // A list of files in their categories
		File* selectedFile = nullptr;
		Directory* selectedDirectory = nullptr;
	};
}

#endif // !FILE_SYSTEM_H