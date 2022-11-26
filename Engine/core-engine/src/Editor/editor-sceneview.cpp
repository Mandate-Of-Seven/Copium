/*!***************************************************************************************
\file			editor-sceneview.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the definition of functions for the scene view of the editor, where
	the user can view the scene.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include <GL/glew.h>
#include "Editor/editor-sceneview.h"
#include "Editor/editor-system.h"
#include "Windows/windows-system.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo.h>



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
		camera.update();

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
		// Gizmos
		Scene* scene = sm.get_current_scene();
		EditorSceneView::update_gizmos();
		ImGui::End();
		ImGui::PopStyleVar();
		// End Scene View
		
		// Render stats settings
		windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

		// Begin Render Stats
		ImGui::Begin("Renderer Stats", 0, windowFlags);
		
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
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && windowHovered)
		{
			scene = sm.get_current_scene();
			if (scene != nullptr)
			{
				std::vector<GameObject*> pGameObjs; // Possible selectable gameobjects
				for (GameObject* gameObject : scene->gameObjects)
				{
					Transform& t = gameObject->transform;
					glm::vec2 mousePosition = glm::vec3(camera.get_ndc(), 0.f);
					glm::vec2 objPosition = glm::vec2(t.position.x, t.position.y);
					// Not Within bounds
					if (glm::distance(objPosition, mousePosition)
						> glm::length(camera.get_dimension()))
						continue;

					glm::vec2 min = glm::vec2(objPosition.x - t.scale.x * 0.5f, objPosition.y - t.scale.y * 0.5f);
					glm::vec2 max = glm::vec2(objPosition.x + t.scale.x * 0.5f, objPosition.y + t.scale.y * 0.5f);

					// Check AABB
					if (mousePosition.x > min.x && mousePosition.x < max.x)
					{
						if (mousePosition.y > min.y && mousePosition.y < max.y)
						{
							pGameObjs.push_back(gameObject);
						}
					}
				}

				// Ensure that the container is not empty
				if (!pGameObjs.empty())
				{
					// Sort base on depth value
					std::sort(pGameObjs.begin(), pGameObjs.end(),
						[](GameObject* lhs, GameObject* rhs) -> bool
						{return (lhs->transform.position.z > rhs->transform.position.z); });

					bool selected = false;
					for (int i = 0; i < pGameObjs.size(); i++)
					{
						// Get the next gameobject after
						if (sm.get_selected_gameobject() == pGameObjs[i])
						{
							if (i + 1 < pGameObjs.size())
							{
								sm.set_selected_gameobject(pGameObjs[i + 1]);
								selected = true;
								break;
							}
							else if (i + 1 >= pGameObjs.size())
							{
								sm.set_selected_gameobject(pGameObjs[0]);
								selected = true;
								break;
							}
						}
					}

					// If there is no selected gameobject
					if (sm.get_selected_gameobject() == nullptr || !selected)
					{
						GameObject* selectObject = pGameObjs.front();
						if (sm.selectedGameObject != selectObject)
						{
							for (GameObject* gameObject : pGameObjs)
							{
								// Select closest gameobject
								float depth = gameObject->transform.position.z;

								if (depth > selectObject->transform.position.z)
								{
									selectObject = gameObject;
								}
							}
							sm.set_selected_gameobject(selectObject);
						}
					}

					//PRINT("Set object to: " << sm.selectedGameObject->get_name());
				}
				else
					sm.set_selected_gameobject(nullptr);
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

	void EditorSceneView::update_gizmos()
	{
		static ImGuizmo::OPERATION currop = ImGuizmo::OPERATION::TRANSLATE;
		GameObject* currObj = sm.get_selected_gameobject();
		if (currObj)
		{
			Transform& trf = currObj->transform;
			glm::vec2 size(trf.scale.x, trf.scale.y);
			float rot = trf.rotation.z;
			rot = float(rot * (3.1416/180));
			const glm::vec3& pos = trf.position;

			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();


			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float)sceneWidth, (float)sceneHeight);
			const glm::mat4 camProj = camera.get_projection();
			glm::mat4 camView = camera.get_view_matrix();
			glm::mat4 translate = glm::translate(glm::mat4(1.f), pos);
			glm::mat4 rotation = {
			glm::vec4(cos(rot), sin(rot), 0.f, 0.f),
			glm::vec4(-sin(rot), cos(rot), 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			glm::mat4 scale = {
				glm::vec4(size.x, 0.f, 0.f, 0.f),
				glm::vec4(0.f, size.y, 0.f, 0.f),
				glm::vec4(0.f, 0.f, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};
			glm::mat4 transform = translate * rotation * scale;
			if (ImGui::IsKeyReleased(ImGuiKey_W))
			{
				currop = ImGuizmo::OPERATION::TRANSLATE;

			}
			if (ImGui::IsKeyReleased(ImGuiKey_R))
			{
				currop = ImGuizmo::OPERATION::SCALE;

			}
			if (ImGui::IsKeyReleased(ImGuiKey_E))
			{	
				currop = ImGuizmo::OPERATION::ROTATE;

			}
			ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(camProj),
				currop, ImGuizmo::LOCAL, glm::value_ptr(transform));
			if (ImGuizmo::IsUsing())
			{
				float newTranslate[3]; 
				float newRotate[3];
				float newScale[3];
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), newTranslate, newRotate, newScale);
				glm::vec3 nTranslation{ newTranslate[0], newTranslate[1], newTranslate[2] };
				glm::vec3 nRotation{ newRotate[0], newRotate[1], newRotate[2] };
				glm::vec3 nScale{ newScale[0], newScale[1], newScale[2] };
				trf.position = nTranslation;
				trf.scale = nScale;
				trf.rotation.z = nRotation.z;
				

			}

		}
	}
}
