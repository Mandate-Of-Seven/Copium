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

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include <pch.h>
#include "SceneManager/sm.h"
#include "../Graphics/graphics-system.h"
#include "../Windows/windows-system.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

namespace Copium {
	GameObject* NewSceneManager::findGameObjByID(GameObjectID _ID)
	{
		for (GameObject* pGameObj : currentScene->get_gameobjectvector())
		{
			if (pGameObj->id == _ID)
			{
				return pGameObj;
			}
		}
		return nullptr;
	}

	NewSceneManager::NewSceneManager() : gof{nullptr}, currentScene{nullptr}, selectedGameObject{nullptr}
	{
		gof = new GameObjectFactory();
		if (!gof)
		{
			std::cout << "Error allocating memory for GameObjectFactory\n";
		}	
		//std::cout << "sm ctor\n";
	}

	NewSceneManager::~NewSceneManager()
	{
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

	}

	void NewSceneManager::init()
	{
		// Load default scene

		// Debug Purposes
		std::string str("Data\\sandbox.json");
		load_scene(str);
		sceneFilePath = str;
		//std::cout << "No. of GameObjects in scene:" << currentScene->get_gameobjcount() << std::endl;
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

		sceneFilePath = _filepath;

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


	// M2
	bool NewSceneManager::startPreview()
	{
		if (!currentScene)
		{
			PRINT("Cannot preview scene as there is no scene! There might be too much copium in your system...\n");
			return false;
		}
		storageScene = currentScene;
		currentScene = nullptr;
		
		// Make copy 
		Scene* tmp = new NormalScene(storageScene->get_filename());
		if (!tmp)
			return false;

		tmp->set_name(storageScene->get_name());

		// Copy game object data
		for (size_t i{ 0 }; i < storageScene->get_gameobjcount(); ++i)
		{

			// Build a game object copy from original scene
			GameObject* go = new GameObject(*(storageScene->get_gameobjectvector()[i]));
			if (go)
				tmp->get_gameobjectvector().emplace_back(go);
		}

		currentScene = tmp;
		return true;


	}
	bool NewSceneManager::endPreview()
	{

		// Delete memory for the preview scene
		if (!currentScene)
			return false;
		delete currentScene;

		// Swap the original unmodified scene back to current scene
		currentScene = nullptr;
		currentScene = storageScene;
		return true;

	}

	bool NewSceneManager::save_scene()
	{
		double startTime = glfwGetTime();
		std::cout << "saving scene...\n";
		if (!save_scene(sceneFilePath))
			return false;

		std::cout << "save complete\n";
		std::cout << "Time taken: " << glfwGetTime() - startTime << std::endl;
		return true;
	}
	bool NewSceneManager::save_scene(const std::string& _filepath)
	{
		rapidjson::Document doc;

		doc.SetObject();
		rapidjson::Value name;
		create_rapidjson_string(doc, name,  currentScene->get_name());
		doc.AddMember("Name", name, doc.GetAllocator());

		std::vector<GameObject*> roots;
		for (size_t i{ 0 }; i < currentScene->get_gameobjcount(); ++i) {
			GameObject* tmp = currentScene->get_gameobjectvector()[i];
			if (!tmp->has_parent())
				roots.emplace_back(tmp);
		}

		//Create array of game objects
		rapidjson::Value gameObjects(rapidjson::kArrayType);
		for (size_t i{ 0 }; i < roots.size(); ++i) {

			GameObject* tmp = roots[i];
			if (tmp->has_parent())
				continue;

			rapidjson::Value go(rapidjson::kObjectType);
			tmp->serialize(go, doc);

			
			//rapidjson::Value components(rapidjson::kArrayType);
			// Insert transform component into component array


			gameObjects.PushBack(go, doc.GetAllocator());
		}

		doc.AddMember("GameObjects", gameObjects, doc.GetAllocator());

		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
		doc.Accept(writer);
		std::ofstream ofs(_filepath);
		ofs << sb.GetString();
		//std::cout << sb.GetString() << std::endl;
		return true;
	}

	void create_rapidjson_string(rapidjson::Document& _doc, rapidjson::Value& _value, const std::string& _str)
	{
		_value.SetString(_str.c_str(), _str.length(), _doc.GetAllocator());
	}

}
