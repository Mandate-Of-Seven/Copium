/*!***************************************************************************************
\file			game-object-factory.cpp
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
	Contains definitions for the GameObjectFactory class.
	The GameObjectFactory is responsible for the creation, management and destruction of game objects. 
	Note: The GameObjectFactory will also place the created gameobjects inside the current scene.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include <pch.h>
#include "GameObject/game-object-factory.h"
#include "GameObject/renderer-component.h"
#include <rttr/registration>
#include <filesystem>

namespace Copium 
{
	GameObjectFactory::GameObjectFactory() : currentScene{ nullptr }
	{
		std::cout << "GOF ctor\n";

	}
	GameObjectFactory::~GameObjectFactory()
	{
	}

	GameObject* GameObjectFactory::build_gameobject()
	{
		GameObject* tmp = new GameObject();
		if (!tmp)
			return nullptr;
		currentScene->add_gameobject(tmp);

		return tmp;
	}


	GameObject* GameObjectFactory::build_gameobject(GameObject& _src)
	{
		GameObject* go = new GameObject(_src);
		if (!go)
			return nullptr;

		currentScene->add_gameobject(go);

		//for (std::list<GameObject*>::iterator iter = _src.mchildList().begin(); iter != _src.mchildList().end(); ++iter)
		//{
		//	if (!(*iter))
		//		continue;
		//	//std::cout << "adding a child\n";
		//	GameObject* cgo = build_gameobject(*(*iter));
		//	if (!cgo)
		//		break;

		//	cgo->set_parent(go);
		//	go->attach_child(cgo);

		//}
		return go;

	}

	GameObject* GameObjectFactory::build_gameobject(rapidjson::Value& _value) {

		GameObject* go = new GameObject();
		if (!go)
			return nullptr;

		if (!go->deserialize(_value))
		{
			delete go;
			return nullptr;
		}

		std::cout << "Object name: " << go->get_name() << std::endl;

		if (_value.HasMember("Components")) 
		{
			rapidjson::Value& compArr = _value["Components"].GetArray();
			for (rapidjson::Value::ValueIterator iter = compArr.Begin(); iter != compArr.End(); ++iter)
			{
				rapidjson::Value& component = *iter;
				if (component.HasMember("Type")) 
				{
					std::string key = component["Type"].GetString();
					//PRINT("Component: " << name);
					if (key == "Transform")
						// deserialize transform component
						go->Transform().deserialize(component);
					else
						add_component(key, go);

				}
			}
		}

		currentScene->add_gameobject(go);					

		//unsigned int childCount{ 0 };
		// Deserialize children (if any)
		if (_value.HasMember("Children")) {
			rapidjson::Value& childArr = _value["Children"].GetArray();
			for (rapidjson::Value::ValueIterator iter = childArr.Begin(); iter != childArr.End(); ++iter)
			{
				GameObject* cgo = build_gameobject(*iter);
				go->attach_child(cgo);
				//++childCount;
			}
		}

		//std::cout << "No. of children:" << childCount << std::endl;
		return go;
	}
	GameObject* GameObjectFactory::clone_gameobject(GameObject* _src)
	{
		GameObject* tmpGO = build_gameobject(*_src);
		if (!tmpGO)
			return nullptr;

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

		// Deattach children from this game object (if any)
		for (std::list<GameObject*>::iterator iter = _go->mchildList().begin(); iter != _go->mchildList().end(); ++iter)
		{
			GameObjectID id{ 0 };
			(*iter)->set_parent(nullptr);
			(*iter)->set_ppid(id);
		}

		std::cout << "Deleting " << _go->get_name() << std::endl;

		delete _go;

		//Iterate through currentScene vector and destroy
		for (size_t i{ 0 }; i < currentScene->get_gameobjectvector().size(); ++i)
		{
			if (currentScene->get_gameobjectvector()[i] == _go)
			{
				std::cout << "trimming go vector\n";
				currentScene->get_gameobjectvector().erase(currentScene->get_gameobjectvector().begin() + i);
				currentScene->get_gameobjectvector().shrink_to_fit();
				break;

			}
		}

		std::cout << "Number of Game Objects left: " << currentScene->get_gameobjcount() << std::endl;

		return true;

	}

	GameObject* GameObjectFactory::build_archetype(rapidjson::Value& _value)
	{
		GameObject* go = new GameObject();
		if (!go)
			return nullptr;

		if (!go->deserialize(_value))
		{
			delete go;
			return nullptr;
		}

		if (_value.HasMember("Components"))
		{
			rapidjson::Value& compArr = _value["Components"].GetArray();
			for (rapidjson::Value::ValueIterator iter = compArr.Begin(); iter != compArr.End(); ++iter)
			{
				rapidjson::Value& component = *iter;
				if (component.HasMember("Type"))
				{
					const char* name = component["Type"].GetString();
					go->addComponent(Component::nameToType(name))->deserialize(component);
				}
			}
		}

		if (_value.HasMember("Children")) {

			rapidjson::Value& childArr = _value["Children"].GetArray();
			for (rapidjson::Value::ValueIterator iter = childArr.Begin(); iter != childArr.End(); ++iter)
			{
				GameObject* cgo = build_gameobject(*iter);
				go->attach_child(cgo);
			}

		}
		//std::cout << '\n';
		return go;
	}
	bool GameObjectFactory::register_archetypes(const std::filesystem::path& _directoryPath)
	{
		std::cout << "Registration of Archetypes Begin----\n";
		std::filesystem::directory_entry dir(_directoryPath);

		// Iterate through the Archetypes folder and create a game object for each archetype file
		for (const auto& dir_entry : std::filesystem::directory_iterator{ dir })
		{
			std::ifstream ifs(dir_entry.path());
			if (!ifs)
				continue;
			rapidjson::IStreamWrapper isw(ifs);
			rapidjson::Document doc;
			doc.ParseStream(isw);
			GameObject* tmp = build_archetype(doc);
			if (!tmp)
			{
				ifs.close();
				continue;
			}
			gameObjectCreators.emplace(doc["Archetype"].GetString(), tmp);
			ifs.close();
		}
		std::cout << "Registration of Archetypes End----\n";
		return true;
	}
	GameObject* GameObjectFactory::build_gameobject(const std::string& _archetype)
	{
		if (gameObjectCreators.find(_archetype) == gameObjectCreators.end())
			return nullptr;
		std::cout << "Making an archetype: " << _archetype << std::endl;
		GameObject* tmp = build_gameobject(*gameObjectCreators[_archetype]);
		return tmp;
	}
	bool GameObjectFactory::add_component(const std::string& _key, GameObject* _go)
	{
		
		_go->addComponent(Component::nameToType(_key));
		return true;
	}


}
