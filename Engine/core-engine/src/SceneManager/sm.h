/*!***************************************************************************************
****
\file			sm.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	Contains declarations for the NewSceneManager class.
	NewSceneManager is responsible for managing the current scene in the engine. 
	It is also in charge of:
	1. loading in scene data from a file
	2. switching to another scene
	3. de-allocation of resources used by current scene (cleanup before engine close)
	4. Calling scene's update functions

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#pragma once
#ifndef SM_H
#define SM_H


#include "GameObject/game-object-factory.h"
#include "CopiumCore/system-interface.h"
#include "SceneManager/scene.h"
#include <filesystem>

namespace Copium {

	CLASS_SYSTEM(NewSceneManager)
	{
	public:
		NewSceneManager();
		~NewSceneManager();

		/*******************************************************************************
		/*!
		*
		\brief
			The Initialization function for NewSceneManager ISystem

		\return
			void
		*/
		/*******************************************************************************/
		void init();
		/*******************************************************************************
		/*!
		*
		\brief
			The Update function for NewSceneManager ISystem

		\return
			void
		*/
		/*******************************************************************************/
		void update();
		/*******************************************************************************
		/*!
		*
		\brief
			The Exit function for NewSceneManager ISystem.
			Should do the cleanup for any resources used by this ISystem

		\return
			void
		*/
		/*******************************************************************************/
		void exit();
		/*******************************************************************************
		/*!
		*
		\brief
			Load scene data from a file

		\param _filepath
			reference to a string containing the filepath of the file that contains the scene data to be loaded.

		\return
			if loading of scene data is successful, return true
			if there are errors with loading scene data, return false
		*/
		/*******************************************************************************/
		bool load_scene(const std::string& _filepath);
		/*******************************************************************************
		/*!
		*
		\brief
			Switch to another scene, whose data is in a file specified by input param

		\param _newfilepath
			reference to a string containing the filepath of the file that contains the scene data of the new scene.

		\return
			on success, return true
			on failure, return false
		*/
		/*******************************************************************************/
		bool change_scene(const std::string& _newfilepath);

		/*******************************************************************************
		/*!
		*
		\brief
			Get reference to the GOF in the SM

		\return
			reference to the GOF
		*/
		/*******************************************************************************/
		GameObjectFactory& get_gof();

		/*******************************************************************************
		/*!
		*
		\brief
			Get pointer to the current scene

		\return
			pointer to the current scene
			if there is no scene loaded, return nullptr
		*/
		/*******************************************************************************/
		Scene* get_current_scene();

		/*******************************************************************************
		/*!
		*
		\brief
			Set the selected game object

		\param _go
			ptr to the game object that is selected

		\return
			void
		*/
		/*******************************************************************************/
		void set_selected_gameobject(GameObject * _go);
		/*******************************************************************************
		/*!
		*
		\brief
			Get pointer to the selected game object (if any)

		\return
			pointer to the selected game object
			if there is no game object selected, return nullptr
		*/
		/*******************************************************************************/
		GameObject* get_selected_gameobject();

		GameObject* selectedGameObject;

	private:
		GameObjectFactory* gof;
		Scene* currentScene;
		rapidjson::Document document;
	};


}

#endif
