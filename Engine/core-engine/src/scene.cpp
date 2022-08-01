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
