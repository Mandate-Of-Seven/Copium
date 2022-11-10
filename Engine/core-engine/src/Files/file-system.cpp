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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#include "pch.h"
#include "../Files/file-system.h"
#include <utility>

namespace Copium
{
	File::File() : std::filesystem::path()
	{
		modified = true;
	}

	bool File::is_modified()
	{
		if (modified)
		{
			modified = false;
			return true;
		}
		return modified;
	}

	void File::update_modification_timing()
	{
		struct _stat64i32 statsBuffer;
		_stat(string().c_str(), &statsBuffer);
		if (lastModifiedTime != statsBuffer.st_mtime)
		{
			modified = true;
			lastModifiedTime = statsBuffer.st_mtime;
		}
	}

	void FileSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR;

		// Instance ID of assets directory is always preset to a default ID
		assetsDirectory.set_id(DEFAULT_INSTANCE_ID);
		std::filesystem::path assetsPath = Paths::assetPath;
		for (auto& dirEntry : std::filesystem::recursive_directory_iterator(assetsPath))
		{
			if (dirEntry.is_directory())
			{
				//assetsDirectory.
			}
		}
	}

	void FileSystem::update()
	{

	}

	void FileSystem::exit()
	{

	}

	std::list<std::string>& FileSystem::get_filepath_in_directory(const char* _path, const char* _extension)
	{
		// Empty the files in the list
		assetsPath.clear();
		for (const auto& entry : std::filesystem::recursive_directory_iterator(_path))
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
		for (const auto& entry : std::filesystem::recursive_directory_iterator(_path))
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


	File::File(const std::filesystem::path& pathRef) : std::filesystem::path(pathRef)
	{
		modified = true;
	}
}