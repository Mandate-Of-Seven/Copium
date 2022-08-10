/*!***************************************************************************************
****
\file			scene.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains definitions for the Scene class.
	The Scene contains:
		1. load, init, update, draw, free, unload function
		2. string containing the filename of the file in which the scene data is stored on
		3. Data pertaining to the game objects in the scene

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include "scene.h"

Scene::Scene(std::string& _filepath) : filename(_filepath) 
{

}
Scene::~Scene()
{
	for (size_t i{ 0 }; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i])
		{
			delete gameObjects[i];
			gameObjects[i] = nullptr;
		}
		
	}
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
void Scene::set_filename(std::string& _newFilename) {filename = _newFilename;}
size_t Scene::get_gameobjcount() const { return gameObjects.size(); }
std::vector<GameObject*>& Scene::get_gameobjectvector() { return gameObjects;}
GameObject* Scene::add_gameobject(GameObject* _gameObj) 
{
	gameObjects.push_back(_gameObj);
	return _gameObj;
}
