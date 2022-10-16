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

namespace Copium::Editor
{
	// Bean: Temporary global variable
	EditorCamera camera;
	Copium::Graphics::GraphicsSystem* graphics;

	void EditorSceneView::init()
	{
		graphics = Copium::Graphics::GraphicsSystem::Instance();
		EditorSystem* editor = EditorSystem::Instance();
		camera = editor->get_camera();

		sceneDimension = { sceneWidth, sceneHeight };
	}

	void EditorSceneView::update()
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		// Begin
		ImGui::Begin("Scene View", 0, window_flags);
		scenePosition = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

		char buffer[64];
		sprintf(buffer, "Sprite Count: %d", (int)graphics->get_sprites().size());
		ImGui::Text(buffer);

		sprintf(buffer, "Viewport Dimensions: %d by %d", sceneWidth, sceneHeight);
		ImGui::Text(buffer);

		GLuint textureID = graphics->get_framebuffer().get_color_attachment_id();

		ImVec2 viewportEditorSize = ImGui::GetContentRegionAvail();
		resize_sceneview(*((glm::vec2*) &viewportEditorSize));

		ImGui::Image((void*) (size_t) textureID, ImVec2{ (float)sceneWidth, (float)sceneHeight }, ImVec2{ 0 , 1 }, ImVec2{ 1 , 0 });

		// End
		ImGui::End();
		ImGui::PopStyleVar();
		
	}

	void EditorSceneView::exit()
	{
		
	}

	void EditorSceneView::resize_sceneview(glm::vec2 _newDimension)
	{
		// Only if the current scene dimension is not the same as new dimension
		if (sceneDimension != _newDimension)
		{
			sceneDimension = { _newDimension.x, _newDimension.y };
			sceneWidth = (int)sceneDimension.x;
			sceneHeight = (int)sceneDimension.y;
			camera.on_resize(sceneWidth, sceneHeight);
			graphics->get_framebuffer().resize();
		}
	}
}
