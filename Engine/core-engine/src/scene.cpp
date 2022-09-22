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

	Contains definitions for NormalScene class which is a derived class from Scene class.
	Note: this is the latest version of our scene class, use this

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include "pch.h"
#include "scene.h"

Scene::Scene(std::string& _filepath) : filename(_filepath) 
{

}
Scene::~Scene()
{

	std::cout << get_gameobjcount() << std::endl;


	for (size_t i{ 0 }; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i])
		{
			std::cout << "deleting a game object\n";
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
void Scene::set_filename(std::string& _newFilename) {filename = _newFilename;}
size_t Scene::get_gameobjcount() const { return gameObjects.size(); }
std::vector<GameObject*>& Scene::get_gameobjectvector() { return gameObjects;}
GameObject* Scene::add_gameobject(GameObject* _gameObj) 
{
	std::cout << "adding\n";
	gameObjects.push_back(_gameObj);
	return _gameObj;
}
bool Scene::remove_gameobject(GameObject* _gameObj)
{
	//Look for specified game object in scene
	for (size_t i{0}; i < get_gameobjectvector().size(); ++i)
	{
		GameObject* g = get_gameobjectvector()[i];
		
		if (g != _gameObj)
			continue;
		else
		{
			if (g->has_parent())
			{
				GameObject* p = g->get_parent();
				p->childList().remove(g); 
				gameObjects.erase(gameObjects.begin() + i);
				gameObjects.shrink_to_fit();

			}
			else
			{

				if (g->is_parent())
				{
					for (GameObject* c : g->childList())
					{
						delete c;
						c = nullptr;
					}

				}

				delete g;
				g = nullptr; 
			}

			return true;
		}


	}

	return false;
}

void Scene::set_name(const std::string& _name){ name = _name; }
std::string Scene::get_name() const{ return name; }

namespace Copium {
	NormalScene::NormalScene(std::string& _filepath) : Scene(_filepath)
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





