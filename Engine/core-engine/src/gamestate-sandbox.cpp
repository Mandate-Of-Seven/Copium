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
Vector3 v1(1, 2, 3), v2(5,1,10);
Copium::Math::Vec2 vv(1, 10);

GameObject g1(v2, v1, v1);
GameObject g2(v1, v1, v2);

using namespace Copium::Graphics;

SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) 
{

}

void SceneSandbox::load_scene() 
{
	std::cout << "load sandbox" << std::endl;
	//Copium::GameObjectFactory GOF;
	//GOF.link_to_scene(this);

	//std::ifstream ifs("Data\\sandbox.json", std::ios::in);
	//if (!ifs)
	//{
	//	std::cout << "Error opening file\n";
	//	ifs.close();
	//	return;
	//}

	//rapidjson::IStreamWrapper isw(ifs);
	//rapidjson::Document d;
	//if (d.ParseStream(isw).HasParseError())
	//{
	//	std::cout << "Parsing error\n";
	//	ifs.close();
	//	return;
	//}

	//if (d.HasMember("Name"))
	//	std::cout << "Scene Name: " << d["Name"].GetString() << std::endl;

	//if (!d.HasMember("GameObjects"))
	//{
	//	ifs.close();
	//	return;
	//}

	//rapidjson::Value& goArr = d["GameObjects"].GetArray();
	//std::vector<GameObject*> tmpGOARR;
	//for (rapidjson::Value::ValueIterator iter = goArr.Begin(); iter != goArr.End(); ++iter)
	//{
	//	GameObject* tmpGO = GOF.build_gameobject(*iter);
	//}

	//std::cout << "No. of game objects loaded:" << get_gameobjcount() << std::endl;


	//for (size_t i{ 0 }; i < get_gameobjectvector().size(); ++i)
	//{

	//	std::cout << "GameObject Name:" << get_gameobjectvector()[i]->get_name() << std::endl;
	//	std::cout << "GameObject ID:" << get_gameobjectvector()[i]->get_id() << std::endl;
	//	std::cout << "No. of children:" << get_gameobjectvector()[i]->childList().size() << std::endl;
	//	std::cout << "No. of components:" << get_gameobjectvector()[i]->Components().size() << std::endl;
	//}
	//std::cout << "Number of game objects in file: " << tmpGOARR.size() << std::endl;


	//ifs.close();
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