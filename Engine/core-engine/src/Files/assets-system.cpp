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

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

//#include <GLFW/glfw3.h>

#include "Files/assets-system.h"
#include "Files/file-system.h"
#include "Audio/sound-system.h"
#include "Graphics/graphics-system.h"
#include "Events/events-system.h"

namespace Copium
{
	void AssetsSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		
		MyEventSystem->subscribe(this, &AssetsSystem::CallbackFileAsset);

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
		LoadAssets(&MyFileSystem.get_asset_directory());
		PRINT("ASSETS LOADED");

		LoadExistingMetaFile();
		PRINT("META LOADED");

		// Generate Meta Files for all assets
		/*for (int i = 0; i < (int)FILE_TYPE::NUM_TYPES; i++)
		{
			if ((FILE_TYPE) i != FILE_TYPE::SPRITE)
				continue;

			std::list<File*> files = MyFileSystem.get_file_references()[FILE_TYPE::SPRITE];
			for (File* file : files)
				GenerateMetaFile(file);
		}*/

		std::list<File*> files = MyFileSystem.get_file_references()[FILE_TYPE::SPRITE];
		for (File* file : files)
			GenerateMetaFile(file);
	}

	void AssetsSystem::update()
	{

	}
	
	void AssetsSystem::exit()
	{
		for (Texture& texture : textures)
			texture.exit();
	}

	void AssetsSystem::LoadFile(File* _file)
	{
		FILE_TYPE type = _file->get_file_type().fileType;

		switch (type)
		{
		case FILE_TYPE::FOLDER:
			break;

		case FILE_TYPE::AUDIO:
			break;

		case FILE_TYPE::FONT:
			break;

		case FILE_TYPE::SCENE:
			break;

		case FILE_TYPE::SCRIPT:
			break;

		case FILE_TYPE::SHADER:
			break;

		case FILE_TYPE::SPRITE:
			LoadTexture(_file);
			break;

		case FILE_TYPE::TEXT:
			break;

		case FILE_TYPE::ASSET:
			break;
		}
	}

	void AssetsSystem::UnloadFile(File* _file)
	{
		FILE_TYPE type = _file->get_file_type().fileType;

		switch (type)
		{
		case FILE_TYPE::FOLDER:
			break;

		case FILE_TYPE::AUDIO:
			break;

		case FILE_TYPE::FONT:
			break;

		case FILE_TYPE::SCENE:
			break;

		case FILE_TYPE::SCRIPT:
			break;

		case FILE_TYPE::SHADER:
			break;

		case FILE_TYPE::SPRITE:
			UnloadTexture(_file);
			break;

		case FILE_TYPE::TEXT:
			break;

		case FILE_TYPE::ASSET:
			break;
		}
	}

	void AssetsSystem::LoadAssets(Directory* _directory)
	{
		(void) _directory;
		PRINT("LOADING TEXTURES");
		// Load Textures (.png)
		LoadAllTextures(MyFileSystem.get_file_references()[FILE_TYPE::SPRITE]);

		PRINT("LOADING SHADERS");
		// Load Shaders (.vert & .frag)
		LoadAllShaders(MyFileSystem.get_filepath_in_directory(Paths::dataPath.c_str(), ".vert", ".frag"));

		PRINT("LOADING AUDIO");
		// Load Audio (.wav)
		LoadAllAudio(MyFileSystem.get_filepath_in_directory(Paths::assetPath.c_str(), ".wav"));
	}

	void AssetsSystem::LoadAllTextures(std::list<File*>& _files)
	{
		for (File* file : _files)
		{
			// Store the texture
			LoadTexture(file);
		}
	}

	void AssetsSystem::LoadTexture(File* _file)
	{
		PRINT("LOADING TEXTURE: " << _file->filePath.string());
		Texture texture(_file->filePath.string());
		texture.set_id(_file->get_id());
		textures.push_back(texture);
	}

	void AssetsSystem::UnloadTexture(File* _file)
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

	void AssetsSystem::LoadAllAudio(std::list<std::string>& _path)
	{
		for (std::string path : _path)
		{
			//std::cout << "Audio: " << path << "\n";
			size_t lastSlash = path.find_last_of("/");
			std::string temp = path.substr(lastSlash + 1);
			size_t lastDot = temp.find_last_of(".");
			//std::cout << "Alias: " << temp.substr(0, lastDot) << "\n";
			MySoundSystem.CreateSound(path, temp.substr(0, lastDot));
			MySoundSystem.SetVolume(temp.substr(0, lastDot), 1.0f);
		}
	}

	void load_audio(File* _file)
	{
		if (_file->filePath.extension() ==".wav")
		{
			//std::cout << _file->name << " "<<_file->filename();
			std::string temp = _file->filePath.filename().string();
			size_t lastDot = temp.find_last_of(".");
			MySoundSystem.CreateSound(_file->filename().string(), temp.substr(0,lastDot));
			MySoundSystem.SetVolume(temp.substr(0, lastDot), 1.0f);
		}
		else
		{
			std::cout << _file->get_name() << " is not a .wav file!\n";
		}
	}
	void unload_audio(File* _file)
	{
		if (_file->filePath.extension() == ".wav")
		{
			std::string temp = _file->filePath.filename().string();
			size_t lastDot = temp.find_last_of(".");
			std::string targetAlias = temp.substr(0, lastDot);
			for (auto iter = MySoundSystem.soundList.begin(); iter != MySoundSystem.soundList.end();iter++)
			{
				if ((*iter).first == targetAlias)
				{
					MySoundSystem.soundList.erase(iter);
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

	void AssetsSystem::LoadAllShaders(std::list<std::string>& _path)
	{
		for (auto it = _path.begin(); it != _path.end(); it++)
		{
			// Bean: This is slightly hardcored because it depends on the file arrangement
			std::string fragShader = it->c_str();
			std::string vtxShader = (++it)->c_str();
			MyGraphicsSystem.SetupShaderProgram(vtxShader, fragShader);
		}
	}

	void AssetsSystem::CreateAsset(const std::string& _directory, const std::string& _name)
	{
		std::filesystem::path currentDir = _directory + "\\";
		std::filesystem::path pathName = currentDir.string() + _name + ".asset";

		std::fstream createAsset;

		File* temp = MyFileSystem.get_file(pathName);
		int counter = 1;
		if (temp != nullptr)
		{
			std::filesystem::path editedPath;
			while (temp != nullptr)
			{
				editedPath = currentDir.string() + pathName.stem().string();
				editedPath += " " + std::to_string(counter++) + pathName.extension().string();
				temp = MyFileSystem.get_file(editedPath);
			}
			createAsset.open(editedPath.string(), std::ios::out);
		}
		else
			createAsset.open(pathName.string(), std::ios::out);

		if (!createAsset)
			PRINT("Error in creating file");

		createAsset.close();
	}

	void AssetsSystem::CopyAsset(const File& _file, const std::string& _ext)
	{
		MyFileSystem.copy_file(_file, _ext);
	}

	void AssetsSystem::LoadExistingMetaFile()
	{
		std::list<File*> metaFiles = MyFileSystem.get_file_references()[FILE_TYPE::META];

		for (File* metaFile : metaFiles)
		{
			std::ifstream readMetaFile(metaFile->filePath.string());
			
			std::string str, filePath, assetImporter;
			uint64_t uuid;

			// File path
			readMetaFile >> str >> str >> filePath;

			// UUID
			readMetaFile >> str >> uuid;

			// Asset Importer
			readMetaFile >> assetImporter;
			assetImporter.erase(assetImporter.end() - 1);

			MetaID id{ uuid,filePath,reinterpret_cast<void*>(assetImporter.data()) };

			/*PRINT("MetaID: ");
			PRINT("  File Path: " << id.filePath);
			PRINT("  UUID     : " << id.uuid);
			PRINT("  Importer : " << reinterpret_cast<const char*>(id.assetImporter));*/

			uint64_t pathID = std::hash<std::string>{}(id.filePath);
			metaData.emplace(std::make_pair(pathID, id));
		}
	}

	void AssetsSystem::GenerateMetaFile(File* _file)
	{
		// Check that file has a UUID
		if (CheckForMetaFile(_file))
			return;

		// Generate UUID
		MetaID id;

		uint64_t pathID = std::hash<std::string>{}(_file->filePath.string());
		metaData.emplace(std::make_pair(pathID, id));

		std::ofstream writeMetaFile(_file->filePath.string() + ".meta");

		GenerateFileStream(writeMetaFile, _file);

		//PRINT("Meta file successfully generated...");

		writeMetaFile.close();
	}

	bool AssetsSystem::CheckForMetaFile(File* _file)
	{
		uint64_t pathID = std::hash<std::string>{}(_file->filePath.string());
		if (metaData.find(pathID) != metaData.end())
		{
			//PRINT("This file already contains a meta file!");
			return true;
		}

		return false;
	}

	void AssetsSystem::GenerateFileStream(std::ofstream& _outputFile, File* _file)
	{
		// Default meta headers
		uint64_t pathID = std::hash<std::string>{}(_file->filePath.string());
		metaData[pathID].filePath = _file->filePath.string();
		_outputFile << "File Path: " << metaData[pathID].filePath << "\n";
		_outputFile << "uuid: " << metaData[pathID].uuid << "\n";

		// Hidden meta headers
		std::string str = _file->get_file_type().stringType; // Check Format Importer
		metaData[pathID].assetImporter = reinterpret_cast<void*>(str.data());

		// Importer type body
		switch (GetImporterFile(_file->get_file_type()))
		{
		case IMPORTER_TYPE::DEFAULT:
			_outputFile << "DefaultImporter:\n";
			break;
		case IMPORTER_TYPE::AUDIO:
			_outputFile << "AudioImporter:\n";
			break;
		case IMPORTER_TYPE::MONO:
			_outputFile << "MonoImporter:\n";
			break;
		case IMPORTER_TYPE::NATIVE:
			_outputFile << "NativeImporter:\n";
			break;
		case IMPORTER_TYPE::PREFAB:
			_outputFile << "PrefabImporter:\n";
			break;
		case IMPORTER_TYPE::SHADER:
			_outputFile << "ShaderImporter:\n";
			break;
		case IMPORTER_TYPE::TEXTURE:
			_outputFile << "TextureImporter:\n";
			break;
		}
	}

	IMPORTER_TYPE AssetsSystem::GetImporterFile(const FileType& _type)
	{
		switch (_type.fileType)
		{
		case FILE_TYPE::FOLDER:
		case FILE_TYPE::SCENE:
			return IMPORTER_TYPE::DEFAULT;
		case FILE_TYPE::AUDIO:
			return IMPORTER_TYPE::AUDIO;
		case FILE_TYPE::ASSET:
			return IMPORTER_TYPE::NATIVE;
		case FILE_TYPE::PREFAB:
			return IMPORTER_TYPE::PREFAB;
		case FILE_TYPE::SHADER:
			return IMPORTER_TYPE::SHADER;
		case FILE_TYPE::SPRITE:
			return IMPORTER_TYPE::TEXTURE;
		case FILE_TYPE::SCRIPT:
			return IMPORTER_TYPE::MONO;
		}

		return IMPORTER_TYPE::DEFAULT;
	}

	void AssetsSystem::CallbackFileAsset(FileAssetEvent* pEvent)
	{
		switch (pEvent->type)
		{
		case 0: // Generate Meta File
			GenerateMetaFile(pEvent->file);
			break;
		case 1: // Load Asset File
			LoadFile(pEvent->file);
			break;
		case 2: // Unload Asset File
			UnloadFile(pEvent->file);
			break;
		}

	}

}