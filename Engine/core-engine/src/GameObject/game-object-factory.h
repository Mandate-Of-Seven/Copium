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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H
#include "CopiumCore/system-interface.h"
#include "GameObject/game-object.h"
#include "SceneManager/scene.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Copium {

	class GameObjectCreator {

	};

	class Default : public GameObjectCreator
	{

	};
	

	class GameObjectFactory {
		
	public:
		GameObjectFactory();
		~GameObjectFactory();

		/*******************************************************************************
		/*!
		*
		\brief
			Builds a game object with default data

		\return
			pointer to the new game object
		*/
		/*******************************************************************************/
		GameObject* build_gameobject();

		/*******************************************************************************
		/*!
		*
		\brief
			Build a game object that is a copy of specified game object.
			Note: if the specified game object has a family tree, the whole tree is duplicated.

		\return
			pointer to the new game object (head of the tree)
		*/
		/*******************************************************************************/
		GameObject* build_gameobject(GameObject& _src);
		// Set up for future
		//GameObject* build_gameobject(prefab);

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
		GameObject* build_gameobject(rapidjson::Value& _value);
		/*******************************************************************************
		/*!
		*
		\brief
			Link this GameObjectFactory to a Scene object
			Note: whenever a new scene is loaded in, this function should be called by the SceneManager that
			owns this GameObjectFactory

		\param _s
			Ptr to the Scene that this GameObjectFactory needs to be linked to
			
		\return
			pointer to the new game object
		*/
		/*******************************************************************************/
		//void link_to_scene(Scene* _s);
		/*******************************************************************************
		/*!
		*
		\brief
			Destroy a gameobject

		\param _go
			Ptr to the game object that is to be destroyed.
			Note: the game object must exist in the current scene.

		\return
			on success, return true
			on failure, return false
		*/
		/*******************************************************************************/
		bool delete_gameobject(GameObject* _go);
		/*******************************************************************************
		/*!
		*
		\brief
			Clones a game object and places the clone into the current scene
			NOTE: This function does NOT clone the children

		\param _src
			Ptr to a game object that is to be cloned

		\return
			pointer to the new game object(clone)
		*/
		/*******************************************************************************/
		GameObject* clone_gameobject(GameObject* _src);

		/*******************************************************************************
		/*!
		*
		\brief
			Adds a component to the specified game object

		\param _key
			read-only reference to a string that contains the key that will determine what component to add

		\param _go
			pointer to the game object that the component is to be attached to

		\return
			if successful, return true
			if specified key does not exist, return false
			if there is an error in creation of component, return false

		*/
		/*******************************************************************************/
		bool add_component(const std::string& _key, GameObject* _go);
		/*******************************************************************************
		/*!
		*
		\brief
			Build an archetype and place it into the gameObjectCreators map

		\param _value
			reference to the rapidjson value which should contain the archetype game object

		\return
			if successful, return pointer to the newly created game object
			if there were any errors in the process, return nullptr

		*/
		/*******************************************************************************/
		GameObject* build_archetype(rapidjson::Value& _value);

		/*******************************************************************************
		/*!
		*
		\brief
			Build a game object using an archetype registered to the GOF

		\param _archetype
			read-only reference to a string which contains the key of the archetype that is to be created

		\return
			if successful, return ptr to the newly created game object
			if there were any errors in the process, return nullptr

		*/
		/*******************************************************************************/
		GameObject* build_gameobject(const std::string& _archetype);

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
			Get reference of the archetype map

		\return
			reference to the archetype map

		*/
		/*******************************************************************************/
		std::map<std::string, GameObject*>& get_archetype_map();

	private:
		std::map<std::string, GameObject*> gameObjectCreators;
		friend GameObject;
	};

}



#endif
