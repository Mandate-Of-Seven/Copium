/*!***************************************************************************************
\file			editor-system.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the declaration of functions for editor-system.cpp.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef EDITOR_SYSTEM_H
#define EDITOR_SYSTEM_H

#include "CopiumCore/system-interface.h"
#include "Editor/editor-sceneview.h"
#include "Editor/editor-game.h"
#include "Editor/editor-content-browser.h"
#include "Editor/editor-camera.h"

namespace Copium
{
	CLASS_SYSTEM(EditorSystem)
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Initialises the editor for the engine. It creates the ImGui context for
			the engine
		*/
		/***************************************************************************/
		void init();

		/***************************************************************************/
		/*!
		\brief
			Updates the editors in the engine
		*/
		/***************************************************************************/
		void update();

		/***************************************************************************/
		/*!
		\brief
			Renders all Imgui in the engine
		*/
		/***************************************************************************/
		void draw();

		/***************************************************************************/
		/*!
		\brief
			Shutdown Imgui and its components
		*/
		/***************************************************************************/
		void exit();

		/***************************************************************************/
		/*!
		\brief
			Adds a log to the imgui console
		*/
		/***************************************************************************/
		void imguiConsoleAddLog(std::string value);
		
		bool is_enabled() { return enableEditor; }

		EditorSceneView* get_scene_view() { return &sceneView; }
		EditorContentBrowser* get_content_browser() { return &contentBrowser; }
		EditorCamera* get_camera() { return &camera; }

	private:
		bool enableEditor = true;

		EditorSceneView sceneView;
		EditorGame game;
		EditorContentBrowser contentBrowser;
		EditorCamera camera;
	};
}
#endif // !EDITOR_SYSTEM_H
