/*!***************************************************************************************
****
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
******************************************************************************************
****/
#include "pch.h"
#include "scene-manager.h"

//Ctors and Dtor
SceneManager::SceneManager() : current(0), next(0), previous(0), numberOfScenes(0), currentScene(nullptr) {
	
}
SceneManager::~SceneManager() {
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

void SceneManager::addScene(Scene* _newScene) {
	scenes.push_back(_newScene);
	numberOfScenes++;
}
bool SceneManager::changeScene(int _id) {
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
Scene* SceneManager::getCurrentScene() {
	return currentScene;
}
int SceneManager::getSceneCount() const {
	return numberOfScenes;
}

//Scene State Functions
void SceneManager::loadScene() {
	if(currentScene)
		currentScene->loadScene();
}
void SceneManager::initScene() {
	if(currentScene)
		currentScene->initScene();
}
void SceneManager::updateScene() {
	if (currentScene)
		currentScene->updateScene();
}
void SceneManager::drawScene() {
	if (currentScene)
		currentScene->drawScene();
}
void SceneManager::freeScene() {
	if (currentScene)
		currentScene->freeScene();
}
void SceneManager::unloadScene() {
	if (currentScene)
		currentScene->unloadScene();
}

