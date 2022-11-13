/*!***************************************************************************************
\file			file-system.cpp
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
	Contains function definitions for the FileSystem

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

//#include <GLFW/glfw3.h>
#include "Files/file-system.h"
#include <utility>

namespace Copium
{
	namespace
	{
		namespace fs = std::filesystem;
	}

	void FileSystem::init()
	{
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_RELOAD_ASSETS, this);

		systemFlags |= FLAG_RUN_ON_EDITOR;

		init_file_types();
		
		// Instance ID of assets directory is always preset to a default ID
		generate_directories(&assetsDirectory, Paths::assetPath);

		update_file_references();

		/*PRINT("");
		print_directories(assetsDirectory, 0);
		PRINT("");*/
	}

	void FileSystem::init_file_types()
	{
		fileTypes.emplace(std::make_pair("", FileType("Folder", FOLDER)));
		fileTypes.emplace(std::make_pair(".wav", FileType("Audio", AUDIO)));
		fileTypes.emplace(std::make_pair(".ttf", FileType("Font", FONT)));
		fileTypes.emplace(std::make_pair(".json", FileType("Scene", SCENE))); // Bean: change to .scene in the future
		fileTypes.emplace(std::make_pair(".cs", FileType("Script", SCRIPT)));
		fileTypes.emplace(std::make_pair(".vert", FileType("Shader", SHADER))); // Bean: change to .shader in the future
		fileTypes.emplace(std::make_pair(".frag", FileType("Shader", SHADER)));
		fileTypes.emplace(std::make_pair(".png", FileType("Sprite", SPRITE)));
		fileTypes.emplace(std::make_pair(".txt", FileType("Text", TEXT)));
	}

	void FileSystem::update()
	{
		
	}

	void FileSystem::exit()
	{
		delete_directories(&assetsDirectory);
	}

	void FileSystem::handleMessage(MESSAGE_TYPE mType)
	{
		if (mType == MESSAGE_TYPE::MT_RELOAD_ASSETS)
		{
			//double start = glfwGetTime();
			update_directories(&assetsDirectory);
			update_file_references();
			//double end = glfwGetTime();

			//PRINT("File time taken to reload: " << end - start);
		}
	}

	void FileSystem::generate_directories(Directory* _directory, std::filesystem::path const& _path)
	{
		if (fs::exists(_path) && fs::is_directory(_path))
		{
			// Assign a path to the directory
			_directory->assign(_path);

			// Set instance index
			_directory->set_id(DEFAULT_INSTANCE_ID + indexes++);

			// Set folder name
			_directory->set_name(_path.filename().string());

			// Set files and folders within directory
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
			}
		}
	}

	void FileSystem::print_directories(Directory& _directory, int level)
	{
		std::string space = "";
		for (int i = 0; i < level; i++)
		{
			space += "  ";
		}
		level++;

		PRINT(space << "Directory: " << _directory.get_id() << " " << _directory.get_name());

		if (!_directory.get_child_directory().empty())
		{
			for (auto dir : _directory.get_child_directory())
			{
				print_directories(*dir, level);
			}
		}
		
		if (!_directory.get_files().empty())
		{
			for (auto& file : _directory.get_files())
			{
				PRINT(space << "  File: " << file.get_id() << " " << file.get_name());
			}
		}
	}

	void FileSystem::update_directories(Directory* _directory)
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
						}
						
					}
					// Store file
					else
					{
						// Check if the file exist 
						bool hasFile = false;
						for (auto assetDir : _directory->get_files())
						{
							if (path == assetDir)
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
							files[file.get_file_type().fileType].push_back(&file);
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
						if (!(*it).compare(dirEntry.path()))
						{
							hasFile = true;
							break;
						}
					}

					// Delete the file from the vector container
					if (!hasFile)
						it = (*filesPtr).erase(it);
					else
						it++;
				}
				(*filesPtr).shrink_to_fit();
			}

			for (auto& dirEntry : _directory->get_child_directory())
			{
				update_directories(dirEntry);
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

	Directory* FileSystem::get_directory(std::filesystem::path const& _path, Directory* _currentDir)
	{
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
				//std::cout << "Path name: " << entry.path().string() << "\n";
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
					//std::cout << "Path name: " << entry.path().string() << "\n";
				}
			}
		}

		return assetsPath;
	}

	std::list<File>& FileSystem::get_files_with_extension(const char* _extension)
	{
		if (extensionTrackedFiles.count(_extension) == 0)
			extensionTrackedFiles.emplace(std::make_pair(_extension, std::list<File>()));
		return extensionTrackedFiles[_extension];
	}
}