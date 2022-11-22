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
#include "SceneManager/sm.h"
#include "Windows/windows-system.h"

namespace Copium
{
	namespace
	{
		EditorCamera& camera{ *EditorSystem::Instance()->get_camera() };
		NewSceneManager& sm{ *NewSceneManager::Instance() };
	}

	void EditorSceneView::init()
	{
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
		
		unsigned int textureID = camera.get_framebuffer()->get_color_attachment_id();
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

		Scene* scene = sm.get_current_scene();
		size_t gameobjectCount = 0;
		if (scene != nullptr)
			gameobjectCount = scene->get_gameobjcount();

		ImGui::Text("Render Stats");
		char buffer[64];
		sprintf(buffer, "GameObject Count: %zd", (size_t)gameobjectCount);
		ImGui::Text(buffer);

		sprintf(buffer, "Viewport Dimensions: %d by %d", sceneWidth, sceneHeight);
		ImGui::Text(buffer);

		// End Render Stats
		ImGui::End();

		// Mouse picking
		//check_click();

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && windowHovered)
		{
			Scene* scene = sm.get_current_scene();
			if (scene != nullptr)
			{
				std::vector<GameObject*> possibleGameObjects;
				for (GameObject* gameObject : scene->get_gameobjectvector())
				{
					Transform& t = gameObject->transform;
					glm::vec2 mousePosition = glm::vec3(camera.get_ndc(), 0.f);
					glm::vec2 objPosition = glm::vec2(t.glmPosition().x, t.glmPosition().y);
					// Not Within bounds
					if (glm::distance(objPosition, mousePosition)
						> glm::length(camera.get_dimension()))
						continue;

					glm::vec2 min = glm::vec2(objPosition.x - t.glmScale().x * 0.5f, objPosition.y - t.glmScale().y * 0.5f);
					glm::vec2 max = glm::vec2(objPosition.x + t.glmScale().x * 0.5f, objPosition.y + t.glmScale().y * 0.5f);

					// Check AABB
					if (mousePosition.x > min.x && mousePosition.x < max.x)
					{
						if (mousePosition.y > min.y && mousePosition.y < max.y)
						{
							possibleGameObjects.push_back(gameObject);
						}
					}
				}

				// Sort base on depth

				if (!possibleGameObjects.empty())
				{
					// If selected gameobject is one of the possible gameobjects
					for (int i = 0; i < possibleGameObjects.size(); i++)
					{
						// Get the next gameobject after
						if (sm.get_selected_gameobject() == possibleGameObjects[i])
						{
							if (i + 1 < possibleGameObjects.size())
							{
								sm.set_selected_gameobject(possibleGameObjects[i + 1]);
								break;
							}
							else if (i + 1 >= possibleGameObjects.size())
							{
								sm.set_selected_gameobject(possibleGameObjects[0]);
								break;
							}
						}
					}

					// If there is no selected gameobject
					if (sm.selectedGameObject == nullptr)
					{
						GameObject* selectObject = possibleGameObjects.front();
						for (GameObject* gameObject : possibleGameObjects)
						{
							// Select closest gameobject
							float depth = gameObject->transform.glmPosition().z;

							if (depth > selectObject->transform.glmPosition().z)
							{
								selectObject = gameObject;
							}
						}
						sm.set_selected_gameobject(selectObject);
					}

					PRINT("Set object to: " << sm.selectedGameObject->get_name());
				}
			}
		}
	}

	void EditorSceneView::exit()
	{
		
	}

	void EditorSceneView::resize_sceneview(glm::vec2 _newDimension)
	{
		// Only resize if the window is focused
		if (!WindowsSystem::Instance()->get_window_focused())
			return;

		// Only if the current scene dimension is not the same as new dimension
		if (sceneDimension != _newDimension && _newDimension.x != 0 && _newDimension.y != 0)
		{
			sceneDimension = { _newDimension.x, _newDimension.y };
			sceneWidth = (int)sceneDimension.x;
			sceneHeight = (int)sceneDimension.y;
			camera.on_resize(sceneDimension.x, sceneDimension.y);
		}
	}

	void EditorSceneView::check_click()
	{
		ImGuiIO& io = ImGui::GetIO();

		static float clickTime = 0.f;
		clickTime = io.MouseClickedTime[ImGuiMouseButton_Left];
		static float previousClickTime = 0.f;

		static bool doubleClick = false;
		static bool singleClick = false;
		
		static float timer = 0.f;
		timer += (float)WindowsSystem::Instance()->get_delta_time();
		if (io.MouseClicked[ImGuiMouseButton_Left])
		{
			doubleClick = singleClick = false;
			float timeTaken = clickTime - previousClickTime;
			PRINT("Time: " << timer);
			PRINT("Click time: " << timeTaken);
			if (timeTaken < io.MouseDoubleClickTime)
			{
				PRINT("Double Click");
				doubleClick = true;
				timer = 0.f;
			}
			else if(timer > io.MouseDoubleClickTime)
			{
				PRINT("Single Click");
				singleClick = true;
				timer = 0.f;
			}
			PRINT("");
		}

		previousClickTime = clickTime;
	}
}
