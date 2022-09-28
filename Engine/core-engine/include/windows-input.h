/*!***************************************************************************************
\file			windows-input.h
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			16/07/2022

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
		virtual bool isKeyPressedImpl(int keycode) override;

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
		virtual bool isKeyHeldImpl(int keycode) override;


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
		virtual bool isMouseButtonPressedImpl(int button) override;

		/***************************************************************************/
		/*!
		\brief
			Returns the position of the mouse cursor based on the top left of the game window
		\return
			the mouse position
		*/
		/**************************************************************************/
		virtual std::pair<float, float> getMousePositionImpl() override;

		/***************************************************************************/
		/*!
		\brief
			Returns the x coordinate of the mouse cursor
		\return
			x coordinate of the mouse cursor
		*/
		/**************************************************************************/
		virtual float getMouseXImpl() override;

		/***************************************************************************/
		/*!
		\brief
			Returns the y coordinate of the mouse cursor
		\return
			y coordinate of the mouse cursor
		*/
		/**************************************************************************/
		virtual float getMouseYImpl() override;

		
		~WindowsInput();

		public:
			void Init();
			
};