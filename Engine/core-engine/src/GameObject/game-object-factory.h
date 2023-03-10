/*!***************************************************************************************
\file			game-object-factory.h
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
	Contains declarations for the GameObjectFactory class.
	The GameObjectFactory is responsible for the creation, management and destruction of game objects.
	Note: The GameObjectFactory will also place the created gameobjects inside the current scene.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H
#include <CopiumCore/system-interface.h>
#include <Events/events-system.h>

#define MyGOF (*Copium::GameObjectFactory::Instance())

namespace Copium {

	class GameObjectCreator {

	};

	class Default : public GameObjectCreator
	{

	};
	

	class GameObjectFactory : public Singleton<GameObjectFactory>
	{
		
	public:
		/*******************************************************************************
		/*!
		*
		\brief
			Builds a game object with default data

		\return
			pointer to the new game object
		*/
		/*******************************************************************************/
		GameObject& Instantiate(Scene& scene);

		/*******************************************************************************
		/*!
		*
		\brief
			ASSIGNS new ID and builds a game object that is a copy of specified game object.
			Note: if the specified game object has a family tree, the whole tree is duplicated.

		\return
			pointer to the new game object (head of the tree)
		*/
		/*******************************************************************************/
		GameObject& Instantiate(GameObject& _src, Scene& scene, bool copyID = false);

		// Set up for future
		//GameObject* instantiate(prefab);

		/*******************************************************************************
		/*!
		*
		\brief
			Builds a game object from a rapidjson::Value
			Note: Value provided should be a game object value

		\return
			pointer to the new game object
		*/
		/*******************************************************************************/
		GameObject& Instantiate(rapidjson::Value& _value, Scene& scene);

		template <typename T>
		T& AddComponent(GameObject& gameObject, Scene& scene, UUID uuid = UUID(),T* pCopy = nullptr);

		Text& AddComponent(GameObject& gameObj, Scene& scene, bool inspector, UUID uuid = UUID() , Text* pCopy = nullptr);
		
		Script& AddComponent(GameObject& gameObj, Scene& scene, const char* scriptName, UUID uuid = UUID() , Script* pCopy = nullptr);

		/*******************************************************************************
		/*!
		*
		\brief
			Destroy a gameobject

		\param _go
			Ptr to the game object that is to be destroyed.
			Note: the game object must exist in the current scene.
		*/
		/*******************************************************************************/
		void Destroy(GameObject& _go, GameObjectsArray& gameObjectArray, bool flag);


		template <typename T>
		void RemoveComponent(T& component, Scene& scene)
		{
			auto it = component.gameObj.componentPtrArrays.GetArray<T>().begin();
			auto end = component.gameObj.componentPtrArrays.GetArray<T>().end();
			while (it != end)
			{
				if (component.uuid == (*it)->uuid)
				{
					component.gameObj.componentPtrArrays.GetArray<T>().erase(it);
					PRINT("REMOVED FROM GAMEOBJECT");
					break;
				}
				++it;
			}
			int i{ 0 };
			auto sceneIt = scene.componentArrays.GetArray<T>().begin();
			auto sceneEnd = scene.componentArrays.GetArray<T>().end();
			while (sceneIt != sceneEnd)
			{
				if (component.uuid == (*sceneIt).uuid)
				{
					scene.componentArrays.GetArray<T>().erase(sceneIt);
					PRINT("REMOVED FROM SCENE");
					break;
				}
				++sceneIt;
				PRINT(i);
			}
		}

		/*******************************************************************************
		/*!
		*
		\brief
			Destroy a gameobject by ID

		\param id
			ID of gameobject to be destroyed
		*/
		/*******************************************************************************/
		void Destroy(UUID uuid, GameObjectsArray& gameObjectArray);

		/*******************************************************************************
		/*!
		*
		\brief
			Iterates through the filepath that is specified and creates an archetype game object for each archetype json found in the folder.
			Note: user must guarantee that the path provided is correct and that the files contain the correct format for a single game object

		\param _directoryPath
			read-only reference to the file path of the directory containing all the archetype files

		\return
			if successful, return true
			if there were any errors in the process, return false

		*/
		/*******************************************************************************/
		bool register_archetypes(const std::filesystem::path& _directoryPath);

		/*******************************************************************************
		/*!
		*
		\brief
			Clear the memory allocated to the Original Archetype templates

		\return
			void
		*/
		/*******************************************************************************/
		void clear_archetypes();

		/*******************************************************************************
		/*!
		*
		\brief
			Get reference of the archetype map

		\return
			reference to the archetype map

		*/
		/*******************************************************************************/
		std::map<std::string, GameObject*>& get_archetype_map();

		//M3

		/*******************************************************************************
		/*!
		*
		\brief
			Create a default child game object and attach it to the specified game object which will be the parent

		\param _parent
			reference to the parent of the newly created child game object

		\return
			ptr to thew newly created child game object

		*/
		/*******************************************************************************/
		GameObject& InstantiateChild(GameObject& _parent, Scene& _scene);


	private:
		std::map<std::string, GameObject*> archetypes;
	};

	
}


#endif
