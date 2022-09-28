/*!***************************************************************************************
****
\file			scenemanager.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains declarations for the Scene Manager class.
	The Scene Manager is able to manage the different scenes assigned to it, allowing for:
		1. Transitioning between scenes
		2. Calling of load, init, update, draw, free and unload state functions for each scene
		3. Assigning of new scenes to itself

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include "gamestate-sandbox.h"
#include "scene.h"
#include <system-interface.h>

//GAMESTATE ID - int type
enum GameState 
{
	gsQuit = -2,
	gsRestart = -1,	
};

class SceneManager 
{
public:
	SceneManager();
	~SceneManager();
	/*******************************************************************************
	/*!
	*
	\brief
		Add a new scene to the scene manager

	\param _newScene
		Pointer to the scene to be added

	\return
		void
	*/
	/*******************************************************************************/
	void add_scene(Scene* _newScene);
	/*******************************************************************************
	/*!
	*
	\brief
		Change to a different scene whose ID is specified as a param

	\param _id
		ID of the scene that is to be changed to

	\return
		if change is successful, return true
		if change is unsuccessful, return false
	*/
	/*******************************************************************************/
	bool change_scene(int _id);
	/*******************************************************************************
	/*!
	*
	\brief
		Access the current active scene

	\return
		Pointer to the current active scene
	*/
	/*******************************************************************************/
	Scene* get_currentscene();
	/*******************************************************************************
	/*!
	*
	\brief
		Access the number of scenes assigned to this scene manager

	\return
		Number of scenes assigned to this scene manager
	*/
	/*******************************************************************************/
	int get_scenecount() const;
	
	/*******************************************************************************
	/*!
	*
	\brief
		Calls the LOAD function of the current scene

	\return
		void
	*/
	/*******************************************************************************/
	void load_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		Calls the INIT function of the current scene

	\return
		void
	*/
	/*******************************************************************************/
	void init_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		Calls the UPDATE function of the current scene

	\return
		void
	*/
	/*******************************************************************************/
	void update_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		Calls the DRAW function of the current scene

	\return
		void
	*/
	/*******************************************************************************/
	void draw_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		Calls the FREE function of the current scene

	\return
		void
	*/
	/*******************************************************************************/
	void free_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		Calls the UNLOAD function of the current scene

	\return
		void
	*/
	/*******************************************************************************/
	void unload_scene();

	int current, next, previous;

private:
	int numberOfScenes;
	std::vector<Scene*> scenes;
	Scene* currentScene;
};






#endif // !SCENEMANAGER_H
