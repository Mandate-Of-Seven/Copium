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
#include <filesystem>
//#include <Editor/editor-undoredo.h>
//#include <Editor/editor-system.h>
#include <GameObject/game-object.h>
#include <GameObject/components.h>

namespace 
{
	Copium::MessageSystem& messageSystem{ *Copium::MessageSystem::Instance() };
}

namespace Copium 
{
	namespace
	{
		GameObjectsArray* pGameObjectsArray{};
		ComponentsArrays* pComponentsArrays{};
	}

	template <typename T>
	Component* Add(GameObject& gameObj)
	{
		T component(gameObj);
		T& newComponent = pComponentsArrays->GetArray<T>().push_back(component);
		gameObj.AddComponent<T>(&newComponent);
		return (Component*)&newComponent;
	}

	Component* AddComponent(GameObject& gameObj ,ComponentType componentType)
	{
		switch (componentType)
		{
			switch (componentType)
			{
			case ComponentType::Animator:
				return Add<Animator>(gameObj);
			case ComponentType::BoxCollider2D:
				return Add<BoxCollider2D>(gameObj);
			case ComponentType::Camera:
				return Add<Camera>(gameObj);
			case ComponentType::Rigidbody2D:
				return Add<Rigidbody2D>(gameObj);
			case ComponentType::SpriteRenderer:
				return Add<SpriteRenderer>(gameObj);
			case ComponentType::Script:
				return Add<Script>(gameObj);
			case ComponentType::Button:
				return Add<Button>(gameObj);
			case ComponentType::Image:
				return Add<Image>(gameObj);
			case ComponentType::Text:
				return Add<Text>(gameObj);
			case ComponentType::AudioSource:
				return Add<AudioSource>(gameObj);
			case ComponentType::SortingGroup:
				return Add<SortingGroup>(gameObj);
			default:
				COPIUM_ASSERT(true, "TRYING TO ADD INVALID COMPONENT TYPE");
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

	GameObject& GameObjectFactory::Instantiate()
	{
		GameObject& tmp = pGameObjectsArray->push_back();
		size_t count = pGameObjectsArray->size() - 1;
		if (count)
			tmp.name += '(' + std::to_string(count) + ')';
		return tmp;
	}

	GameObject& GameObjectFactory::Instantiate(GameObject& _src, bool copyID)
	{
		if (copyID)
		{
			GameObject& tmp = pGameObjectsArray->push_back(GameObject(_src,_src.uuid));
			size_t count = pGameObjectsArray->size() - 1;
			if (count)
				tmp.name += '(' + std::to_string(count) + ')';
			return tmp;
		}
		GameObject& tmp = pGameObjectsArray->push_back(_src);
		size_t count = pGameObjectsArray->size() - 1;
		if (count)
			tmp.name += '(' + std::to_string(count) + ')';
		return tmp;
	}

	//GameObject* GameObjectFactory::Instantiate(rapidjson::Value& _value) 
	//{
	//	if (!pGameObjectsArray)
	//		return nullptr;

	//	GameObject* go = nullptr;
	//	//if (_value.HasMember("ID"))
	//	//{
	//	//	go = new GameObject(_value["ID"].GetUint64());
	//	//}
	//	//else
	//	//{
	//	//	go = new GameObject();
	//	//}

	//	//if (!go)
	//	//{
	//	//	PRINT("FAILED TO DESERIALIZE");
	//	//	return nullptr;
	//	//}
	//	////go->id = currScene->assignGameObjID();


	//	//if (!go->deserialize(_value))
	//	//{
	//	//	PRINT("FAILED TO DESERIALIZE");
	//	//	delete go;
	//	//	return nullptr;
	//	//}

	//	//if (_value.HasMember("Components"))
	//	//{
	//	//	rapidjson::Value& compArr = _value["Components"].GetArray();
	//	//	for (rapidjson::Value::ValueIterator iter = compArr.Begin(); iter != compArr.End(); ++iter)
	//	//	{

	//	//		rapidjson::Value& component = *iter;
	//	//		if (component.HasMember("ComponentType"))
	//	//		{
	//	//			ComponentType componentType = (ComponentType)component["ComponentType"].GetInt();
	//	//			if (componentType == ComponentType::Transform)
	//	//				go->transform.deserialize(component);
	//	//			else
	//	//			{
	//	//				Component* tmp = AddComponent(*go, componentType);
	//	//				if (tmp)
	//	//					tmp->deserialize(component);
	//	//			}
	//	//		}
	//	//	}
	//	//}
	//	//currScene->add_gameobject(go);

	//	//// Deserialize children (if any)
	//	////if (_value.HasMember("Children")) {
	//	////	rapidjson::Value& childArr = _value["Children"].GetArray();
	//	////	for (rapidjson::Value::ValueIterator iter = childArr.Begin(); iter != childArr.End(); ++iter)
	//	////	{
	//	////		GameObject* cgo = instantiate(*iter);
	//	////		cgo->transform.SetParent(&go->transform);
	//	////	}
	//	////}

	//	return go;
	//}

	void GameObjectFactory::Destroy(GameObject& _go)
	{
		_go.transform.SetParent(nullptr);
		for (Transform* pTransform : _go.transform.children)
		{
			pTransform->SetParent(nullptr);
			Destroy(pTransform->gameObject);
		}
		pGameObjectsArray->erase(_go);
	}


	void GameObjectFactory::Destroy(UUID id)
	{
		GameObjectsArray::Iterator it = pGameObjectsArray->begin();
		while (it != pGameObjectsArray->end())
		{
			GameObject& gameObject = (*it);
			if (gameObject.uuid == id)
			{
				gameObject.transform.SetParent(nullptr);
				pGameObjectsArray->erase(it);
				//It is safe because the object is not destroyed
				for (Transform* pTransform : gameObject.transform.children)
				{
					pTransform->SetParent(nullptr);
					Destroy(pTransform->gameObject);
					std::string key = component["Type"].GetString();
					//PRINT("Component: " << name);
					// if (key == "Transform")
					// 	// deserialize transform component
					// 	go->transform.deserialize(component);
					// else if (key == "SortingGroup")
					// {
					// 	int sort = 0, order = 0;
					// 	if (component.HasMember("SortingLayer"))
					// 		sort = component["SortingLayer"].GetInt();

					// 	if (component.HasMember("OrderInLayer"))
					// 		order = component["OrderInLayer"].GetInt();

					// 	SortingGroup* sg = new SortingGroup(*go, order, sort);
					// 	sg->id = go->assign_id();
					// 	go->components.push_back(reinterpret_cast<Component*>(sg));
					// }
					// else
					// {
					// 	Component* tmp = go->addComponent(Component::nameToType(key));
					// 	//PRINT();
					// 	if (tmp)
					// 		tmp->deserialize(component);
					// }						

				}
				return;
			}
			++it;
		}
	}

	//GameObject* GameObjectFactory::build_archetype(rapidjson::Value& _value)
	//{
	//	Scene* currScene = sceneManager.get_current_scene();
	//	if (!currScene)
	//		return nullptr;

	//	GameObject* go = new GameObject(0);

	//	if (!go)
	//		return nullptr;

	//	if (!go->deserialize(_value))
	//	{
	//		delete go;
	//		return nullptr;
	//	}

	//	if (_value.HasMember("Components"))
	//	{
	//		rapidjson::Value& compArr = _value["Components"].GetArray();
	//		for (rapidjson::Value::ValueIterator iter = compArr.Begin(); iter != compArr.End(); ++iter)
	//		{

	//			rapidjson::Value& component = *iter;
	//			if (component.HasMember("ComponentType"))
	//			{
	//				ComponentType componentType = (ComponentType)component["ComponentType"].GetInt();
	//				if (componentType == ComponentType::Transform)
	//					go->transform.deserialize(component);
	//				else
	//				{
	//					Component* tmp = AddComponent(*go, componentType);
	//					if (tmp)
	//						tmp->deserialize(component);
	//				}
	//			}
	//		}
	//	}
	//	// Deserialize children (if any)
	//	if (_value.HasMember("Children")) {
	//		rapidjson::Value& childArr = _value["Children"].GetArray();
	//		for (rapidjson::Value::ValueIterator iter = childArr.Begin(); iter != childArr.End(); ++iter)
	//		{
	//			GameObject* cgo = instantiate(*iter);
	//			go->transform.SetParent(&cgo->transform);
	//		}
	//	}

	//	//std::cout << "No. of children:" << childCount << std::endl;
	//	return go;
	//}
	//bool GameObjectFactory::register_archetypes(const std::filesystem::path& _directoryPath)
	//{
	//	std::cout << "Registration of Archetypes Begin----\n";
	//	std::filesystem::directory_entry dir(_directoryPath);

	//	// Iterate through the Archetypes folder and create a game object for each archetype file
	//	for (const auto& dir_entry : std::filesystem::directory_iterator{ dir })
	//	{
	//		std::ifstream ifs(dir_entry.path());
	//		if (!ifs)
	//			continue;
	//		rapidjson::IStreamWrapper isw(ifs);
	//		rapidjson::Document doc;
	//		doc.ParseStream(isw);
	//		GameObject* tmp = build_archetype(doc);
	//		if (tmp)
	//		{
	//			std::cout << "Registering " << doc["Archetype"].GetString() << std::endl;
	//			archetypes.emplace(doc["Archetype"].GetString(), tmp);
	//		}

	//		ifs.close();
	//	}
	//	std::cout << "Registration of Archetypes End----\n";
	//	return true;
	//}
	//void GameObjectFactory::clear_archetypes()
	//{
	//	for (std::map<std::string, GameObject*>::iterator iter = archetypes.begin(); iter != archetypes.end(); ++iter)
	//	{
	//		if ((*iter).second != nullptr) {
	//			delete (*iter).second;
	//			(*iter).second = nullptr;
	//		}
	//	}
	//}

	GameObject& GameObjectFactory::Instantiate(const std::string& _archetype)
	{
		std::cout << "Making an archetype: " << _archetype << std::endl;
		GameObject& tmp = Instantiate(*archetypes[_archetype]);
		return tmp;
	}

	std::map<std::string, GameObject*>& GameObjectFactory::get_archetype_map()
	{
		return archetypes;
	}

	GameObject* GameObjectFactory::create_child(GameObject& _parent)
	{
		//Scene* currScene = sceneManager.get_current_scene();
		//GameObject* newChild = new GameObject();
		//newChild->transform.SetParent(&_parent.transform);
		//currScene->add_gameobject(newChild);
		//auto it = currScene->gameObjects.begin();
		//for (auto pGameObject : currScene->gameObjects)
		//{
		//	if (pGameObject->id == _parent.id)
		//	{
		//		currScene->gameObjects.insert(it + 1, newChild);
		//		_parent.transform.SetParent(&newChild->transform);
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
		//		newChild->transform.SetParent(&_parent.transform);
		//		return newChild;
		//	}
		//}

		return nullptr;

	}

}
