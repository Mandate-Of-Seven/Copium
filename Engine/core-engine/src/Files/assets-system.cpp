/*!***************************************************************************************
\file			assets-system.cpp
\project
\author			Sean Ngo
				Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			20/10/2022

\brief
	This file contains functions for the Assets system where the user can load and unload
	assets that are retrieved from the File system into the engine.

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

	}
	
	void AssetsSystem::exit()
	{
		for (Texture& texture : textures)
			texture.exit();
	}

	void AssetsSystem::load_file(File* _file)
	{
		FILE_TYPE type = _file->get_file_type().fileType;

		switch (type)
		{
		case FOLDER:
			break;

		case AUDIO:
			break;

		case FONT:
			break;

		case SCENE:
			break;

		case SCRIPT:
			break;

		case SHADER:
			break;

		case SPRITE:
			load_texture(_file);
			break;

		case TEXT:
			break;
		}
	}

	void AssetsSystem::unload_file(File* _file)
	{
		FILE_TYPE type = _file->get_file_type().fileType;

		switch (type)
		{
		case FOLDER:
			break;

		case AUDIO:
			break;

		case FONT:
			break;

		case SCENE:
			break;

		case SCRIPT:
			break;

		case SHADER:
			break;

		case SPRITE:
			unload_texture(_file);
			break;

		case TEXT:
			break;
		}
	}

	void AssetsSystem::load_assets(Directory* _directory)
	{
		(void) _directory;

		// Load Textures (.png)
		load_all_textures(fs->get_file_references()[SPRITE]);

		// Load Shaders (.vert & .frag)
		load_all_shaders(fs->get_filepath_in_directory(Paths::dataPath.c_str(), ".vert", ".frag"));

		// Load Audio (.wav)
		load_all_audio(fs->get_filepath_in_directory(Paths::assetPath.c_str(), ".wav"));
	}

	void AssetsSystem::load_all_textures(std::list<File*>& _files)
	{
		for (File* file : _files)
		{
			// Store the texture
			load_texture(file);
		}

		// Bean: This should be done in the animation system or animator component
		//for (int i = 0; i < get_textures().size(); i++)
		//{

		//	//if (!get_texture(i)->get_file_path().compare("../PackedTracks/Assets/Textures/TrackSpriteSheet.png"))
		//	//{
		//	//	Spritesheet ss(get_textures()[i], 1, 8);
		//	//	spritesheets.push_back(ss);
		//	//}
		//	//else if (!get_texture(i)->get_file_path().compare("../PackedTracks/Assets/Textures/mock-up.png"))
		//	//{
		//	//	Spritesheet ss(get_textures()[i], 4, 3);
		//	//	spritesheets.push_back(ss);
		//	//}


		//}
	}

	void AssetsSystem::load_texture(File* _file)
	{
		Texture texture(_file->generic_string());
		texture.set_id(_file->get_id());
		textures.push_back(texture);
	}

	void AssetsSystem::unload_texture(File* _file)
	{
		for (auto it = textures.begin(); it != textures.end();)
		{
			if (_file->get_id() == (*it).get_id())
			{
				(*it).exit();
				it = textures.erase(it);
				break;
			}
			else
				++it;
		}

		textures.shrink_to_fit();
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

	void load_audio(File* _file)
	{
		if (_file->extension() ==".wav")
		{
			//std::cout << _file->get_name() << " "<<_file->filename();
			std::string temp = _file->filename().string();
			size_t lastDot = temp.find_last_of(".");
			SoundSystem::Instance()->CreateSound(_file->filename().string(), temp.substr(0,lastDot));
			SoundSystem::Instance()->SetVolume(temp.substr(0, lastDot), 1.0f);
		}
		else
		{
			std::cout << _file->get_name() << " is not a .wav file!\n";
		}
	}
	void unload_audio(File* _file)
	{
		if (_file->extension() == ".wav")
		{
			std::string temp = _file->filename().string();
			size_t lastDot = temp.find_last_of(".");
			std::string targetAlias = temp.substr(0, lastDot);
			for (auto iter = SoundSystem::Instance()->soundList.begin(); iter != SoundSystem::Instance()->soundList.end();iter++)
			{
				if ((*iter).first == targetAlias)
				{
					SoundSystem::Instance()->soundList.erase(iter);
					std::cout << "Unloading audio file: " << targetAlias << " from sound list\n";
					break;
				}
			}
		}
		else
		{
			std::cout << _file->get_name() << " is not a .wav file!\n";
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