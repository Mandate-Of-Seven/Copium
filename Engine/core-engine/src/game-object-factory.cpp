/*!***************************************************************************************
\file			game-object-factory.cpp
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
	Contains definitions for the GameObjectFactory class.
	The GameObjectFactory is responsible for the creation of game objects. These game objects are then linked
	to their parent scenes.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include <pch.h>
#include "game-object-factory.h"
#include <rttr/registration>

namespace Copium {

	GameObjectFactory::GameObjectFactory() : currentScene{ nullptr }
	{
		std::cout << "Game Object Factory created\n";
	}
	GameObjectFactory::~GameObjectFactory()
	{
		std::cout << "Game Object Factory destructed\n";
	}


	void GameObjectFactory::init()
	{
		std::cout << "Game Object Factory initialised\n";
	}
	void GameObjectFactory::update()
	{
		
	}
	void GameObjectFactory::exit()
	{
		std::cout << "Game Object Factory exiting\n";
	}

	GameObject* GameObjectFactory::build_gameobject(rapidjson::Value& _value) {

		if (_value.GetType() == rapidjson::kObjectType) {
			std::cout << "value is an array\n";
		}

		GameObject* go = new GameObject();
		if (!go)
			return nullptr;

		if (!go->deserialize(_value))
		{
			delete go;
			return nullptr;
		}

		return go;
	}


}
