/*!***************************************************************************************
\file			scenemanager.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains definitions of the functions pertaining to the Scene Manager class. 
	The Scene Manager is able to manage the different scenes assigned to it, allowing for:
		1. Transitioning between scenes
		2. Calling of load, init, update, draw, free and unload state functions for each scene
		3. Assigning of new scenes to itself

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "SceneManager/scene-manager.h"

//Ctors and Dtor
SceneManager::SceneManager() : current(0), next(0), previous(0), numberOfScenes(0), currentScene(nullptr) 
{
	
}
SceneManager::~SceneManager() 
{
	for (size_t i{ 0 }; i < numberOfScenes; ++i) {
		if (scenes[i]) {
			delete scenes[i];
		}
	}
	scenes.clear();
	numberOfScenes = 0;
	std::cout << "Scene Manager Destructor called\n";
	std::cout << "Scenes left: " << numberOfScenes << std::endl;
}

void SceneManager::add_scene(Scene* _newScene) 
{
	scenes.push_back(_newScene);
	numberOfScenes++;
}
bool SceneManager::change_scene(int _id) 
{
	if (_id > (numberOfScenes - 1) || _id < gsQuit) {
		std::cout << "Invalid scene ID, change terminated\n";
		return false;
	}

	if (_id >= 0) {
		currentScene = scenes[_id];
	}
	
	next = _id;
	return true;
}
Scene* SceneManager::get_currentscene() {return currentScene;}
int SceneManager::get_scenecount() const {return numberOfScenes;}

//Scene State Functions
void SceneManager::load_scene() 
{
	if(currentScene)
		currentScene->load_scene();
}
void SceneManager::init_scene() 
{
	if(currentScene)
		currentScene->init_scene();
}
void SceneManager::update_scene() 
{
	if (currentScene)
		currentScene->update_scene();
}
void SceneManager::draw_scene() 
{
	if (currentScene)
		currentScene->draw_scene();
}
void SceneManager::free_scene() 
{
	if (currentScene)
		currentScene->free_scene();
}
void SceneManager::unload_scene() 
{
	if (currentScene)
		currentScene->unload_scene();
}



