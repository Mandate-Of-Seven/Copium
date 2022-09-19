/*!***************************************************************************************
****
\file			game-object-factory.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
	Contains declarations for the GameObjectFactory class.
	The GameObjectFactory is responsible for the creation of game objects. These game objects are then linked
	to their parent scenes.
	Note: GameObjectFactory inherits from ISystem class

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H
#include <system-interface.h>
#include <game-object.h>
#include <scene.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Copium {

	CLASS_SYSTEM(GameObjectFactory) {
		
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

		// Do nothing
		void init();
		void update();
		void exit();

	private:
		Scene* currentScene;


	};



}



#endif
