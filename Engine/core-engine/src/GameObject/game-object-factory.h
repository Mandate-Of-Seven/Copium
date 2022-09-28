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
		void link_to_scene(Scene* _s);
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

	private:
		Scene* currentScene;

	};



}



#endif
