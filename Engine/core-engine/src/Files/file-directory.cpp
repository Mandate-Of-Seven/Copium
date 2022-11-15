/*!***************************************************************************************
\file			file-directory.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/11/2022

\brief
	Contains function definitions for the file directory

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

#include "Files/file-directory.h"

namespace Copium
{

	void Directory::inspector_view()
	{

	}

	bool Directory::within_directory(Directory* _directory)
	{
		for (auto dirEntry : std::filesystem::directory_iterator(path()))
		{
			if (dirEntry.path() == _directory->path())
				return true;
		}

		return false;
	}

	bool Directory::within_directory(File* _file)
	{
		for (auto dirEntry : std::filesystem::directory_iterator(path()))
		{
			if (dirEntry.path() == *_file)
				return true;
		}

		return false;
	}
}