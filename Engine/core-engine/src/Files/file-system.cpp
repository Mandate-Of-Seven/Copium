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
#include "../Files/file-system.h"
#include <utility>

namespace Copium::Files
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

		for (const auto& entry : std::filesystem::directory_iterator(_path))
		{
			if (entry.is_directory())
			{
				get_filepath_in_directory(entry.path().generic_string().c_str(), _extension);
			}
			// Is a file of type extension
			else if (!entry.is_directory() && !entry.path().extension().generic_string().compare(_extension)) 
			{
				assetsPath.push_back(entry.path().generic_string());
				std::cout << "Path name: " << entry.path().generic_string() << "\n";
			}

		}

		if (!assetsPath.empty())
			return assetsPath;
		else
		{
			assetsPath.clear();
			return assetsPath;
		}
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