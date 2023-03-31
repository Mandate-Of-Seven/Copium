/*!***************************************************************************************
\file			file-system.cpp
\project
\author			Zacharie Hong
\co-authors		Sean Ngo

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
	Contains function definitions for the File system which loads files and folders into 
	the engine. This system checks for file modifications within a specific assets folder.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

#include <GLFW/glfw3.h>


#include "Files/file-system.h"
#include "Events/events-system.h"
#include "Editor/editor-system.h"

namespace Copium
{
	namespace
	{
		namespace fs = std::filesystem;
	}

	void FileSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR;

		MyEventSystem->subscribe(this, &FileSystem::CallbackSetSelectedFile);
		MyEventSystem->subscribe(this, &FileSystem::CallbackSetSelectedDirectory);
		MyEventSystem->subscribe(this, &FileSystem::CallbackDeleteFromBrowser);
		MyEventSystem->subscribe(this, &FileSystem::CallbackFileAccess);
		
		// Instance ID of assets directory is always preset to a default ID
		generate_directories(&assetsDirectory, Paths::assetPath);

		update_file_references();

		/*PRINT("");
		print_directories(assetsDirectory, 0);
		PRINT("");*/
	}

	void FileSystem::accept_dropped_files(int _pathCount, const char* _paths[])
	{
		// Retrieve all paths from the drop and store in a container
		std::list<fs::path> paths;
		for (int i = 0; i < _pathCount; i++)
			paths.push_back(_paths[i]);

		// Check which directory it is currently in
		// Create directories / folders / files in the directory
		for (auto path : paths)
		{
			fs::path currentDir = currentDirectory->path().string() + "\\";
			fs::path pathName = currentDir.string() + path.filename().string();
			//PRINT("  New path: " << pathName);
			
			// Check for duplicate folder / file
			if (fs::is_directory(path))
			{
				Directory* temp = get_directory(pathName, currentDirectory, true);
				int counter = 1;
				if (temp != nullptr)
				{
					fs::path editedPath;
					while (temp != nullptr)
					{
						editedPath = currentDir.string() + path.stem().string();
						editedPath += " " + std::to_string(counter++) + path.extension().string();
						temp = get_directory(editedPath, currentDirectory, true);
					}
					fs::copy(path, editedPath, fs::copy_options::recursive);
				}
				else
					fs::copy(path, pathName, fs::copy_options::recursive);
			}
			else
			{
				copy_file(path);
			}
		}
	}

	void FileSystem::update()
	{
		static double timer = 0; 
		timer += MyFrameRateController.getFixedDt();

		if (timer >= 0.5)
		{
			check_directory_count(&assetsDirectory);
			timer = 0.0;
		}
	}

	void FileSystem::exit()
	{
		delete_directories(&assetsDirectory);
	}

	void FileSystem::generate_directories(Directory* _directory, std::filesystem::path const& _path)
	{
		if (fs::exists(_path) && fs::is_directory(_path))
		{
			// Assign the path to the directory
			_directory->assign(_path);

			// Set instance index
			_directory->set_id(DEFAULT_INSTANCE_ID + indexes++);

			// Set folder name
			_directory->set_name(_path.filename().string());

			// Set files and folders within directory
			int count = 0;
			for (auto& dirEntry : fs::directory_iterator(_path))
			{
				fs::path path = dirEntry.path();
				// Store in folders and access it
				if (dirEntry.is_directory())
				{
					//Directory folder;
					Directory* folder = new Directory;
					folder->set_parent_directory(_directory);

					// Generate folder
					generate_directories(folder, path);

					// Add folder into this directory
					_directory->add_child_directory(folder);
				}
				// Store file
				else 
				{
					File file(path);
					file.set_id(DEFAULT_INSTANCE_ID + indexes++);
					file.set_name(path.stem().string());
					file.set_file_type(get_file_type(path.extension().string()));
					_directory->add_files(file);
				}

				count++;
			}

			_directory->set_file_count(count);
		}
	}

	void FileSystem::print_directories(Directory& _directory, int _level)
	{
		std::string space = "";
		for (int i = 0; i < _level; i++)
		{
			space += "  ";
		}
		_level++;

		PRINT(space << "Directory: " << _directory.get_id() << " " << _directory.get_name());

		if (!_directory.get_child_directory().empty())
		{
			for (auto dir : _directory.get_child_directory())
			{
				print_directories(*dir, _level);
			}
		}
		
		//if (!_directory.get_files().empty())
		//{
		//	for (auto& file : _directory.get_files())
		//	{
		//		PRINT(space << "  File: " << file.get_id() << " " << file.get_name());
		//	}
		//}
	}

	void FileSystem::update_directories(Directory* _directory, bool _recursive)
	{
		if (fs::exists(_directory->path()) && fs::is_directory(_directory->path()))
		{
			// Check for number of files in this directory
			unsigned int numFiles = 0;
			for (auto dirEntry : fs::directory_iterator(_directory->path()))
			{
				(void) dirEntry;
				numFiles++;
			}

			// There is a change in number of files
			if (_directory->get_child_directory().size() + _directory->get_files().size() != (size_t)numFiles)
			{
				// For checking current directory selection
				//Directory* currentDir = editor->get_content_browser()->get_current_directory();

				// Check for new files and folders
				for (auto& dirEntry : fs::directory_iterator(_directory->path()))
				{
					fs::path path = dirEntry.path();
					// Store in folders and access it
					if (dirEntry.is_directory())
					{
						Directory tempFolder;
						tempFolder.assign(path);

						// Check if the folder exist 
						bool hasFolder = false;
						for (auto assetDir : _directory->get_child_directory())
						{
							if (tempFolder == *assetDir)
							{
								hasFolder = true;
								break;
							}
						}

						// Directory folder;
						if (!hasFolder)
						{
							Directory* folder = new Directory;
							folder->set_parent_directory(_directory);

							// Generate folder
							generate_directories(folder, path);

							// Add folder into this directory
							_directory->add_child_directory(folder);

							// Bean: This should be moved to a general function
							// Prevent selection of file / directory
							if (currentDirectory->within_directory(folder))
								selectedDirectory = nullptr;
						}
						
					}
					// Store file
					else
					{
						// Check if the file exist 
						bool hasFile = false;
						for (auto assetDir : _directory->get_files())
						{
							if (path == assetDir.filePath)
							{
								hasFile = true;
								break;
							}
						}

						if (!hasFile)
						{
							File file(path);
							file.set_id(DEFAULT_INSTANCE_ID + indexes++);
							file.set_name(path.stem().string());
							file.set_file_type(get_file_type(path.extension().string()));
							_directory->add_files(file);
							add_file_reference(&_directory->get_files().back());

							// Generate Meta File
							//if(file.get_file_type().fileType == FILE_TYPE::SPRITE)
							//	MyAssetsSystem.GenerateMetaFile(&_directory->get_files().back());

							// Bean: This should be moved to a general function
							// Prevent selection of file / directory
							if (currentDirectory->within_directory(&file))
								selectedFile = nullptr;
						}
					}
				}

				// Check for deleted folders
				std::vector<Directory*> iterators;
				std::vector<Directory*>* dirs = &_directory->get_child_directory();
				for (auto it = (*dirs).begin(); it != (*dirs).end();)
				{
					bool hasFolder = false;
					for (auto& dirEntry : fs::directory_iterator(_directory->path()))
					{
						if ((*it)->path() == dirEntry.path())
						{
							hasFolder = true;
							break;
						}
					}

					// Delete the folder from the vector container
					if (!hasFolder)
					{
						// Bean: This should be moved to a general function
						// Prevent selection of file / directory
						if (currentDirectory == _directory && selectedDirectory != nullptr)
							selectedDirectory = nullptr;

						iterators.push_back(*it);
						it = (*dirs).erase(it);
					}
					else
						it++;
				}
				(*dirs).shrink_to_fit();

				for (auto dir : iterators)
				{
					delete_directories(dir);
					delete dir;
				}

				// Check for deleted files
				std::vector<File>* filesPtr = &_directory->get_files();
				for (auto it = (*filesPtr).begin(); it != (*filesPtr).end();)
				{
					
					bool hasFile = false;
					for (auto& dirEntry : fs::directory_iterator(_directory->path()))
					{
						if (!(*it).filePath.compare(dirEntry.path()))
						{
							hasFile = true;
							break;
						}
					}

					// Delete the file from the vector container
					if (!hasFile)
					{
						// Bean: This should be moved to a general function
						// Prevent selection of file / directory
						if (currentDirectory == _directory && selectedFile != nullptr)
							selectedFile = nullptr;

						remove_file_reference(&(*it));
						it = (*filesPtr).erase(it);
					}
					else
						it++;
				}
				(*filesPtr).shrink_to_fit();
			}

			if (_recursive)
			{
				for (auto& dirEntry : _directory->get_child_directory())
				{
					update_directories(dirEntry);
				}
			}
		}
	}

	void FileSystem::check_directory_count(Directory* _directory, bool _recursive)
	{
		int fileCount = 0;
		for (auto dirEntry : fs::directory_iterator(_directory->path()))
		{
			(void)dirEntry;
			fileCount++;
		}

		// Check if there is a change in the number of files
		if (_directory->get_file_count() != 0 && _directory->get_file_count() != fileCount)
		{
			update_directories(_directory, false);
		}

		_directory->set_file_count(fileCount);

		if (_recursive)
		{
			for (Directory* dirEntry : _directory->get_child_directory())
			{
				check_directory_count(dirEntry);
			}
		}
	}

	Directory* FileSystem::get_directory(std::filesystem::path const& _path)
	{
		if (_path == assetsDirectory.path())
			return &assetsDirectory;

		if (!assetsDirectory.get_child_directory().empty())
		{
			Directory* temp = get_directory(_path, &assetsDirectory);
			if (temp != nullptr)
				return temp;
		}

		return nullptr;
	}

	Directory* FileSystem::get_directory(std::filesystem::path const& _path, Directory* _currentDir, bool _withinDirectory)
	{
		if (_withinDirectory)
		{
			for (Directory* dir : _currentDir->get_child_directory())
			{
				if (_path == dir->path())
					return dir;
			}

			return nullptr;
		}

		for (Directory* dir : _currentDir->get_child_directory())
		{
			if (_path == dir->path())
				return dir;

			if (!dir->get_child_directory().empty())
			{
				Directory* temp = get_directory(_path, dir);
				if (temp != nullptr)
					return temp;
			}
		}

		return nullptr;
	}

	Directory* FileSystem::get_directory(std::string const& _directoryName)
	{
		if (!assetsDirectory.get_name().compare(_directoryName))
			return &assetsDirectory;

		if (!assetsDirectory.get_child_directory().empty())
		{
			Directory* temp = get_directory(_directoryName, &assetsDirectory);
			if (temp != nullptr)
				return temp;
		}

		return nullptr;
	}

	Directory* FileSystem::get_directory(std::string const& _directoryName, Directory* _currentDir)
	{
		for (Directory* dir : _currentDir->get_child_directory())
		{
			if (!dir->get_name().compare(_directoryName))
				return dir;

			if (!dir->get_child_directory().empty())
			{
				Directory* temp = get_directory(_directoryName, dir);
				if (temp != nullptr)
					return temp;
			}
		}

		return nullptr;
	}

	File* FileSystem::get_file(std::filesystem::path const& _path)
	{
		if (!assetsDirectory.get_child_directory().empty())
		{
			File* file = get_file(_path, &assetsDirectory);
			if (file != nullptr)
				return file;
		}

		return nullptr;
	}

	File* FileSystem::get_file(std::filesystem::path const& _path, Directory* _currentDir, bool _withinDirectory)
	{
		for (File& file : _currentDir->get_files())
		{
			if (_path == file.filePath)
				return &file;
		}

		if (_withinDirectory)
			return nullptr;

		if (!_currentDir->get_child_directory().empty())
		{
			for (Directory* dir : _currentDir->get_child_directory())
			{
				File* file = get_file(_path, dir, false);
				if (file != nullptr)
					return file;
			}
		}

		return nullptr;
	}

	void FileSystem::copy_file(std::filesystem::path const& _path)
	{
		fs::path currentDir = currentDirectory->path().string() + "\\";
		fs::path pathName = currentDir.string() + _path.filename().string();
		File* temp = get_file(pathName, currentDirectory, true);
		int counter = 1;
		if (temp != nullptr)
		{
			fs::path editedPath;
			while (temp != nullptr)
			{
				editedPath = currentDir.string() + _path.stem().string();
				editedPath += " " + std::to_string(counter++) + _path.extension().string();
				temp = get_file(editedPath, currentDirectory, true);
			}
			fs::copy(_path, editedPath);
		}
		else
			fs::copy(_path, pathName);
	}

	void FileSystem::copy_file(std::filesystem::path const& _path, const std::string& _ext)
	{
		fs::path currentDir = currentDirectory->path().string() + "\\";
		fs::path pathName = currentDir.string() + _path.filename().string();
		File* temp = get_file(pathName, currentDirectory, true);
		int counter = 1;
		if (temp != nullptr)
		{
			fs::path editedPath;
			while (temp != nullptr)
			{
				editedPath = currentDir.string() + _path.stem().string();
				editedPath += " " + std::to_string(counter++) + _ext;
				temp = get_file(editedPath, currentDirectory, true);
			}
			fs::copy(_path, editedPath);
		}
		else
		{
			pathName = currentDir.string() + _path.stem().string() + _ext;
			fs::copy(_path, pathName);
		}
	}

	void FileSystem::delete_directories(Directory* _directory)
	{
		if (!_directory->get_child_directory().empty())
		{
			for (Directory* dir : _directory->get_child_directory())
			{
				if (!dir->get_child_directory().empty())
				{
					delete_directories(dir);
				}

				delete dir;
			}
		}
	}

	void FileSystem::update_file_references()
	{
		files.clear();
		store_file_references(&assetsDirectory);
	}

	void FileSystem::delete_from_browser()
	{
		PRINT("Attemp to delete object...");
		if (selectedFile != nullptr)
		{
			MyEventSystem->publish(new FileAssetEvent(selectedFile, 2));
			PRINT("Deleting: " << selectedFile->filePath.filename() << " With result: " << DeleteFile(selectedFile->filePath.c_str()));
			remove_file_reference(selectedFile);
			selectedFile = nullptr;
		}
		else if (selectedDirectory != nullptr)
		{
			fs::path tmp = selectedDirectory->path();
			if (std::filesystem::remove_all(tmp))
			{
				update_directories(selectedDirectory->get_parent_directory());
				PRINT("Delete complete");
			}
			else
			{
				PRINT("Delete failed, could not find folder at: " << tmp);
			}

			selectedDirectory = nullptr;
		}
	}

	std::list<std::string>& FileSystem::get_filepath_in_directory(const char* _path, const char* _extension)
	{
		// Empty the files in the list
		assetsPath.clear();
		for (const auto& entry : fs::recursive_directory_iterator(_path))
		{
			if (entry.is_directory())
			{
				//get_filepath_in_directory(entry.path().generic_string().c_str(), _extension);
				//PRINT("Dir: " << entry.path().string());
			}
			// Is a file of type extension
			else if (!entry.is_directory() && !entry.path().extension().generic_string().compare(_extension)) 
			{
				assetsPath.push_back(entry.path().generic_string());
			}

		}

		return assetsPath;
	}

	std::list<std::string>& FileSystem::get_filepath_in_directory(const char* _path, const char* _extension1, const char* _extension2)
	{
		// Empty the files in the list
		assetsPath.clear();
		for (const auto& entry : fs::recursive_directory_iterator(_path))
		{
			if (entry.is_directory())
			{
				//get_filepath_in_directory(entry.path().generic_string().c_str(), _extension);
				//PRINT("Dir: " << entry.path().string());
			}
			// Is a file of type extension
			else if (!entry.is_directory())
			{
				bool extension1 = !entry.path().extension().generic_string().compare(_extension1);
				bool extension2 = !entry.path().extension().generic_string().compare(_extension2);
				if (extension1 || extension2)
				{
					assetsPath.push_back(entry.path().generic_string());
				}
			}
		}

		return assetsPath;
	}

	std::list<File>& FileSystem::get_files_with_extension(const char* _extension)
	{
		//if (extensionTrackedFiles.count(_extension) == 0)
		//	extensionTrackedFiles.emplace(std::make_pair(_extension, std::list<File>()));
		return extensionTrackedFiles[_extension];
	}

	void FileSystem::store_file_references(Directory* _directory)
	{
		for (auto& fileEntry : _directory->get_files())
		{
			files[fileEntry.get_file_type().fileType].push_back(&fileEntry);
		}

		for (auto dirEntry : _directory->get_child_directory())
		{
			store_file_references(dirEntry);
		}
	}

	void FileSystem::add_file_reference(File* _file)
	{
		files[_file->get_file_type().fileType].push_back(_file);
		MyEventSystem->publish(new FileAssetEvent(_file, 1));
	}

	void FileSystem::remove_file_reference(File* _file)
	{
		files[_file->get_file_type().fileType].remove(_file);
		MyEventSystem->publish(new FileAssetEvent(_file, 2));
	}

	void FileSystem::CallbackSetSelectedFile(SetSelectedFileEvent* pEvent)
	{
		set_selected_file(pEvent->file);
	}

	void FileSystem::CallbackSetSelectedDirectory(SetSelectedDirectoryEvent* pEvent)
	{
		set_selected_directory(pEvent->directory);
	}

	void FileSystem::CallbackDeleteFromBrowser(DeleteFromBrowserEvent* pEvent)
	{
		delete_from_browser();
	}

	void FileSystem::CallbackFileAccess(FileAccessEvent* pEvent)
	{
		PRINT("Opening file: " << pEvent->name << "...");
		ShellExecuteA(NULL, "open", pEvent->name, NULL, NULL, SW_SHOWNORMAL);
	}
}