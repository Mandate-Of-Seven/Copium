/*!***************************************************************************************
\file			editor-sceneview.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the definition of functions for the scene view of the editor.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "windows-system.h"
#include "editor-sceneview.h"
#include "graphics-system.h"

namespace Copium::Editor::SceneView
{
	// Bean: Temporary global variable
	glm::vec2 viewportSize;
	Copium::Graphics::GraphicsSystem* graphics;

	void init()
	{
		graphics = Copium::Graphics::GraphicsSystem::Instance();
		graphics->sceneWidth = 1280;
		graphics->sceneHeight = 720;
	}

	void update()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Scene View");

		GLuint textureID = graphics->framebuffer.get_color_attachment_id();

		ImVec2 viewportEditorSize = ImGui::GetContentRegionAvail();

		if (viewportSize != *((glm::vec2 *) &viewportEditorSize))
		{
			viewportSize = { viewportEditorSize.x, viewportEditorSize.y };
			graphics->framebuffer.resize(viewportSize.x, viewportSize.y);
		}

		ImGui::Image((void *) textureID, ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0 , 1 }, ImVec2{ 1 , 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void exit()
	{
		
	}
}
