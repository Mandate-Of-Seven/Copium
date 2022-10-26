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
#include "CopiumCore/system-interface.h"

namespace Copium::Math 
{
	struct Vec2;
}

namespace Copium
{
	CLASS_SYSTEM(InputSystem)
	{
		public:
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
			bool is_key_pressed(int keycode);

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
			bool is_key_held(int keycode);

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
			bool is_mousebutton_pressed(int button);

			/***************************************************************************/
			/*!
			\brief
				Returns the position of the mouse cursor based on the top left of the game window
			\return
				the mouse position
			*/
			/**************************************************************************/
			Copium::Math::Vec2 get_mouseposition();

			/***************************************************************************/
			/*!
			\brief
				Returns the x coordinate of the mouse cursor
			\return
				x coordinate of the mouse cursor
			*/
			/**************************************************************************/
			float get_mouseX();

			/***************************************************************************/
			/*!
			\brief
				Returns the y coordinate of the mouse cursor
			\return
				y coordinate of the mouse cursor
			*/
			/**************************************************************************/
			float get_mouseY();

			/***************************************************************************/
			/*!
			\brief
				Returns the y offset of the mouse scroll
			\return
				 y offset of the mouse scroll
			*/
			/**************************************************************************/
			double get_mousescroll();

			/***************************************************************************/
			/*!
			\brief
				Initialises all the needed variables
			*/
			/**************************************************************************/
			void init();

			void update();

			void exit();

			static void mousescroll_callback(GLFWwindow * window, double xOffset, double yOffset);
			static void mousepos_callback(GLFWwindow * window, double xPos, double yPos);
			static void mousebutton_callback(GLFWwindow * window, int button, int action, int mods);
			static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	};
}