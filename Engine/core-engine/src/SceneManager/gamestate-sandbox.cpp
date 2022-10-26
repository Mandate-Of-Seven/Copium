/*!***************************************************************************************
****
\file			gamestate-sandbox.cpp (deprecated)
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains definitions for the Sandbox Scene which inherits from Scene class.
	The Scene contains:
		1. load, init, update, draw, free, unload functions

	Note: load, init, free and unload functions MUST be defined by scene sub-classes

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include <pch.h>
#include "SceneManager/gamestate-sandbox.h"
#include "GameObject/game-object-factory.h"

#include "Windows/windows-system.h"
#include "SceneManager/serializer.h"
#include <rapidjson/prettywriter.h>

// Global variables
GLfloat movement_x = 0.f, movement_y = 0.f;

SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) 
{

}

void SceneSandbox::load_scene() 
{
	std::cout << "load sandbox" << std::endl;
}
void SceneSandbox::init_scene() 
{
	std::cout << "init sandbox" << std::endl;
}

void SceneSandbox::update_scene() 
{
	//std::cout << "update sandbox" << std::endl;
	
	// Update Graphics system
}

void SceneSandbox::draw_scene() 
{
	//std::cout << "draw sandbox" << std::endl;

}

void SceneSandbox::free_scene() 
{
	std::cout << "free sandbox" << std::endl;

}

void SceneSandbox::unload_scene() 
{
	std::cout << "scene unloaded\n";
}