/*!***************************************************************************************
\file			assets-system.h
\project
\author			Sean Ngo
				Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			20/10/2022

\brief
	Contains function declarations for Assets system which allows for laoding and unloading
	of assets from the File system.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef ASSETS_SYSTEM_H
#define ASSETS_SYSTEM_H

#include <map>
#include "CopiumCore/system-interface.h"
#include "Graphics/textures.h"
#include "Graphics/spritesheet.h"
#include "Animation/animation-struct.h"

namespace Copium
{
	class File;
	class Directory;

	CLASS_SYSTEM(AssetsSystem)
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Initializes all the assets in the engine to be used by the users
		*/
		/**************************************************************************/
		void init();

		/***************************************************************************/
		/*!
		\brief
			Updates the assets in the engine in real time
		*/
		/**************************************************************************/
		void update();

		/***************************************************************************/
		/*!
		\brief
			Exits the asset system
		*/
		/**************************************************************************/
		void exit();

		/***************************************************************************/
		/*!
		\brief
			Loads a file into the asset system
		\param _file
			The file to load
		*/
		/**************************************************************************/
		void load_file(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Unloads a file from the asset system
		\param _file
			The file to unload
		*/
		/**************************************************************************/
		void unload_file(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Loads all assets path in the assets folder
		*/
		/**************************************************************************/
		void load_assets(Directory* _directory);

		/***************************************************************************/
		/*!
		\brief
			Loads all textures into the engine
		\param _path
			The file path to load from
		*/
		/**************************************************************************/
		void load_all_textures(std::list<File*>& _files);

		/***************************************************************************/
		/*!
		\brief
			Loads a single texture from a file path
		\param _fie
			The file path to load from
		*/
		/**************************************************************************/
		void load_texture(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Unloads a single texture from a file path
		\param _fie
			The file path to unload from
		*/
		/**************************************************************************/
		void unload_texture(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Loads all audio into the engine
		\param _path
			The file path to load from
		*/
		/**************************************************************************/
		void load_all_audio(std::list<std::string>& _path);

		void load_audio(File * _file);
		void unload_audio(File * _file);

		/***************************************************************************/
		/*!
		\brief
			Loads all shaders into the engine
		\param _path
			The file path to load from
		*/
		/**************************************************************************/
		void load_all_shaders(std::list<std::string>& _path);

		const std::vector<Texture>& get_textures() { return textures; }
		Texture* get_texture(unsigned int const& _index)
		{
			if (_index >= textures.size())
				return nullptr;

			//COPIUM_ASSERT(_index >= textures.size(), "Index is out of bound!");
			return &textures[_index];
		}

		const std::vector<Spritesheet>& get_spritesheets() { return spritesheets; }
		Spritesheet* get_spritesheet(unsigned int const& _index)
		{
			if (_index >= spritesheets.size())
				return nullptr;

			//COPIUM_ASSERT(_index >= spritesheets.size(), "Index is out of bound!");
			return &spritesheets[_index];
		}

		void CreateAsset(const std::string& _directory, const std::string& _name);

		void CopyAsset(const File& _file, const std::string& _ext);

	private:
		/* Assets Data ******************************************************************/
		std::vector<Texture> textures;
		std::vector<Spritesheet> spritesheets;
		std::vector<Animation> animations; 
	};
}

#endif // !ASSETS_SYSTEM_H
