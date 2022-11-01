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

		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		FileSystem* fs = FileSystem::Instance();
		
		// Bean: In the future, we shall store all extension files in their own extension vectors std::map<std::string, std::vector<std::string>>
		// And update when the user adds more files into the system during runtime

		// Load Textures (.png)
		load_all_textures(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".png"));
		
		// Load Shaders (.vert & .frag)
		load_all_shaders(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".vert", ".frag"));
		
		// Load Audio (.wav)
		load_all_audio(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".wav"));
		
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
			//std::cout << "Audio: " << path << "\n";
			size_t lastSlash = path.find_last_of("/");
			std::string temp = path.substr(lastSlash + 1);
			size_t lastDot = temp.find_last_of(".");
			//std::cout << "Alias: " << temp.substr(0, lastDot) << "\n";
			SoundSystem::Instance()->CreateSound(path, temp.substr(0, lastDot));
		}
	}

	void AssetsSystem::load_all_shaders(std::list<std::string>& _path)
	{
		for (auto it = _path.begin(); it != _path.end(); it++)
		{
			// Bean: This is slightly hardcored because it depends on the file arrangement
			std::string fragShader = it->c_str();
			std::string vtxShader = (++it)->c_str();
			GraphicsSystem::Instance()->setup_shader_program(vtxShader, fragShader);
		}
	}
}