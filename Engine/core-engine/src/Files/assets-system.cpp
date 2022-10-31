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
#include "Graphics/graphics-system.h"

namespace Copium
{
	void AssetsSystem::init()
	{
		FileSystem* fs = FileSystem::Instance();
		load_all_textures(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".png"));

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//std::cout << "Texture: " << textures[i].get_object_id() << " " << textures[i].get_file_path() << "\n";
		}

	}

	void AssetsSystem::update()
	{

	}

	void AssetsSystem::exit()
	{

	}

	void AssetsSystem::load_all_textures(std::list<std::string>& _path)
	{
		for (std::string path : _path)
		{
			//std::cout << "Texture: " << path << "\n";
			// Generate texture
			Copium::Texture texture(path);

			// Store the texture
			textures.push_back(texture);
		}

		Copium::Graphics::Spritesheet ss(get_textures()[10], 1, 8);
		spritesheets.push_back(ss);
	}
}