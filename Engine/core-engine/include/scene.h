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
******************************************************************************************
****/
#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>

class Scene {
public:
	Scene(std::string& _filepath);
	/*******************************************************************************
	/*!
	*
	\brief
		LOAD function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void loadScene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		INIT function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void initScene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		UPDATE function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void updateScene();
	/*******************************************************************************
	/*!
	*
	\brief
		DRAW function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void drawScene();
	/*******************************************************************************
	/*!
	*
	\brief
		FREE function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void freeScene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		UNLOAD function for the scene

	\return
		void
	*/
	/*******************************************************************************/
	virtual void unloadScene() = 0;
	/*******************************************************************************
	/*!
	*
	\brief
		Accessor for the filename of the file that contains this scene's data

	\return
		the filename of the file that contains this scene's data
	*/
	/*******************************************************************************/
	std::string getFilename();
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
	void setFilename(std::string& _newFilename);
private:
	std::string& filename;
	//add vector of game objs here
};



#endif SCENE_H