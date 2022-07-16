/*!***************************************************************************************
****
\file			inspector.h
\project
\author			Zacharie Hong
\co-authors		// DELETE IF NOT APPLICABLE

\par			Course: GAM200
\par			Section:
\date			16/07/2022

\brief
	// BRIEF DESCRIPTION OF FILE PURPOSE

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/

#pragma once

#ifndef HEADER_INSPECTOR

#define HEADER_INSPECTOR


//INCLUDES

#include <imgui.h>

#include "game-object.h"

namespace Window
{
	namespace Inspector
	{
		extern GameObject* selectedGameObject;
		void init();
		void update();
	}
}


#endif // !HEADER_INSPECTOR