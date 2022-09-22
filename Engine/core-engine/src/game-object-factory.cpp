/*!***************************************************************************************
****
\file			game-object-factory.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
	Contains definitions for the GameObjectFactory class.
	The GameObjectFactory is responsible for the creation, management and destruction of game objects. 
	Note: The GameObjectFactory will also place the created gameobjects inside the current scene.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
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

	GameObject* GameObjectFactory::build_gameobject()
	{
		GameObject* tmp = new GameObject();
		if (!tmp)
			return nullptr;

		return tmp;
	}
	GameObject* GameObjectFactory::build_gameobject(rapidjson::Value& _value) {

		if (_value.GetType() == rapidjson::kObjectType) {
		}

		GameObject* go = new GameObject();
		if (!go)
			return nullptr;

		if (!go->deserialize(_value))
		{
			delete go;
			return nullptr;
		}

		currentScene->add_gameobject(go);			
		std::cout << "building 1x game object\n";

		unsigned int childCount{ 0 };
		// Deserialize children (if any)
		if (_value.HasMember("Children")) {

			rapidjson::Value& childArr = _value["Children"].GetArray();
			for (rapidjson::Value::ValueIterator iter = childArr.Begin(); iter != childArr.End(); ++iter)
			{
				GameObject* cgo = build_gameobject(*iter);
				go->attach_child(cgo);
				++childCount;
			}

		}

		//std::cout << "No. of children:" << childCount << std::endl;
		return go;
	}
	GameObject* GameObjectFactory::clone_gameobject(GameObject* _src)
	{
		GameObject* tmpGO = new GameObject();
		if (!tmpGO)
			return nullptr;

		tmpGO->set_name(_src->get_name());

		currentScene->add_gameobject(tmpGO);
		return tmpGO;
	}

	void GameObjectFactory::link_to_scene(Scene* _s)
	{
		currentScene = _s;
	}
	bool GameObjectFactory::delete_gameobject(GameObject* _go)
	{
		if (!_go)
			return false;

		delete _go;
		_go = nullptr;
		//Iterate through currentScene vector and destroy
		for (size_t i{ 0 }; i < currentScene->get_gameobjectvector().size(); ++i)
		{
			if (!currentScene->get_gameobjectvector()[i])
			{
				currentScene->get_gameobjectvector().erase(currentScene->get_gameobjectvector().begin() + i);
				currentScene->get_gameobjectvector().shrink_to_fit();
				break;

			}
		}

		return true;

	}
}
