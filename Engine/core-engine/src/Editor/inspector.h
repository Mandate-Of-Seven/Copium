/*!***************************************************************************************
\file			inspector.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
	Declares Window::Inspector namespace to have ImGui functions for displaying and
	interacting with the window.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once

#ifndef INSPECTOR_H

#define INSPECTOR_H

//INCLUDES

#include <imgui.h>
#include "SceneManager/sm.h"
#include "GameObject/game-object.h"

namespace Window
{
	namespace Inspector
	{
		extern GameObject* selectedGameObject;

		/***************************************************************************/
		/*!
		\brief
			Initialize function called at the start of program to reset fields
		*/
		/**************************************************************************/
		void init();
		/***************************************************************************/
		/*!
		\brief
			Update function called every frame for ImGui window
		*/
		/**************************************************************************/
		void update();
		/***************************************************************************/
		/*!
		\brief
			Exit function called at end of program
		*/
		/**************************************************************************/
		void exit();
	}
}


#endif // !INSPECTOR_H