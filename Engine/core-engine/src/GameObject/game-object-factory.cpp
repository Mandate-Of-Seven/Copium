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
#include <GameObject/game-object.h>
#include <GameObject/components.h>

#include "Utilities/serializer.h"

namespace 
{
	Copium::MessageSystem& messageSystem{ *Copium::MessageSystem::Instance() };
}

namespace Copium 
{
	//template <typename T>
	//Component* Add(GameObject& gameObj)
	//{
	//	T component(gameObj);
	//	T& newComponent = pComponentsArrays->GetArray<T>().push_back(component);
	//	gameObj.AddComponent<T>(&newComponent);
	//	return (Component*)&newComponent;
	//}

	//Component* AddComponent(GameObject& gameObj ,ComponentType componentType)
	//{
	//	switch (componentType)
	//	{
	//		switch (componentType)
	//		{
	//		case ComponentType::Animator:
	//			return Add<Animator>(gameObj);
	//		case ComponentType::BoxCollider2D:
	//			return Add<BoxCollider2D>(gameObj);
	//		case ComponentType::Camera:
	//			return Add<Camera>(gameObj);
	//		case ComponentType::Rigidbody2D:
	//			return Add<Rigidbody2D>(gameObj);
	//		case ComponentType::SpriteRenderer:
	//			return Add<SpriteRenderer>(gameObj);
	//		case ComponentType::Script:
	//			return Add<Script>(gameObj);
	//		case ComponentType::Button:
	//			return Add<Button>(gameObj);
	//		case ComponentType::Image:
	//			return Add<Image>(gameObj);
	//		case ComponentType::Text:
	//			return Add<Text>(gameObj);
	//		case ComponentType::AudioSource:
	//			return Add<AudioSource>(gameObj);
	//		case ComponentType::SortingGroup:
	//			return Add<SortingGroup>(gameObj);
	//		default:
	//			COPIUM_ASSERT(true, "TRYING TO ADD INVALID COMPONENT TYPE");
	//			break;
	//		}
	//	}
	//	return nullptr;
	//}



	template <typename T>
	T& GameObjectFactory::AddComponent(GameObject& gameObj, Scene& scene, T* pCopy, bool copyID)
	{
		if (pCopy)
		{
			if (copyID)
			{
				T& component = scene.componentArrays.GetArray<T>().emplace_back(gameObj, *pCopy, pCopy->uuid);
				gameObj.AddComponent(&component);
				return component;
			}
			else
			{
				T& component = scene.componentArrays.GetArray<T>().emplace_back(gameObj, *pCopy);
				gameObj.AddComponent(&component);
				return component;
			}
		}
		T& component = scene.componentArrays.GetArray<T>().emplace_back(gameObj);
		gameObj.AddComponent(&component);
		return component;
	}


	GameObject& GameObjectFactory::Instantiate(Scene& scene)
	{
		size_t count = 0;
		for (GameObject& gameObject : scene.gameObjects)
		{
			if (gameObject.name.find(defaultGameObjName) == 0)
				++count;
		}
		GameObject& tmp = scene.gameObjects.emplace_back();
		if (count)
			tmp.name += '(' + std::to_string(count) + ')';
		return tmp;
	}

	template<typename T, typename... Ts>
	struct CloneComponentsHelper
	{
		GameObject& dest;
		GameObject& src;
		Scene& scene;
		bool copyID;
		constexpr CloneComponentsHelper(TemplatePack<T, Ts...> pack) {};
		CloneComponentsHelper(GameObject& _dest, GameObject& _src, Scene& _scene, bool _copyID) :
			dest{_dest},src{_src},scene{_scene},copyID{_copyID}
		{ CloneComponents<T,Ts...>(); }
		template <typename T1, typename... T1s>
		void CloneComponents()
		{
			for (T1* pComponent : src.GetComponents<T1>())
			{
				MyGOF.AddComponent(dest, scene, pComponent, copyID);
			}
			if constexpr (sizeof...(T1s) != 0)
			{
				return CloneComponents<T1s...>();
			}
		}
	};

	using CloneAllComponents = decltype(CloneComponentsHelper(ComponentTypes()));

	void CloneComponents(GameObject& _dest, GameObject& _src, Scene& _scene, bool _copyID = false)
	{
		CloneAllComponents clone{_dest,_src,_scene,_copyID};
		return;
	}

	GameObject& GameObjectFactory::Instantiate(GameObject& _src, Scene& scene, bool copyID)
	{
		size_t count = 0;
		for (GameObject& gameObject : scene.gameObjects)
		{
			if (gameObject.name.find(_src.name) == 0)
				++count;
		}
		if (copyID)
		{
			GameObject& tmp = scene.gameObjects.emplace_back(_src,_src.uuid);
			if (count)
				tmp.name += '(' + std::to_string(count) + ')';
			CloneComponents(tmp,_src,scene,copyID);
			for (Transform* pChild : _src.transform.children)
			{
				Instantiate(pChild->gameObject, scene, copyID).transform.SetParent(&tmp.transform);
			}
			return tmp;
		}
		GameObject& tmp = scene.gameObjects.emplace_back(_src);
		if (count)
			tmp.name += '(' + std::to_string(count) + ')';
		CloneComponents(tmp, _src, scene, copyID);
		for (Transform* pChild : _src.transform.children)
		{
			Instantiate(pChild->gameObject, scene, copyID).transform.SetParent(&tmp.transform);
		}
		return tmp;
	}

	GameObject& GameObjectFactory::Instantiate(rapidjson::Value& _value, Scene& scene)
	{
		if (_value.HasMember("ID"))
		{
			GameObject& go = scene.gameObjects.emplace_back(_value["ID"].GetUint64());
			Serializer::Deserialize(go, "", _value);
			return go;
		}
		GameObject& go = scene.gameObjects.emplace_back();
		Serializer::Deserialize(go, "", _value);
		return go;
	}

	void GameObjectFactory::Destroy(GameObject& _go, GameObjectsArray& gameObjectArray)
	{
		_go.transform.SetParent(nullptr);
		for (Transform* pTransform : _go.transform.children)
		{
			pTransform->SetParent(nullptr);
			Destroy(pTransform->gameObject, gameObjectArray);
		}
		gameObjectArray.erase(_go);
	}


	void GameObjectFactory::Destroy(UUID id, GameObjectsArray& gameObjectArray)
	{
		GameObjectsArray::Iterator it = gameObjectArray.begin();
		while (it != gameObjectArray.end())
		{
			GameObject& gameObject = (*it);
			if (gameObject.uuid == id)
			{
				gameObject.transform.SetParent(nullptr);
				gameObjectArray.erase(it);
				//It is safe because the object is not destroyed
				for (Transform* pTransform : gameObject.transform.children)
				{
					pTransform->SetParent(nullptr);
					Destroy(pTransform->gameObject, gameObjectArray);

				}
				return;
			}
			++it;
		}
	}

	std::map<std::string, GameObject*>& GameObjectFactory::get_archetype_map()
	{
		return archetypes;
	}

	GameObject& GameObjectFactory::InstantiateChild(GameObject& _parent, Scene& _scene)
	{
		GameObject& child = _scene.gameObjects.emplace_back();
		child.transform.SetParent(&_parent.transform);

		return child;

	}

}
