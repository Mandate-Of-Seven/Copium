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
#include "SceneManager/sm.h"
#include "GameObject/renderer-component.h"
#include <rttr/registration>
#include <filesystem>

namespace 
{
	Copium::NewSceneManager& sceneManager{ *Copium::NewSceneManager::Instance() };
}

namespace Copium 
{
	GameObjectFactory::GameObjectFactory()
	{
		std::cout << "GOF ctor\n";
		//register_archetypes("Data/Archetypes");

	}
	GameObjectFactory::~GameObjectFactory()
	{
		//clear_archetypes();
	}

	GameObject* GameObjectFactory::build_gameobject()
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
		{
			std::cout << "There is no scene loaded\n";
			return nullptr;

		}


		unsigned count{ 0 };
		for (GameObject* go : currScene->get_gameobjectvector())
		{
			if (go->get_name().find("New GameObject") != std::string::npos)
				++count;
		}



		GameObject* tmp = new GameObject();
		if (!tmp)
			return nullptr;
		if (count)
		{
			std::string postfix('(' + std::to_string(count) + ')');
			tmp->set_name(tmp->get_name() + postfix);
		}
		currScene->add_gameobject(tmp);

		return tmp;
	}


	GameObject* GameObjectFactory::build_gameobject(GameObject& _src)
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return nullptr;
		GameObject* go = new GameObject(_src);
		if (!go)
			return nullptr;

		currScene->add_gameobject(go);

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

		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return nullptr;
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
					{
						Component* tmp = go->addComponent(Component::nameToType(key));
						if (tmp)
							tmp->deserialize(component);
					}						

				}
			}
		}

		currScene->add_gameobject(go);

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

	bool GameObjectFactory::delete_gameobject(GameObject* _go)
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return false;
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

		//Iterate through currentScene vector and destroy
		for (size_t i{ 0 }; i < currScene->get_gameobjectvector().size(); ++i)
		{
			if (currScene->get_gameobjectvector()[i] == _go)
			{
				delete _go;
				currScene->get_gameobjectvector()[i] = nullptr;
				std::cout << "trimming go vector\n";
				currScene->get_gameobjectvector().erase(currScene->get_gameobjectvector().begin() + i);
				currScene->get_gameobjectvector().shrink_to_fit();
				std::cout << "Number of GameObjects left: " << currScene->get_gameobjcount() << std::endl;
				break;

			}
		}

		std::cout << "Number of Game Objects left: " << currScene->get_gameobjcount() << std::endl;

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

		//std::cout << "Object name: " << go->get_name() << std::endl;

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
					{
						Component* tmp = go->addComponent(Component::nameToType(key));
						if (tmp)
							tmp->deserialize(component);
					}

				}
			}
		}
		// Deserialize children (if any)
		if (_value.HasMember("Children")) {
			rapidjson::Value& childArr = _value["Children"].GetArray();
			for (rapidjson::Value::ValueIterator iter = childArr.Begin(); iter != childArr.End(); ++iter)
			{
				GameObject* cgo = build_gameobject(*iter);
				go->attach_child(cgo);
			}
		}

		//std::cout << "No. of children:" << childCount << std::endl;
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
			if (tmp)
			{
				std::cout << "Registering " << doc["Archetype"].GetString() << std::endl;
				gameObjectCreators.emplace(doc["Archetype"].GetString(), tmp);
			}

			ifs.close();
		}
		std::cout << "Registration of Archetypes End----\n";
		return true;
	}
	void GameObjectFactory::clear_archetypes()
	{
		for (std::map<std::string, GameObject*>::iterator iter = gameObjectCreators.begin(); iter != gameObjectCreators.end(); ++iter)
		{
			if ((*iter).second != nullptr) {
				delete (*iter).second;
				(*iter).second = nullptr;
			}
		}
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
		
		Component* tmp = _go->addComponent(Component::nameToType(_key));
		return tmp != nullptr;
	}


	std::map<std::string, GameObject*>& GameObjectFactory::get_archetype_map()
	{
		return gameObjectCreators;
	}



}
