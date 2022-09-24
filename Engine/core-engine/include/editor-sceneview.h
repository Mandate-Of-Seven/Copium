/*!***************************************************************************************
\file			editor-sceneview.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the declaration of functions for editor-sceneview.cpp.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef EDITOR_SCENEVIEW_H
#define EDITOR_SCENEVIEW_H

#include "editor-system.h"

namespace Copium::Editor
{
	namespace SceneView
	{
		void init();

		void update();

		void exit();
	};

}
#endif // !EDITOR_SCENEVIEW_H
