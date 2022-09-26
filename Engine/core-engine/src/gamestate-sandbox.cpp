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
#include <windows-system.h>
#include <input.h>
#include <serializer.h>
#include <rapidjson/prettywriter.h>

// Global variables
GLfloat movement_x = 0.f, movement_y = 0.f;
Vector3 v1(1, 2, 3), v2(5,1,10);
Copium::Math::Vec2 vv(1, 10);
GameObject g1(v2, v1, v1);
GameObject g2(v1, v1, v2);

SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) 
{

}

void SceneSandbox::load_scene() 
{
	std::cout << "load sandbox" << std::endl;
	//std::string filepath("Data\\sandbox.json");
	//JsonSerializer js(filepath);
	//Copium::Math::Vec2 alpha;
	//load(js, alpha);

	//std::cout << alpha;

}
void SceneSandbox::init_scene() 
{
	std::cout << "init sandbox" << std::endl;
	//Serialization Testingf
	this->get_gameobjectvector().push_back(new GameObject(v2,v2,v2));
	this->get_gameobjectvector().push_back(new GameObject(v1,v1,v1));
	this->get_gameobjectvector().push_back(new GameObject(v2, v2, v1));

	GameObject* dad = get_gameobjectvector()[0];
	GameObject* kid = get_gameobjectvector()[2];
	dad->attach_child(kid);
	GameObjectID g{ 5 };
	kid->set_id(g);

	dad->set_name("Rex");

	Copium::Math::Vec2 b1(1,1);
	std::cout << b1/2;
	
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
	std::cout << "unload sandbox" << std::endl;
	//std::cout << "does file exist: " << does_file_exist("sandbox.dat") << std::endl;
	//serialize_scene("sandbox.dat", this);
	//JSON
	std::ofstream os("Data\\sandbox.json", std::ios::out);
	if (!os)
		std::cout << "file not open\n";
	Copium::Math::Vec2 tester(3, 4);
	//ss.serialize(os, tester);

	// Bean: This should be handles by ISystem

}