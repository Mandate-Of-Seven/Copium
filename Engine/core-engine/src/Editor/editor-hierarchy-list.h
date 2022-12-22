#pragma once
/*!***************************************************************************************
\file			editor-hierarchy-list.h
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			27/10/2022

\brief
	Declaration of functions that handle initialization and update of Hierarchy List

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SCENE_HIERARCHY_LIST_H
#define SCENE_HIERARCHY_LIST_H
#include <imgui.h>
#include "SceneManager/scene-manager.h"
#include "GameObject/game-object.h"

namespace Window::Hierarchy
{
	inline bool isHierarchyOpen;

	/*******************************************************************************
	/*!
	*
	\brief
		Set-up necessary values for drawing of Hierarchy window. 

	\return
		void
	*/
	/*******************************************************************************/
	void init();

	/*******************************************************************************
	/*!
	*
	\brief
		Draw an ImGui window which will store the Hierarchy list of Game Objects in the current scene.
		Uses ImGui Tree to list out the game objects.

	\return
		void
	*/
	/*******************************************************************************/
	void update();

	/*******************************************************************************
	/*!
	*
	\brief
		Create an ImGui Tree Node for the game object. 
		Note: this function will be called recursively for game object's children (if any)
		Note: game object's with no children will be drawn as leaves

	\param	_go
		read-only reference to the game object that is to be displayed

	\return
		void
	*/
	/*******************************************************************************/
	void display_gameobject(const Copium::GameObject& _go);

	/*******************************************************************************
	/*!
	*
	\brief
		Create an ImGui Tree Node for the game object. Allow for selection of game objects
		Note: this function will be called recursively for game object's children (if any)
		Note: game object's with no children will be drawn as leaves

	\param	_go
		reference to the game object that is to be displayed

	\param _selected
		reference to an int value which stores id of the game object that is selected (if any)
		Note: default value should be -1 if no game objects are selected

	\return
		if nothing is selected, return false
		if a game object is selected return true
	*/
	/*******************************************************************************/
	bool display_gameobject_advanced(Copium::GameObject& _go, Copium::GameObjectID& _selected);

	/*******************************************************************************
	/*!
	*
	\brief
		Create a button in the Hierarchy List window's menu bar that upon click will create a game object
		in the current scene

	\param	_btnName
		read-only reference to a string that contains the name of the button

	\return
		if successful, return true
		if there are errors in creation of the button, return false
	*/
	/*******************************************************************************/
	bool create_gameobject_btn(const std::string& _btnName);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a button in the Hierarchy List window's menu bar that upon click will delete the selected game object
		in the current scene

	\param	_btnName
		read-only reference to a string that contains the name of the button

	\return
		if successful, return true
		if there are errors in creation of the button, return false
	*/
	/*******************************************************************************/
	bool create_delete_btn(const std::string& _btnName);
}
#endif
