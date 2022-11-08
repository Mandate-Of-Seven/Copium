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

#include <GLFW/glfw3.h>

#include "Files/assets-system.h"
#include "Files/file-system.h"
#include "Graphics/graphics-system.h"
#include "Audio/sound-system.h"

namespace Copium
{
	void AssetsSystem::init()
	{
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_RELOAD_ASSETS, this);

		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		FileSystem* fs = FileSystem::Instance();
		
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
		load_assets();

		// Load Textures (.png)
		load_all_textures(assetFilePath[".png"]);

		//load_all_textures(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".png"));
		
		// Load Shaders (.vert & .frag)
		load_all_shaders(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".vert", ".frag"));
		
		// Load Audio (.wav)
		load_all_audio(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".wav"));
	}

	void AssetsSystem::update()
	{
		static int previousFileCount = 0;

		int fileCount = 0;
		for (auto dirEntry : std::filesystem::recursive_directory_iterator(Paths::assetPath))
		{
			(void)dirEntry;
			fileCount++;
		}

		// Check if there is a change in the number of files
		if (previousFileCount != 0 && previousFileCount != fileCount)
			reload_assets();

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

	void AssetsSystem::load_assets()
	{
		for (auto& dirEntry : std::filesystem::recursive_directory_iterator(Paths::assetPath))
		{
			if (!dirEntry.is_directory()) // If it is a file
			{
				// Check if the file extension exists
				std::string fileExt = dirEntry.path().extension().string();
				if (assetFilePath.contains(fileExt))
				{
					// Add file path into the map
					assetFilePath[fileExt].push_back(dirEntry.path().generic_string());
				}
				else
				{
					// Create new key
					assetFilePath.emplace(std::make_pair(fileExt, std::list<std::string>()));
					assetFilePath[fileExt].push_back(dirEntry.path().generic_string());
				}
			}
		}

		/*for (auto str : assetFilePath)
		{
			PRINT("Extension: " << str.first);
			for (auto str2 : str.second)
			{
				PRINT("  Path: " << str2);
			}
			PRINT("");
		}*/
	}

	void AssetsSystem::reload_assets()
	{
		assetFilePath.clear();

		load_assets();

		reload_textures(assetFilePath[".png"]);
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

		// Bean: This should be done in the animation system or animator component
		for (int i = 0; i < get_textures().size(); i++)
		{

			if (!get_textures()[i].get_file_path().compare("Assets/textures/TrackSpriteSheet.png"))
			{
				Spritesheet ss(get_textures()[i], 1, 8);
				spritesheets.push_back(ss);
			}
			else if (!get_textures()[i].get_file_path().compare("Assets/textures/mock-up.png"))
			{
				Spritesheet ss(get_textures()[i], 4, 3);
				spritesheets.push_back(ss);
			}
		}
	}

	void AssetsSystem::reload_textures(std::list<std::string>& _path)
	{
		double start = glfwGetTime();
		for (std::string path : _path)
		{
			// Only add new textures
			bool isNewTexture = true;
			for (auto texture : textures)
			{
				if (!texture.get_file_path().compare(path))
				{
					isNewTexture = false;
					break;
				}
			}

			// Generate texture
			if (isNewTexture)
			{
				// Bean: STBI load is intensive for some reasons, probably because the file
				//		 dimension is above 2k or file size too huge
				Texture newTexture(path);
				//std::cout << "Texture: " << path << "\n";
				
				// Store the texture
				textures.push_back(newTexture);
				isNewTexture = false;
			}			
		}

		for (auto it = textures.begin(); it != textures.end();)
		{
			bool hasTexture = false;
			for (std::string path : _path)
			{
				// Check if texture still exists in the assets folder
				if (!((*it).get_file_path().compare(path)))
				{
					hasTexture = true;
					break;
				}
			}

			// If it does not exist in the assets folder, delete it
			if (!hasTexture)
				it = textures.erase(it);
			else
				it++;
		}

		double end = glfwGetTime();

		PRINT("Time taken to reload: " << end - start);
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