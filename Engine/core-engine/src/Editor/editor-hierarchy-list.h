#pragma once

#ifndef SCENE_HIERARCHY_LIST_H
#define SCENE_HIERARCHY_LIST_H
#include <imgui.h>
#include "SceneManager/sm.h"
#include "GameObject/game-object.h"

namespace Window::Hierarchy
{


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
	void display_gameobject(const GameObject& _go);

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
	bool display_gameobject_advanced(GameObject& _go, GameObjectID& _selected);

	bool create_gameobject_btn(const std::string& _btnName);
	bool create_delete_btn(const std::string& _btnName);
}
#endif
