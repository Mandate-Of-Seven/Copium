/*!***************************************************************************************
****
\file			gamestate-sandbox.cpp
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

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include <pch.h>
#include "gamestate-sandbox.h"
#include <game-object-factory.h>

#include <windows-system.h>
#include <graphics.h>
#include <renderer.h>
#include <input.h>
#include <serializer.h>
#include <rapidjson/prettywriter.h>

// Global variables
GLfloat movement_x = 0.f, movement_y = 0.f;


using namespace Copium::Graphics;

SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) 
{

}

void SceneSandbox::load_scene() 
{
	std::cout << "load sandbox" << std::endl;
	std::ifstream ifs("Data\\transform.json");
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);
	Transform tester;
	tester.deserialize(document);
	std::cout << tester.Position() << std::endl;
}
void SceneSandbox::init_scene() 
{
	std::cout << "init sandbox" << std::endl;

	// Init Graphics System 
	// (In the future should be stored in a vector container and looped initialised)
	graphics.init();

	//Serialization Testing
	
}

void SceneSandbox::update_scene() 
{
	//std::cout << "update sandbox" << std::endl;

	// Update Graphics system
	graphics.update();
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
	std::cout << "unload sandbox" << std::endl;


	// Bean: This should be handles by ISystem
	graphics.exit();
	std::cout << "scene unloaded\n";
}