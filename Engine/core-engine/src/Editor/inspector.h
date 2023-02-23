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
#include "SceneManager/scene-manager.h"
#include "GameObject/game-object.h"
#include "Audio/sound-system.h"

namespace Copium
{
	class EditorInspector
	{
	public:
		void init();
		void update();
		void exit();
		void AlignforWidth(float width, float alignment = 0.5f);

		bool& getFocused() { return isFocused; }
		bool& status() { return isInspectorOpen; }
	private:
		bool isInspectorOpen;
		bool isFocused;
		UUID targetGameobjectName;

		void CallbackStartPreview(StartPreviewEvent* pEvent);
		void CallbackStopPreview(StopPreviewEvent* pEvent);
	};
}


#endif // !INSPECTOR_H