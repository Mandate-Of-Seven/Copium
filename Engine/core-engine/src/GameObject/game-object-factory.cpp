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
#include "SceneManager/scene-manager.h"
#include "GameObject/Components/renderer-component.h"
#include <rttr/registration>
#include <filesystem>
#include <Editor/editor-undoredo.h>
#include <Editor/editor-system.h>
#include <GameObject/components.h>

namespace 
{
	Copium::SceneManager& sceneManager{ *Copium::SceneManager::Instance() };
	Copium::MessageSystem& messageSystem{ *Copium::MessageSystem::Instance() };
}

namespace Copium 
{
	Component& AddComponent(GameObject& gameObj ,ComponentType componentType)
	{
		switch (componentType)
		{
			switch (componentType)
			{
			case ComponentType::Animator:
				return gameObj.AddComponent<Animator>();
			case ComponentType::BoxCollider2D:
				return &addComponent<BoxCollider2D>();
			case ComponentType::Camera:
				return &addComponent<Camera>();
			case ComponentType::Rigidbody2D:
				return &addComponent<Rigidbody2D>();
			case ComponentType::SpriteRenderer:
				return &addComponent<SpriteRenderer>();
			case ComponentType::Script:
				return &addComponent<Script>();
			case ComponentType::Button:
				return &addComponent<Button>();
			case ComponentType::Image:
				return &addComponent<ImageComponent>();
			case ComponentType::Text:
				return &addComponent<Text>();
			case ComponentType::AudioSource:
				return &addComponent<AudioSource>();
			case ComponentType::SortingGroup:
				return &addComponent<SortingGroup>();
			default:
				PRINT("ADDED NOTHING, MAYBE ADDED THE COMPONENT TO THE GAMEOBJECT.CPP");
				break;
			}
		}
		return nullptr;
	}

	GameObjectFactory::GameObjectFactory()
	{

	}
	GameObjectFactory::~GameObjectFactory()
	{
	}

	GameObject* GameObjectFactory::instantiate()
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
		{
			std::cout << "There is no scene loaded\n";
			return nullptr;
		}


		unsigned count{ 0 };
		for (GameObject* go : currScene->gameObjects)
		{
			if (go->get_name().find("New GameObject") != std::string::npos)
				++count;
		}
		PRINT("Number of GameObjects with same name:" << count);

		GameObject* tmp = new GameObject(currScene->assignGameObjID());

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
	GameObject* GameObjectFactory::instantiate(GameObject& _src)
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return nullptr;
		GameObject* go = new GameObject(currScene->assignGameObjID());
		if (!go)
			return nullptr;

		*go = _src;

		unsigned count{ 0 };
		for (GameObject* g : currScene->gameObjects)
		{
			if (g->get_name().find(_src.get_name()) != std::string::npos)
				++count;
		}
		PRINT("Number of GameObjects with same name:" << count);

		if(count)
			go->set_name(go->get_name() + '(' + std::to_string(count) + ')');

		currScene->add_gameobject(go);

		//for (std::list<GameObject*>::iterator iter = _src.mchildList().begin(); iter != _src.mchildList().end(); ++iter)
		//{
		//	if (!(*iter))
		//		continue;
		//	//std::cout << "adding a child\n";
		//	GameObject* cgo = instantiate(*(*iter));
		//	if (!cgo)
		//		break;

		//	cgo->set_parent(go);
		//	go->attach_child(cgo);

