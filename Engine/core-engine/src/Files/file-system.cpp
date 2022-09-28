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

namespace Copium::Files
{
	File::File() : std::filesystem::path()
	{
		modified = true;
	}

	bool File::Modified()
	{
		if (modified)
		{
			modified = false;
			return true;
		}
		return modified;
	}

	void File::updateModificationTiming()
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

	std::list<File>& FileSystem::getFilesWithExtension(const char* _extension)
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