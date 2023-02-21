/*!***************************************************************************************
\file			scene.h
\project
\author			Matthew Lau

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

	Contains definitions for NormalScene class which is a derived class from Scene class.
	Note: this is the latest version of our scene class, use this


All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <GameObject/game-object.h>
#include <memory>
#include <GameObject/components.h>

namespace Copium
{

class SceneManager;

class Scene 
{
public:
	GameObjectsArray gameObjects;	//Vector should be in order
	ComponentsArrays componentArrays;
	GameObjectsPtrArray gameObjectsForDeletion;
	ComponentsPtrArrays componentsForDeletion;

	std::vector<std::shared_ptr<GameObject>> gameObjectSPTRS;

	enum class SceneState : char {
		edit = 0, 
		play,
		paused
	};


	Scene();
	Scene(const std::string& _filepath);

	Scene(Scene&) = delete;
	Scene& operator=(Scene&) = delete;
	std::string name;
	virtual ~Scene();

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
		Adds a GameObject to the scene

	\param	_gameObj
		pointer to the GameObject that is to be added to the scene

	\return
		pointer to the newly added GameObject
	*/
	/*******************************************************************************/
	GameObject* add_gameobject(GameObject* _gameObj);

	/*******************************************************************************
	/*!
	*
	\brief
		Sets the name of this scene

	\param	_name
		read-only reference to a string containing the new name for this scene

	\return
		void
	*/
	/*******************************************************************************/
	void set_name(const std::string& _name);
	/*******************************************************************************
	/*!
	*
	\brief
		Gets the name of this scene

	\return
		string containing the 
	*/
	/*******************************************************************************/
	std::string get_name() const;

	void inspector_view();
	/*******************************************************************************
	/*!
	*
	\brief
		Gets the current scene state

	\return
		the current scene state
	*/
	/*******************************************************************************/
	SceneState get_state() const;
	/*******************************************************************************
	/*!
	*
	\brief
		Set the current scene state to specified scene state

	\param _newState
		the new scene state to set the current scene state to 

	\return
		void
	*/
	/*******************************************************************************/
	void set_state(SceneState _newState);

private:
	const std::string filename;
	SceneState currSceneState;
	friend class SceneManager;

};

class NormalScene : public Scene
{
public:
	NormalScene();
	NormalScene(const std::string& _filepath);
	/*******************************************************************************
	/*!
	*
	\brief
		LOAD function for a NormalScene

	\return
		void
	*/
	/*******************************************************************************/
	void load_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		INIT function for a NormalScene

	\return
		void
	*/
	/*******************************************************************************/
	void init_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		UPDATE function for a NormalScene

	\return
		void
	*/
	/*******************************************************************************/
	void update_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		DRAW function for a NormalScene

	\return
		void
	*/
	/*******************************************************************************/
	void draw_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		FREE function for a NormalScene

	\return
		void
	*/
	/*******************************************************************************/
	void free_scene();
	/*******************************************************************************
	/*!
	*
	\brief
		UNLOAD function for a NormalScene

	\return
		void
	*/
	/*******************************************************************************/
	void unload_scene();



};
}
#endif SCENE_H