		//}
		return go;
	}

	GameObject* GameObjectFactory::clone(GameObject& _src, Scene* scene)
	{
		GameObject* go = new GameObject(_src);
		if (!go)
			return nullptr;


		unsigned count{ 0 };
		if (scene)
			for (GameObject* g : scene->gameObjects)
			{
				if (g->get_name().find(_src.name) != std::string::npos)
					++count;
			}
		if (count)
			go->name += '(' + std::to_string(count) + ')';

		if (scene)
		{
			scene->add_gameobject(go);
		}


		//for (std::list<Transform*>::iterator iter = _src.transform.children.begin(); iter != _src.transform.children.end(); ++iter)
		//{
		//	if (!(*iter))
		//		continue;
		//	GameObject* cgo = clone((*iter)->gameObj,scene);
		//	if (!cgo)
		//		break;
		//	cgo->transform.setParent(&go->transform);
		//}
		return go;
	}


	GameObject* GameObjectFactory::clone(GameObject& _src)
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return nullptr;
		GameObject* go = new GameObject(currScene->assignGameObjID());
		if (!go)
			return nullptr;
		go->transform.position = _src.transform.position;
		go->transform.scale = _src.transform.scale;
		go->transform.rotation = _src.transform.rotation;
		for (Component* component : _src.components)
		{
			ComponentID newID = _src.assign_id();
			Component* newComponent = component->clone(*go,&newID);
			newComponent->id = newID;
			go->components.push_back(newComponent);
		}

		unsigned count{ 0 };
		if (currScene)
			for (GameObject* g : currScene->gameObjects)
			{
				if (g->get_name().find(_src.name) != std::string::npos)
					++count;
			}
		if (count)
			go->name = _src.name + '(' + std::to_string(count) + ')';

		if (currScene)
		{
			currScene->add_gameobject(go);
			for (Transform* transform : _src.transform.children)
			{
				GameObject* cgo{clone(transform->gameObj)};
				cgo->transform.setParent(&go->transform);
			}
		}
		for (size_t compIndex{ 0 }; compIndex < _src.components.size(); ++compIndex)
		{
			go->components[compIndex]->previewLink(_src.components[compIndex]);
		}
		return go;
	}

	GameObject* GameObjectFactory::instantiate(rapidjson::Value& _value) 
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return nullptr;

		GameObject* go = nullptr;
		if (_value.HasMember("ID"))
		{
			go = new GameObject(_value["ID"].GetUint64());
		}
		else
		{
			go = new GameObject(currScene->assignGameObjID());
		}

		if (!go)
		{
			PRINT("FAILED TO DESERIALIZE");
			return nullptr;
		}
		//go->id = currScene->assignGameObjID();


		if (!go->deserialize(_value))
		{
			PRINT("FAILED TO DESERIALIZE");
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
					std::string key = component["Type"].GetString();
					//PRINT("Component: " << name);
					if (key == "Transform")
						// deserialize transform component
						go->transform.deserialize(component);
					else
					{
						Component* tmp = go->Add(Component::nameToType(key));
						//PRINT();
						if (tmp)
							tmp->deserialize(component);
					}						

				}
			}
		}

		currScene->add_gameobject(go);

		// Deserialize children (if any)
		//if (_value.HasMember("Children")) {
		//	rapidjson::Value& childArr = _value["Children"].GetArray();
		//	for (rapidjson::Value::ValueIterator iter = childArr.Begin(); iter != childArr.End(); ++iter)
		//	{
		//		GameObject* cgo = instantiate(*iter);
		//		cgo->transform.setParent(&go->transform);
		//	}
		//}

		return go;
	}

	bool GameObjectFactory::destroy(GameObject* _go)
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return false;
		if (!_go)
			return false;

		currScene->gameObjects.erase(std::remove_if(currScene->gameObjects.begin(), currScene->gameObjects.end(), [&](GameObject* gameObj) {return gameObj == _go; }));


		// Add the game object's ID to unused pile
		currScene->add_unused_gid(_go->id);


		// Deattach children from this game object (if any)
		for (auto pTransform : _go->transform.children)
		{
			destroy(&pTransform->gameObj);
		}
		//std::cout << "Deleting " << _go->get_name() << std::endl;

		//std::cout << "number of game objects left: " << currScene->get_gameobjcount() << std::endl;
		for (size_t i{ 0 }; i < currScene->gameObjectSPTRS.size(); ++i)
		{
			if (currScene->gameObjectSPTRS[i].get() == _go)
			{
				//std::cout << "take out of sptr vector\n";
				
				currScene->gameObjectSPTRS.erase(currScene->gameObjectSPTRS.begin() + i);
				break;
			}
		}

		return true;
	}


	bool GameObjectFactory::destroy(GameObjectID id)
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return false;

		GameObject* _go = sceneManager.findGameObjByID(id);
		if (!_go)
		{
			return false;
		}

		currScene->add_unused_gid(_go->id);

		// Deattach children from this game object (if any)
		for (auto pTransform : _go->transform.children)
		{
			pTransform->setParent(nullptr);
			destroy(&pTransform->gameObj);
		}

		//std::cout << "Deleting " << _go->get_name() << std::endl;

		if (_go->transform.hasParent())
		{
			_go->transform.setParent(nullptr);
		}

		currScene->gameObjects.erase(std::remove_if(currScene->gameObjects.begin(), currScene->gameObjects.end(), [&](GameObject* gameObj) {return gameObj == _go; }));
		std::cout << "number of game objects left: " << currScene->get_gameobjcount() << std::endl;
		for (size_t i{ 0 }; i < currScene->gameObjectSPTRS.size(); ++i)
		{
			if (currScene->gameObjectSPTRS[i].get()->id == id)
			{
				std::cout << "take out of sptr vector\n";
				currScene->gameObjectSPTRS.erase(currScene->gameObjectSPTRS.begin() + i);
				break;
			}
		}
		return true;
	}

	GameObject* GameObjectFactory::build_archetype(rapidjson::Value& _value)
	{
		Scene* currScene = sceneManager.get_current_scene();
		if (!currScene)
			return nullptr;

		GameObject* go = new GameObject(0);

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
					std::string key = component["Type"].GetString();
					//PRINT("Component: " << key);
					if (key == "Transform")
						// deserialize transform component
						go->transform.deserialize(component);
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
				GameObject* cgo = instantiate(*iter);
				go->transform.setParent(&cgo->transform);
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
				archetypes.emplace(doc["Archetype"].GetString(), tmp);
			}

			ifs.close();
		}
		std::cout << "Registration of Archetypes End----\n";
		return true;
	}
	void GameObjectFactory::clear_archetypes()
	{
		for (std::map<std::string, GameObject*>::iterator iter = archetypes.begin(); iter != archetypes.end(); ++iter)
		{
			if ((*iter).second != nullptr) {
				delete (*iter).second;
				(*iter).second = nullptr;
			}
		}
	}

	GameObject* GameObjectFactory::instantiate(const std::string& _archetype)
	{
		if (archetypes.find(_archetype) == archetypes.end())
			return nullptr;
		std::cout << "Making an archetype: " << _archetype << std::endl;
		GameObject* tmp = instantiate(*archetypes[_archetype]);
		return tmp;
	}
	bool GameObjectFactory::add_component(const std::string& _key, GameObject* _go)
	{
		
		std::cout << "adding component\n";
		Component* tmp = _go->addComponent(Component::nameToType(_key));
		Scene* currScene = sceneManager.get_current_scene();
		if (currScene)
		{
			currScene->assignComponentID();
			currScene->incr_component_count();
		}
		return tmp != nullptr;
	}
	


	std::map<std::string, GameObject*>& GameObjectFactory::get_archetype_map()
	{
		return archetypes;
	}

	GameObject* GameObjectFactory::create_child(GameObject& _parent)
	{
		Scene* currScene = sceneManager.get_current_scene();
		GameObject* newChild = new GameObject(currScene->assignGameObjID());
		newChild->transform.setParent(&_parent.transform);
		currScene->add_gameobject(newChild);
		//auto it = currScene->gameObjects.begin();
		//for (auto pGameObject : currScene->gameObjects)
		//{
		//	if (pGameObject->id == _parent.id)
		//	{
		//		currScene->gameObjects.insert(it + 1, newChild);
		//		_parent.transform.setParent(&newChild->transform);
		//		return newChild;
		//	}
		//	++it;
		//}
		
		//for (auto iter = currScene->gameObjects.begin(); iter != currScene->gameObjects.end(); ++iter)
		//{
		//	if ((*iter)->id == _parent.id)
		//	{
		//		//currScene->gameObjects.insert(iter + 1, newChild);
		//		currScene->add_gameobject(newChild);
		//		newChild->transform.setParent(&_parent.transform);
		//		return newChild;
		//	}
		//}

		return newChild;

	}

}
