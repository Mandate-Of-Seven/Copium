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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
		SceneManager& sm{ *SceneManager::Instance() };
		bool inOp = false;
	}

	void EditorSceneView::init()
	{
		sceneDimension = { sceneWidth, sceneHeight };
	}

	glm::vec2 update_position(Transform* _t, glm::vec2 const& _pos)
	{
		glm::vec2 tempPos = _pos;
		if (_t->hasParent())
		{
			tempPos += glm::vec2(_t->parent->position.x, _t->parent->position.y);
			tempPos = update_position(_t->parent, tempPos);
		}
		//PRINT("Cur : " << tempPos.x << " " << tempPos.y);
		return tempPos;
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
		
		// Gizmos
		Scene* scene = sm.get_current_scene();
		update_gizmos();

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

		sprintf(buffer, "Draw Count: %d", camera.getDraw()->get_renderer()->getDrawCount());
		ImGui::Text(buffer);

		sprintf(buffer, "Quad Count: %d", camera.getDraw()->get_renderer()->getQuadCount());
		ImGui::Text(buffer);

		sprintf(buffer, "Line Count: %d", camera.getDraw()->get_renderer()->getLineCount());
		ImGui::Text(buffer);

		sprintf(buffer, "Circle Count: %d", camera.getDraw()->get_renderer()->getCircleCount());
		ImGui::Text(buffer);

		sprintf(buffer, "Text Count: %d", camera.getDraw()->get_renderer()->getTextCount());
		ImGui::Text(buffer);

		sprintf(buffer, "Vertex Count: %d", camera.getDraw()->get_renderer()->getVertices());
		ImGui::Text(buffer);

		// End Render Stats
		ImGui::End();

		// Mouse picking
		bool mouseReleased = ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		if (!inOp && mouseReleased && windowHovered)
		{
			scene = sm.get_current_scene();
			if (scene != nullptr)
			{
				std::vector<GameObject*> pGameObjs; // Possible selectable gameobjects
				for (GameObject* gameObject : scene->gameObjects)
				{
					Transform& t = gameObject->transform;
					glm::vec2 mousePosition = glm::vec3(camera.get_ndc(), 0.f);
					glm::vec3 tempPos = t.position;
					glm::vec3 tempScale = t.scale;

					if (t.hasParent())
					{
						Transform* tempObj = t.parent;

						glm::mat4 pTranslate, pRotate, pScale, pTransform;

						while (tempObj)
						{
							pTranslate = glm::translate(glm::mat4(1.f), tempObj->position.glmVec3);

							float pRot = glm::radians(tempObj->rotation.z);
							pRotate = {
								glm::vec4(cos(pRot), sin(pRot), 0.f, 0.f),
								glm::vec4(-sin(pRot), cos(pRot), 0.f, 0.f),
								glm::vec4(0.f, 0.f, 1.f, 0.f),
								glm::vec4(0.f, 0.f, 0.f, 1.f)
							};

							glm::vec3 scale = tempObj->scale.glmVec3;
							pScale = {
								glm::vec4(scale.x, 0.f, 0.f, 0.f),
								glm::vec4(0.f, scale.y, 0.f, 0.f),
								glm::vec4(0.f, 0.f, 1.f, 0.f),
								glm::vec4(0.f, 0.f, 0.f, 1.f)
							};

							pTransform = pTranslate * pRotate * pScale;
							tempPos = glm::vec3(pTransform * glm::vec4(tempPos, 1.f));
							tempScale *= tempObj->scale.glmVec3;
							tempObj = tempObj->parent;
						}
					}

					glm::vec2 objPosition = { tempPos.x, tempPos.y };

					// Not Within bounds
					if (glm::distance(objPosition, mousePosition)
						> glm::length(camera.get_dimension()))
						continue;

					glm::vec2 min = glm::vec2(objPosition.x - tempScale.x * 0.5f, objPosition.y - tempScale.y * 0.5f);
					glm::vec2 max = glm::vec2(objPosition.x + tempScale.x * 0.5f, objPosition.y + tempScale.y * 0.5f);

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

		inOp = ImGuizmo::IsUsing();
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
			float rot = glm::radians(trf.rotation.z);
			const glm::vec3& pos = trf.position;

			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float) sceneWidth, (float)sceneHeight + 50.f);
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

			glm::mat4 pTranslate, pRotate, pScale, pTransform, totalTransform, tempTransform;
			if (currObj->transform.hasParent())
			{
				totalTransform = transform;
				Transform* tempObj = currObj->transform.parent;

				while (tempObj)
				{
					pTranslate = glm::translate(glm::mat4(1.f), tempObj->position.glmVec3);

					float pRot = glm::radians(tempObj->rotation.z);
					pRotate = {
						glm::vec4(cos(pRot), sin(pRot), 0.f, 0.f),
						glm::vec4(-sin(pRot), cos(pRot), 0.f, 0.f),
						glm::vec4(0.f, 0.f, 1.f, 0.f),
						glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					glm::vec3 scale = tempObj->scale.glmVec3;
					pScale = {
						glm::vec4(scale.x, 0.f, 0.f, 0.f),
						glm::vec4(0.f, scale.y, 0.f, 0.f),
						glm::vec4(0.f, 0.f, 1.f, 0.f),
						glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					pTransform = pTranslate * pRotate * pScale;
					totalTransform = pTransform * totalTransform;

					tempObj = tempObj->parent;
				}

			}
			else
			{
				totalTransform = transform;
			}

			tempTransform = totalTransform;

			if (ImGui::IsWindowHovered())
			{
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
			}
			
			ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(camProj),
				currop, ImGuizmo::LOCAL, glm::value_ptr(totalTransform));
			if (ImGuizmo::IsUsing())
			{
				float newTranslate[3]; 
				float newRotate[3];
				float pNewRotate[3];
				float newScale[3];
				float pNewScale[3];
				glm::mat4 temp = totalTransform - tempTransform;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(tempTransform), newTranslate, pNewRotate, pNewScale);
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(totalTransform), newTranslate, newRotate, newScale);
				glm::vec3 nTranslation = { temp[3].x, temp[3].y, temp[3].z };
				glm::vec3 nScale = { newScale[0] - pNewScale[0], newScale[1] - pNewScale[1], 0.f};
				// Bean: If parent object is rotated, child guizmos objects will have issues
				trf.position += nTranslation;
				trf.scale += nScale;
				trf.rotation.z += newRotate[2] - pNewRotate[2];
			}
		}
	}
}
