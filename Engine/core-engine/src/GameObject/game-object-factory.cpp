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
	template <typename T>
	T& GameObjectFactory::AddComponent(GameObject& gameObj, Scene& scene, UUID uuid,T* pCopy)
	{
		if (pCopy)
		{
			T& component = scene.componentArrays.GetArray<T>().emplace_back(gameObj, *pCopy , uuid);
			gameObj.AddComponent(&component);
			MyEventSystem->publish(new ReflectComponentEvent(component));
			return component;
		}
		T& component = scene.componentArrays.GetArray<T>().emplace_back(gameObj, uuid);
		gameObj.AddComponent(&component);
		MyEventSystem->publish(new ReflectComponentEvent(component));
		return component;
	}

	Text& GameObjectFactory::AddComponent(GameObject& gameObj, Scene& scene, bool inspector, UUID uuid, Text* pCopy)
	{
		if (pCopy)
		{
			Text& component = scene.componentArrays.GetArray<Text>().emplace_back(gameObj, *pCopy, uuid);
			gameObj.AddComponent(&component);
			MyEventSystem->publish(new ReflectComponentEvent(component));
			return component;
		}
		Text& component = scene.componentArrays.GetArray<Text>().emplace_back(gameObj, uuid, inspector);
		gameObj.AddComponent(&component);
		MyEventSystem->publish(new ReflectComponentEvent(component));
		return component;
	}

	Script& GameObjectFactory::AddComponent(GameObject& gameObj, Scene& scene, const char* scriptName , UUID uuid ,Script* pCopy)
	{
		if (pCopy)
		{
			Script& component = scene.componentArrays.GetArray<Script>().emplace_back(gameObj, *pCopy, uuid);
			gameObj.AddComponent(&component);
			MyEventSystem->publish(new ReflectComponentEvent(component));
			return component;
		}
		Script& component = scene.componentArrays.GetArray<Script>().emplace_back(gameObj, uuid, scriptName);
		gameObj.AddComponent(&component);
		MyEventSystem->publish(new ReflectComponentEvent(component));
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
			//ASSIGN THE SAME UUID AS THE ORIGINAL COPY
			if (copyID)
			{
				for (T1* pComponent : src.GetComponents<T1>())
				{
					MyGOF.AddComponent(dest, scene, pComponent->uuid, pComponent);
				}
			}
			//GENERATE NEW UUID FOR CLONED COMPONENTS
			else
			{
				for (T1* pComponent : src.GetComponents<T1>())
				{
					MyGOF.AddComponent(dest, scene, UUID(), pComponent);
				}
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
			size_t childCount = 0;
			for (Transform* pChild : _src.transform.children)
			{
				Instantiate(pChild->gameObject, scene, copyID).transform.SetParent(&tmp.transform);
				++childCount;
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

	void GameObjectFactory::Destroy(GameObject& _go, GameObjectsArray& gameObjectArray, bool flag)
	{
		PRINT("No of game objects before: " << gameObjectArray.size());
		for (Transform* pTransform : _go.transform.children)
		{
			Destroy(pTransform->gameObject, gameObjectArray, false);
		}
		gameObjectArray.erase(_go);

		if (flag && _go.transform.HasParent())
		{
			GameObject& parent = _go.transform.parent->gameObject;

			parent.transform.children.remove(&_go.transform);
		}

		PRINT("No of game objects after: " << gameObjectArray.size());

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
					Destroy(pTransform->gameObject, gameObjectArray, false);

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
