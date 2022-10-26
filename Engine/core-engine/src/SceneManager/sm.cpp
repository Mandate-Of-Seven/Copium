/*!***************************************************************************************
****
\file			sm.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	Contains definitions for the NewSceneManager class.
	NewSceneManager is responsible for managing the current scene in the engine.
	It is also in charge of:
	1. loading in scene data from a file
	2. switching to another scene
	3. de-allocation of resources used by current scene (cleanup before engine close)
	4. Calling scene's update functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include <pch.h>
#include "SceneManager/sm.h"
#include "../Graphics/graphics-system.h"
#include "../Windows/windows-system.h"

namespace Copium {

	NewSceneManager::NewSceneManager() : gof{nullptr}, currentScene{nullptr}, selectedGameObject{nullptr}
	{
		gof = new GameObjectFactory();
		if (!gof)
		{
			std::cout << "Error allocating memory for GameObjectFactory\n";
		}	
		std::cout << "sm ctor\n";
	}

	NewSceneManager::~NewSceneManager()
	{
		std::cout << selectedGameObject << std::endl;
		selectedGameObject = nullptr;

		if (gof)
		{
			delete gof;
			gof = nullptr;
		}
		

		if (currentScene)
		{
			delete currentScene;
			currentScene = nullptr;
		}


		//std::cout << "new scene manager destruction called\n";

	}

	void NewSceneManager::init()
	{
		// Load default scene

		// Debug Purposes
		std::string str("Data\\sandbox.json");
		load_scene(str);
		std::cout << "No. of GameObjects in scene:" << currentScene->get_gameobjcount() << std::endl;
		//currentScene->get_gameobjectvector()[0]->Trans().Position();

		for (int i = 0; i < currentScene->get_gameobjcount(); i++)
		{
			Copium::Graphics::SpriteRenderer * sprite = new Copium::Graphics::SpriteRenderer;
			TransformComponent* tmp = currentScene->get_gameobjectvector()[i]->Trans();
			if (!tmp)
				continue;

			glm::vec3 pos = tmp->get_transform().glmPosition();
			sprite->set_position(pos);

			PRINT("Coords: " << pos.x << ", " << pos.y);

			glm::vec3 size = tmp->get_transform().glmScale();
			sprite->set_size(glm::vec2(size.x, size.y));

			PRINT("Size: " << size.x << ", " << size.y);

			glm::vec4 color = { 1.f, 1.f ,1.f ,1.f };
			sprite->set_color(color);

			Copium::Graphics::GraphicsSystem::Instance()->add_sprite(sprite);
		}


	}
	void NewSceneManager::update()
	{

		// call current scene's update functions
	}
	void NewSceneManager::exit()
	{
	}

	bool NewSceneManager::load_scene(const std::string& _filepath)
	{
		std::cout << "load_scene\n";

		if (!currentScene)
		{
			currentScene = new NormalScene(_filepath);
		}

		gof->link_to_scene(currentScene);
			
		std::ifstream ifs(_filepath);
		if (!ifs)
			return false;

		rapidjson::IStreamWrapper isw(ifs);
		if (document.ParseStream(isw).HasParseError())
		{
			ifs.close();
			return false;
		}


		if (document.HasMember("Name"))
		{
			currentScene->set_name(document["Name"].GetString());
		}

		if (document.HasMember("GameObjects"))
		{
			rapidjson::Value& _gameObjArr = document["GameObjects"].GetArray();
			for (rapidjson::Value::ValueIterator iter = _gameObjArr.Begin(); iter != _gameObjArr.End(); ++iter)
			{
				GameObject* tmpGO = nullptr;
				tmpGO = gof->build_gameobject(*iter);
			}
		}
		
		ifs.close();
		return true;

	}
	bool NewSceneManager::change_scene(const std::string& _newfilepath)
	{
		bool result;

		// No scene loaded, therefore cannot change
		if (!currentScene)
		{
			result = load_scene(_newfilepath);
			return result;
		}


		// offer the option to save before exiting


		// Clear game objects in scene
		// Deserialize from new file and overwrite other scene data
		
		if (std::filesystem::exists(_newfilepath))
		{
			result = load_scene(_newfilepath);
			delete currentScene;
		}
		else
		{
			std::cout << "file does not exist, scene change aborted\n";
			return false;
		}

		return result;

	}

	GameObjectFactory& NewSceneManager::get_gof()
	{
		return *gof;
	}

	Scene* NewSceneManager::get_current_scene()
	{
		return currentScene;
	}
	void NewSceneManager::set_selected_gameobject(GameObject* _go) { selectedGameObject = _go; }
	GameObject* NewSceneManager::get_selected_gameobject() { return selectedGameObject; }

}
