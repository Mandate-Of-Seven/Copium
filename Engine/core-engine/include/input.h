/*!***************************************************************************************
\file			input.h
\project
\author			Shawn Tanary
\co-authors		// DELETE IF NOT APPLICABLE

\par			Course: GAM200
\par			Section:
\date			16/07/2022

\brief
	Contains functions to check if certain mouse or keyboard inputs are detected

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once
#include <utility> 

//Checks for mouse or keyboard inputs
class Input
{
	public:
		
		/***************************************************************************/
		/*!
		\brief
			Deletes the input system instance, CALLED AT END OF PROGRAM
		*/
		/**************************************************************************/
		static void destroy()
		{
			delete inputInstance;
		}

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
		static bool isKeyPressed(int keycode) { return inputInstance->isKeyPressedImpl(keycode); }

		/***************************************************************************/
		/*!
		\brief
			Checks if the given key is being held
		\param keycode
			the key to check for
		\return
			true if the key is held or false if not
		*/
		/**************************************************************************/
		static bool isKeyHeld(int keycode) { return inputInstance->isKeyHeldImpl(keycode); }


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
		static bool isMouseButtonPressed(int button) { return inputInstance->isMouseButtonPressedImpl(button); }

		/***************************************************************************/
		/*!
		\brief
			Returns the position of the mouse cursor based on the top left of the game window
		\return
			the mouse position
		*/
		/**************************************************************************/
		static std::pair<float,float> getMousePosition() { return inputInstance->getMousePositionImpl(); }

		/***************************************************************************/
		/*!
		\brief
			Returns the x coordinate of the mouse cursor
		\return
			x coordinate of the mouse cursor
		*/
		/**************************************************************************/
		static bool getMouseX() { return inputInstance->getMouseXImpl(); }
		/***************************************************************************/
		/*!
		\brief
			Returns the y coordinate of the mouse cursor
		\return
			y coordinate of the mouse cursor
		*/
		/**************************************************************************/
		static bool getMouseY() { return inputInstance->getMouseYImpl(); }
	protected:
		/***************************************************************************/
		/*!
		\brief
			The implementaion of isKeyPressed - Must be implemented by derived class
		/**************************************************************************/
		virtual bool isKeyPressedImpl(int keycode) = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of isKeyHeld - Must be implemented by derived class
		/**************************************************************************/
		virtual bool isKeyHeldImpl(int keycode) = 0;


		/***************************************************************************/
		/*!
		\brief
			The implementaion of isMouseButtonPressed - Must be implemented
													    by derived class
		/**************************************************************************/
		virtual bool isMouseButtonPressedImpl(int button) = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of getMousePosition - Must be implemented
													by derived class
		/**************************************************************************/
		virtual std::pair<float, float> getMousePositionImpl() = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of getMouseX - Must be implemented
											 by derived class
		/**************************************************************************/
		virtual float getMouseXImpl() = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of getMouseY - Must be implemented
											 by derived class
		/**************************************************************************/
		virtual float getMouseYImpl() = 0;
	private:
		static Input* inputInstance;
};