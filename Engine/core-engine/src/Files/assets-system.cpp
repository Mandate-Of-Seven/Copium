/*!***************************************************************************************
\file			assets-systemcpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			20/10/2022

\brief
	Contains function definitions for Assets system

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

#include "Files/assets-system.h"
#include "Files/file-system.h"

namespace Copium::Assets
{
	void AssetsSystem::init()
	{
		Copium::Files::FileSystem* fs = Copium::Files::FileSystem::Instance();
		fs->get_filepath_in_directory(Copium::Files::Paths::assetPath.c_str());
	}

	void AssetsSystem::update()
	{

	}

	void AssetsSystem::exit()
	{

	}
}