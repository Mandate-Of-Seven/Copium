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

namespace Copium
{
	class EditorHierarchyList
	{
	public:

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
			Cleanup duty

		\return
			void
		*/
		/*******************************************************************************/
		void exit();
		/*******************************************************************************
		/*!
		*
		\brief
			Create an ImGui Tree Node for the game object. Allow for selection of game objects.
			Applies any reordering to the root vector
			Note: this function will be called recursively for game object's children (if any)
			Note: game object's with no children will be drawn as leaves

		\param	_go
			reference to the game object that is to be displayed

		\param _selected
			reference to an int value which stores id of the game object that is selected (if any)
			Note: default value should be -1 if no game objects are selected

		\param _vector
			reference to a std::vector of GameObject* that contains all the root game objects

		\param _index
			_go's index in the root vector

		\return
			if nothing is selected, return false
			if a game object is selected return true
		*/
		/*******************************************************************************/
		bool display_gameobject(GameObject& _go, GameObjectID& _selected, std::vector<GameObject*>& _vector, int _index);
		/*******************************************************************************
		/*!
		*
		\brief
			Create an ImGui Tree Node for the game object. Allow for selection of game objects.
			Applies any reordering to the container specified.
			Note: this function will be called recursively for game object's children (if any)
			Note: game object's with no children will be drawn as leaves
			Note: this overload is meant for game objects with parents.

		\param	_go
			reference to the game object that is to be displayed

		\param _selected
			reference to an int value which stores id of the game object that is selected (if any)
			Note: default value should be -1 if no game objects are selected

		\param _list
			reference to a std::list of Transform* which is the list that contains _go and its siblings

		\param _index
			_go's index in its list that will be used for reordering

		\return
			if nothing is selected, return false
			if a game object is selected return true
		*/
		/*******************************************************************************/
		bool display_gameobject(GameObject& _go, GameObjectID& _selected, std::list<Transform*>& _list, int _index);
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
		bool& status() { return isHierarchyOpen; }

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
		void ShiftUp();

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
		void ShiftDown();

		bool DisplayAdoptionWindow();
		int DisplayOptionsPopUpWindow();
		int DisplayHierarchyOptionsPopUpWindow();

		void Reorder(GameObject* _go);

	private:
		GameObjectID selectedID;
		Scene* currentScene;
		bool isHierarchyOpen;
		bool isParenting;
		bool isPopUpOpen;
	};
}

#endif
