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

#include "Editor/editor-sceneview.h"
#include "Editor/editor-system.h"
#include "Graphics/graphics-system.h"

namespace Copium
{
	// Bean: Temporary global variable
	GraphicsSystem* graphics;

	void EditorSceneView::init()
	{
		graphics = GraphicsSystem::Instance();

		sceneDimension = { sceneWidth, sceneHeight };
	}

	void EditorSceneView::update()
	{
		// Scene view settings
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		// Begin Scene View
		ImGui::Begin("Scene View", 0, windowFlags);
		windowFocused = ImGui::IsWindowFocused();
		windowHovered = ImGui::IsWindowHovered();
		scenePosition = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

		unsigned int textureID = graphics->get_framebuffer()->get_color_attachment_id();
		ImVec2 viewportEditorSize = ImGui::GetContentRegionAvail();
		resize_sceneview(*((glm::vec2*) &viewportEditorSize));
		ImGui::Image((void*) (size_t) textureID, ImVec2{ (float)sceneWidth, (float)sceneHeight }, ImVec2{ 0 , 1 }, ImVec2{ 1 , 0 });

		ImGui::End();
		ImGui::PopStyleVar();
		// End Scene View
		
		// Render stats settings
		windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

		// Begin Render Stats
		ImGui::Begin("Renderer Stats", 0, windowFlags);

		ImGui::Text("Render Stats");
		char buffer[64];
		sprintf(buffer, "Sprite Count: %d", (int) graphics->get_sprites().size());
		ImGui::Text(buffer);

		sprintf(buffer, "Viewport Dimensions: %d by %d", sceneWidth, sceneHeight);
		ImGui::Text(buffer);

		// End Render Stats
		ImGui::End();
	}

	void EditorSceneView::exit()
	{
		
	}

	void EditorSceneView::resize_sceneview(glm::vec2 _newDimension)
	{
		// Only if the current scene dimension is not the same as new dimension
		if (sceneDimension != _newDimension && _newDimension.x != 0 && _newDimension.y != 0)
		{
			sceneDimension = { _newDimension.x, _newDimension.y };
			sceneWidth = (int)sceneDimension.x;
			sceneHeight = (int)sceneDimension.y;
			graphics->get_framebuffer()->resize(sceneWidth, sceneHeight);
			EditorSystem::Instance()->get_camera()->on_resize(sceneDimension.x, sceneDimension.y);
		}
	}
}
