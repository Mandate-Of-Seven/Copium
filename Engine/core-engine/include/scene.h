/*!***************************************************************************************
****
\file			scene.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains declarations for the Scene class.
	The Scene contains:
		1. load, init, update, draw, free, unload function
		2. string containing the filename of the file in which the scene data is stored on
		3. Data pertaining to the game objects in the scene

	Note: load, init, free and unload functions MUST be defined by scene sub-classes

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <vector>
#include <game-object.h>

class Scene 
{
public:
	Scene(std::string& _filepath);

	Scene() = delete;
	Scene(Scene&) = delete;
	Scene& operator=(Scene&) = delete;
	~Scene();

	/*******************************************************************************
	/*!
	*
	\brief
		LOAD function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void load_scene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		INIT function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void init_scene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		UPDATE function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void update_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		DRAW function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void draw_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		FREE function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void free_scene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		UNLOAD function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void unload_scene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		Accessor for the filename of the file that contains this scene's data

	\return
		the filename of the file that contains this scene's data
	*/
	/*******************************************************************************/
	std::string get_filename() const;
	/*******************************************************************************
	/*!
	*
	\brief
		Sets the filename of the file that contains this scene's data

	\param	_newFilename
		the new filename to be set
		
	\return
		void
	*/
	/*******************************************************************************/
	void set_filename(std::string& _newFilename);
	/*******************************************************************************
	/*!
	*
	\brief
		Gets the number of game objects attached to the scene

	\return
		number of game objects attached to the scene
	*/
	/*******************************************************************************/
	size_t get_gameobjcount() const;
	/*******************************************************************************
	/*!
	*
	\brief
		Returns reference to the GameObject* vector

	\return
		reference to the GameObject* vector
	*/
	/*******************************************************************************/
	std::vector<GameObject*>& get_gameobjectvector();
	/*******************************************************************************
	/*!
	*
	\brief
		Adds a GameObject to the scene

	\param	_gameObj
		pointer to the GameObject that is to be added to the scene

	\return
		pointer to the newly added GameObject
	*/
	/*******************************************************************************/
	GameObject* add_gameobject(GameObject* _gameObj);

private:
	std::string& filename;
	std::vector<GameObject*> gameObjects;
};



#endif SCENE_H