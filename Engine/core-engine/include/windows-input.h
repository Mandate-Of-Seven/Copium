/*!***************************************************************************************
\file			windows-input.h
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	Contains functions to check if certain mouse or keyboard inputs are detected

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#pragma once
#include "input.h"

class WindowsInput : public Input
{
	protected:
		/***************************************************************************/
		/*!
		\brief
			Checks if the given key is being pressed
		\param keycode
			the key to check for
		\return
			true if the key is pressed or false if not
		*/
		/**************************************************************************/
		virtual bool is_key_pressed_impl(int keycode) override;

		/***************************************************************************/
		/*!
		\brief
			Checks if the given key is being held (DOESNT WORK YET)
		\param keycode
			the key to check for
		\return
			true if the key is held or false if not
		*/
		/**************************************************************************/
		virtual bool is_key_held_impl(int keycode) override;


		/***************************************************************************/
		/*!
		\brief
			Checks if the given mouse button is being pressed
		\param button
			the mouse button to check for
		\return
			true if the key is pressed or false if not
		*/
		/**************************************************************************/
		virtual bool is_mousebutton_pressed_impl(int button) override;

		/***************************************************************************/
		/*!
		\brief
			Returns the position of the mouse cursor based on the top left of the game window
		\return
			the mouse position
		*/
		/**************************************************************************/
		virtual std::pair<float, float> get_mouseposition_impl() override;

		/***************************************************************************/
		/*!
		\brief
			Returns the x coordinate of the mouse cursor
		\return
			x coordinate of the mouse cursor
		*/
		/**************************************************************************/
		virtual float get_mouseX_impl() override;

		/***************************************************************************/
		/*!
		\brief
			Returns the y coordinate of the mouse cursor
		\return
			y coordinate of the mouse cursor
		*/
		/**************************************************************************/
		virtual float get_mouseY_impl() override;

	public:
		/***************************************************************************/
		/*!
		\brief
			Initialises all the needed variables
		*/
		/**************************************************************************/
		void init();
			
};