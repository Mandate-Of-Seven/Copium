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
#include "Events/events.h"
#include "Files/file.h"

#define MyAssetSystem (*Copium::AssetsSystem::Instance())

namespace Copium
{
	class Directory;

	enum class IMPORTER_TYPE
	{
		DEFAULT,// Folder
		AUDIO,	// Audio
		MONO,	// Script
		NATIVE,	// Asset
		PREFAB,	// Prefab
		SHADER,	// Shader
		TEXTURE,// Texture

		NUM_TYPES
	};

	struct MetaID
	{
		UUID uuid;
		std::string filePath;
		void* assetImporter;
	};

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
		void LoadFile(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Unloads a file from the asset system
		\param _file
			The file to unload
		*/
		/**************************************************************************/
		void UnloadFile(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Loads all assets path in the assets folder
		*/
		/**************************************************************************/
		void LoadAssets(Directory* _directory);

		/***************************************************************************/
		/*!
		\brief
			Loads all textures into the engine
		\param _path
			The file path to load from
		*/
		/**************************************************************************/
		void LoadAllTextures(std::list<File*>& _files);

		/***************************************************************************/
		/*!
		\brief
			Loads a single texture from a file path
		\param _fie
			The file path to load from
		*/
		/**************************************************************************/
		void LoadTexture(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Unloads a single texture from a file path
		\param _fie
			The file path to unload from
		*/
		/**************************************************************************/
		void UnloadTexture(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Loads all audio into the engine
		\param _path
			The file path to load from
		*/
		/**************************************************************************/
		void LoadAllAudio(std::list<std::string>& _path);

		void LoadAudio(File * _file);
		void UnloadAudio(File * _file);

		/***************************************************************************/
		/*!
		\brief
			Loads all shaders into the engine
		\param _path
			The file path to load from
		*/
		/**************************************************************************/
		void LoadAllShaders(std::list<std::string>& _path);

		const std::vector<Texture>& GetTextures() { return textures; }
		Texture* GetTexture(unsigned int const& _index)
		{
			if (_index >= textures.size())
				return nullptr;

			//COPIUM_ASSERT(_index >= textures.size(), "Index is out of bound!");
			return &textures[_index];
		}

		const std::vector<Spritesheet>& GetSpritesheets() { return spritesheets; }
		Spritesheet* GetSpritesheet(unsigned int const& _index)
		{
			if (_index >= spritesheets.size())
				return nullptr;

			//COPIUM_ASSERT(_index >= spritesheets.size(), "Index is out of bound!");
			return &spritesheets[_index];
		}

		/***************************************************************************/
		/*!
		\brief
			Creates an asset in the assets's folder
		\param _directory
			The directory in the asset folder to create in
		\param _name
			The name of the asset
		*/
		/***************************************************************************/
		void CreateAsset(const std::string& _directory, const std::string& _name);

		/***************************************************************************/
		/*!
		\brief
			Copies an asset from another and place it in the same directory
		\param _file
			The file to copy from
		\param _ext
			The extension to change to
		*/
		/***************************************************************************/
		void CopyAsset(const File& _file, const std::string& _ext);

		const MetaID& GetMetaID(const uint64_t& _id) { return metaData[_id]; }

		// Meta Data Files
		/***************************************************************************/
		/*!
		\brief
			Loads exisiting meta files from the engine and stores it in the metaData
			data member
		*/
		/***************************************************************************/
		void LoadExistingMetaFile();

		/***************************************************************************/
		/*!
		\brief
			Generates a meta ID and passes the values to GenerateFileStream
		\param _file
			The file to generate a meta file for
		*/
		/***************************************************************************/
		void GenerateMetaFile(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Checks if a meta file for this specific file already exists
		\param _file
			The file to check
		\return
			True if it exists, else false
		*/
		/***************************************************************************/
		bool CheckForMetaFile(File* _file);

		/***************************************************************************/
		/*!
		\brief
			Generates a meta file for the specific file
		\param _outputFile
			The output file to write data into
		\param _file
			The file to generate a meta file for
		*/
		/***************************************************************************/
		void GenerateFileStream(std::ofstream& _outputFile, File* _file);

		/***************************************************************************/
		/*!
		\brief
			Gets the importer file type depending on the type of the file
		\param _type
			The file type
		\return
			The specific importer type for the meta file
		*/
		/***************************************************************************/
		IMPORTER_TYPE GetImporterFile(const FileType& _type);

	private:

		void CallbackFileAsset(FileAssetEvent* pEvent);

	private:
		/* Assets Data ******************************************************************/
		std::vector<Texture> textures;
		std::vector<Spritesheet> spritesheets;
		std::vector<Animation> animations;
		std::unordered_map<uint64_t, MetaID> metaData;
	};
}

#endif // !ASSETS_SYSTEM_H
