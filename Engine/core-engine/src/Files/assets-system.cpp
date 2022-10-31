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
#include "Audio/sound-system.h"

namespace Copium
{
	void AssetsSystem::init()
	{
		FileSystem* fs = FileSystem::Instance();
		load_all_textures(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".png"));
		load_all_audio(fs->get_filepath_in_directory((Paths::assetPath+"/sounds").c_str(), ".wav"));
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
			Texture texture(path);

			// Store the texture
			textures.push_back(texture);
		}

		Spritesheet ss(get_textures()[10], 1, 8);
		spritesheets.push_back(ss);
	}

	void AssetsSystem::load_all_audio(std::list<std::string>& _path)
	{
		for (std::string path : _path)
		{
			std::cout << "Texture: " << path << "\n";
			size_t lastSlash = path.find_last_of("/");
			std::string temp = path.substr(lastSlash + 1);
			size_t lastDot = temp.find_last_of(".");
			std::cout << "Alias: " << temp.substr(0, lastDot) << "\n";
			SoundSystem::Instance()->CreateSound(path, temp.substr(0, lastDot));
		}
	}
}