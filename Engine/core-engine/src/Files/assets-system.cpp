/*!***************************************************************************************
\file			assets-system.cpp
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

//#include <GLFW/glfw3.h>

#include "Files/assets-system.h"
#include "Files/file-system.h"
#include "Graphics/graphics-system.h"
#include "Audio/sound-system.h"

namespace Copium
{
	namespace
	{
		FileSystem* fs = FileSystem::Instance();
	}

	void AssetsSystem::init()
	{
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_RELOAD_ASSETS, this);

		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		
		// Bean: In the future, we shall store all extension files in their own extension vectors std::map<std::string, std::vector<std::string>>
		// And update when the user adds more files into the system during runtime
		// Assets should have their own file types for easy access and setting up of image icons etc
		// Can be a struct or a class
		// All files should have:
		// 1. A unique ID
		// 2. A file path
		// 3. The extension
		//
		// Load all file paths in the asset folder
		load_assets(&fs->get_asset_directory());

	}

	void AssetsSystem::update()
	{
		static int previousFileCount = 0;

		int fileCount = 0;
		for (auto dirEntry : std::filesystem::recursive_directory_iterator(Paths::assetPath))
		{
			(void) dirEntry;
			fileCount++;
		}

		// Check if there is a change in the number of files
		if (previousFileCount != 0 && previousFileCount != fileCount)
		{
			MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_RELOAD_ASSETS);
		}

		previousFileCount = fileCount;
	}
	
	void AssetsSystem::exit()
	{
		for (Texture& texture : textures)
			texture.exit();
	}

	void AssetsSystem::handleMessage(MESSAGE_TYPE mType)
	{
		if (mType == MESSAGE_TYPE::MT_RELOAD_ASSETS)
		{
			reload_assets();
		}
	}

	void AssetsSystem::load_assets(Directory* _directory)
	{
		(void) _directory;

		// Load Textures (.png)
		load_all_textures(fs->get_file_references()[SPRITE]);

		// Load Shaders (.vert & .frag)
		load_all_shaders(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".vert", ".frag"));

		// Load Audio (.wav)
		load_all_audio(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".wav"));
	}

	void AssetsSystem::reload_assets()
	{
		reload_textures();
	}

	void AssetsSystem::load_all_textures(std::list<File*>& _files)
	{
		for (File* file : _files)
		{
			// Store the texture
			textures.push_back(Texture(file->generic_string()));
		}

		// Bean: This should be done in the animation system or animator component
		for (int i = 0; i < get_textures().size(); i++)
		{

			if (!get_texture(i)->get_file_path().compare("../PackedTracks/Assets/Textures/TrackSpriteSheet.png"))
			{
				Spritesheet ss(get_textures()[i], 1, 8);
				spritesheets.push_back(ss);
			}
			else if (!get_texture(i)->get_file_path().compare("../PackedTracks/Assets/Textures/mock-up.png"))
			{
				Spritesheet ss(get_textures()[i], 4, 3);
				spritesheets.push_back(ss);
			}
		}
	}

	void AssetsSystem::reload_textures()
	{
		//double start = glfwGetTime();
		for (File* file : fs->get_file_references()[SPRITE])
		{
			// Only add new textures
			bool isNewTexture = true;
			for (auto texture : textures)
			{
				if (!texture.get_file_path().compare(file->generic_string()))
				{
					isNewTexture = false;
					break;
				}
			}

			// Generate texture
			if (isNewTexture)
			{
				textures.push_back(Texture(file->generic_string()));
				isNewTexture = false;
			}			
		}

		for (size_t i = 0; i < textures.size(); i++)
		{
			bool hasTexture = false;
			for (File* file : fs->get_file_references()[SPRITE])
			{
				// Check if texture still exists in the assets folder
				if (!textures[i].get_file_path().compare(file->generic_string()))
				{
					hasTexture = true;
					break;
				}
			}

			// If it does not exist in the assets folder, delete it
			if (!hasTexture)
			{
				textures.erase(textures.begin() + i);
				textures.shrink_to_fit();
			}
		}

		//double end = glfwGetTime();
		//PRINT("Assets time taken to reload: " << end - start);
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