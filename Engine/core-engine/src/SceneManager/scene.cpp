/*!***************************************************************************************
\file			scene.cpp
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains definitions for the Scene class.
	The Scene contains:
		1. load, init, update, draw, free, unload function
		2. string containing the filename of the file in which the scene data is stored on
		3. Data pertaining to the game objects in the scene

	Contains definitions for NormalScene class which is a derived class from Scene class.
	Note: this is the latest version of our scene class, use this

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "SceneManager/scene.h"

namespace Copium
{
Scene::Scene() : name{ "NewScene" }, currSceneState{ SceneState::edit }
{

}
Scene::Scene(const std::string& _filepath) : filename(_filepath) 
{

}
Scene::~Scene()
{

	std::cout << get_gameobjcount() << std::endl;


	for (size_t i{ 0 }; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i] != nullptr)
		{
			//std::cout << "deleting a game object\n";
			delete gameObjects[i];
			gameObjects[i] = nullptr;
		}
		
	}
	
	gameObjects.shrink_to_fit();
	std::cout << "game objects destroyed\n";
}

void Scene::update_scene() 
{
	std::cout << "scene updated by default\n";
}
void Scene::draw_scene() 
{
	std::cout << "scene drawn by default\n";
}





std::string Scene::get_filename() const {return filename;}
size_t Scene::get_gameobjcount() const { return gameObjects.size(); }
std::vector<GameObject*>& Scene::get_gameobjectvector() { return gameObjects;}
GameObject* Scene::add_gameobject(GameObject* _gameObj) 
{
	gameObjects.push_back(_gameObj);
	return _gameObj;
}

void Scene::set_name(const std::string& _name) { name = _name; std::cout << name << std::endl; }
std::string Scene::get_name() const{ return name; }
void Scene::inspector_view()
{

}

//M3
void Scene::incr_component_count() { ++numberOfComponents; }
unsigned int Scene::get_component_count() const { return numberOfComponents; }
ComponentID Scene::assignComponentID()
{
	if (unusedCIDs.empty())
	{
		incr_component_count();
		std::cout << "Assigning Component ID: " << numberOfComponents << std::endl;
		return static_cast<ComponentID>(numberOfComponents);
	}
	else {
		ComponentID cid = unusedCIDs.front();
		std::cout << "Taking over cid: " << cid << std::endl;
		unusedCIDs.erase(unusedCIDs.begin());
		return cid;
	}

}
void Scene::add_unused_cid(ComponentID _id)
{
	unusedCIDs.emplace_back(_id);
}
std::vector<ComponentID>& Scene::get_unusedcids() { return unusedCIDs; }
GameObjectID Scene::assignGameObjID()
{

	if (!unusedGIDs.empty())
	{
		// Pop the first unused GID
		GameObjectID id = unusedGIDs[0];
		unusedGIDs.erase(unusedGIDs.begin());
		std::cout << "taking over unused ID: " << id << std::endl;
		return id;
	}

	GameObjectID id = (GameObjectID)gameObjects.size();
	std::cout << "assigning new ID " << id << std::endl;
	return id;
}
void Scene::add_unused_gid(GameObjectID _id)
{
	unusedGIDs.emplace_back(_id);
	//Sort?
}
std::vector<GameObjectID>& Scene::get_unusedgids() { return unusedGIDs; }
Scene::SceneState Scene::get_state() const { return currSceneState; }
void Scene::set_state(SceneState _newState) { currSceneState = _newState; }

// Normal Scene
NormalScene::NormalScene(){}
NormalScene::NormalScene(const std::string& _filepath) : Scene(_filepath)
{

}

void NormalScene::load_scene()
{

}

void NormalScene::init_scene()
{

}

void NormalScene::update_scene()
{

}

void NormalScene::draw_scene()
{

}

void NormalScene::free_scene()
{

}

void NormalScene::unload_scene()
{

}


}




