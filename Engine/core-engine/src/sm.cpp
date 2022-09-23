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
#include <sm.h>

namespace Copium {
	
	NewSceneManager::NewSceneManager() : gof{nullptr}, currentScene{nullptr}
	{
		gof = new GameObjectFactory();
		if (!gof)
		{
			std::cout << "Error allocating memory for GameObjectFactory\n";
		}		
	}

	NewSceneManager::~NewSceneManager()
	{
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
		//std::cout << "sm init\n";


		// Debug Purposes
		std::string str("Data\\sandbox.json");
		load_scene(str);
		std::cout << "No. of GameObjects in scene:" << currentScene->get_gameobjcount() << std::endl;
		gof->clone_gameobject(currentScene->get_gameobjectvector()[0]);
		std::cout << "No. of GameObjects in scene:" << currentScene->get_gameobjcount() << std::endl;

		std::cout << "name comparison: " << currentScene->get_gameobjectvector()[0]->get_ppid() << '|'
			<< currentScene->get_gameobjectvector()[3]->get_ppid() << std::endl;
		std::cout << "New object's id:" << currentScene->get_gameobjectvector()[3]->get_id() << std::endl;

		//currentScene->get_gameobjectvector()[3]->attach_child(currentScene->get_gameobjectvector()[4]);


		std::cout << "name comparison: " << currentScene->get_gameobjectvector()[1]->get_ppid() << '|'
			<< currentScene->get_gameobjectvector()[4]->get_ppid() << std::endl;
		std::cout << "New object's id:" << currentScene->get_gameobjectvector()[4]->get_id() << std::endl;

	}
	void NewSceneManager::update()
	{
		// call current scene's update functions
	}
	void NewSceneManager::exit()
	{
		// unload and free current scene (only if scene is still alive and has not been destructed)
		if (currentScene) {
			delete currentScene;
			currentScene = nullptr;
			std::cout << "deleting current scene\n";
		}

	}

	bool NewSceneManager::load_scene(std::string& _filepath)
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
				GameObject* tmpGO = gof->build_gameobject(*iter);
			}
		}
		
		ifs.close();
		return true;

	}

	bool NewSceneManager::change_scene(std::string& _newfilepath)
	{
		// No scene loaded, therefore cannot change
		if (!currentScene)
			return false;


		// offer the option to save before exiting


		// Clear game objects in scene
		// Deserialize from new file and overwrite other scene data




	}
}
