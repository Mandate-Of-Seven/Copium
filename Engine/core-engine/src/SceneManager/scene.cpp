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

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
}

void Scene::update_scene() 
{
}
void Scene::draw_scene() 
{
}

std::string Scene::get_filename() const {return filename;}

void Scene::set_name(const std::string& _name) { name = _name; }
std::string Scene::get_name() const{ return name; }
//M3
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




