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
#include "Editor/editor-sceneview.h"
#include "graphics-system.h"

namespace Copium::Editor::SceneView
{
	// Bean: Temporary global variable
	glm::vec2 viewportSize;
	Copium::Graphics::GraphicsSystem* graphics;

	void init()
	{
		graphics = Copium::Graphics::GraphicsSystem::Instance();
		graphics->set_scene_width(1280);
		graphics->set_scene_height(720);
	}

	void update()
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		// Begin
		ImGui::Begin("Scene View", 0, window_flags);
		graphics->set_scene_position(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y));

		char buffer[64];
		sprintf(buffer, "Sprite Count: %d", (int)graphics->get_sprites().size());
		ImGui::Text(buffer);

		GLuint textureID = graphics->get_framebuffer().get_color_attachment_id();

		ImVec2 viewportEditorSize = ImGui::GetContentRegionAvail();

		if (viewportSize != *((glm::vec2 *) &viewportEditorSize))
		{
			viewportSize = { viewportEditorSize.x, viewportEditorSize.y };
			graphics->get_framebuffer().resize((GLuint) viewportSize.x, (GLuint) viewportSize.y);
		}

		ImGui::Image((void*) (size_t) textureID, ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0 , 1 }, ImVec2{ 1 , 0 });

		// End
		ImGui::End();
		ImGui::PopStyleVar();
		
	}

	void exit()
	{
		
	}
}
