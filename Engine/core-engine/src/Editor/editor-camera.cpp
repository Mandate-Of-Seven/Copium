/*!***************************************************************************************
\file			editor-camera.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			15/10/2022

\brief
	Contains function definitions for the editor camera.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Windows/windows-input.h"
#include "Editor/editor-camera.h"
#include "Editor/editor-system.h"
#include "SceneManager/sm.h"

namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };

	bool enableCamera = false;
}

namespace Copium
{

	void EditorCamera::init(float _width, float _height, bool _orthographic)
	{
		BaseCamera::init(_width, _height, _orthographic);
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_START_PREVIEW, this);
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
	}

	void EditorCamera::update()
	{
		EditorSceneView* sceneView = EditorSystem::Instance()->get_scene_view();
		if (sceneView->is_window_hovered())
		{
			mouse_controls();
		}

		if (enableCamera)
		{
			NewSceneManager* sm = NewSceneManager::Instance();
			Scene* scene = sm->get_current_scene();
			if (scene != nullptr && !scene->get_name().compare("DemoCLONE"))
			{
				orthographic = 5.f;
				update_ortho_projection();
				for (size_t i = 0; i < scene->get_gameobjectvector().size(); i++)
				{
					// If the object is the player
					if (!scene->get_gameobjectvector()[i]->get_name().compare("PlayerTrain"))
					{
						GameObject* go = scene->get_gameobjectvector()[i];
						focalPoint = go->transform.glmPosition(); // Fix the camera onto the player
						//PRINT("Focal point: " << focalPoint.x << " " << focalPoint.y);
					}
				}
			}
		}

		BaseCamera::update();
	}

	void EditorCamera::handleMessage(MESSAGE_TYPE _mType)
	{
		if (_mType == MESSAGE_TYPE::MT_START_PREVIEW)
		{
			enableCamera = true;
		}
		else if (_mType == MESSAGE_TYPE::MT_STOP_PREVIEW)
		{
			enableCamera = false;
		}
	}

	/*void EditorCamera::update_view_matrix()
	{
		set_position(calculate_position());

		glm::vec3 viewer = get_position();

		glm::mat4 translate = {
			glm::vec4(1.f, 0.f, 0.f, 0.f),
			glm::vec4(0.f, 1.f, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(viewer.x, viewer.y, viewer.z, 1.f)
		};

		float rad = glm::radians(get_angle());

		glm::mat4 rotate = {
			glm::vec4(cos(rad), sin(rad), 0.f, 0.f),
			glm::vec4(-sin(rad), cos(rad), 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		viewMatrix = translate * rotate;

		viewMatrix = glm::inverse(viewMatrix);

		projMatrix = orthoProjMatrix * viewMatrix;
	}*/

	glm::vec2 EditorCamera::get_ndc() const
	{
		EditorSceneView* sceneView = EditorSystem::Instance()->get_scene_view();
		glm::vec2 scenePos = sceneView->get_position();
		glm::vec2 sceneDim = sceneView->get_dimension();
		Math::Vec2 mousePos = inputSystem.get_mouseposition();
		glm::vec2 centreOfScene = { scenePos.x + sceneDim.x / 2, scenePos.y + sceneDim.y / 2 };
		glm::vec2 mouseScenePos = { mousePos.x - centreOfScene.x, centreOfScene.y - mousePos.y };
		glm::vec2 mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 + 0.1f };
		mouseToNDC *= orthographicSize;
		glm::vec2 worldNDC = { mouseToNDC.x + viewer.x, mouseToNDC.y + viewer.y };
		return worldNDC;
	}

	void EditorCamera::mouse_controls()
	{
		// Bean: shouldnt be necessary here
		// Clamping camera within boundary
		if (viewer.x > 100.f)
		{
			focalPoint = glm::vec3(100.f, viewer.y, 0.f);
		}
		if (viewer.x < -100.f)
		{
			focalPoint = glm::vec3(-100.f, viewer.y, 0.f);
		}
		if (viewer.y > 100.f)
		{
			focalPoint = glm::vec3(viewer.x, 100.f, 0.f);
		}
		if (viewer.y < -100.f)
		{
			focalPoint = glm::vec3(viewer.x, -100.f, 0.f);
		}
		
		glm::vec2 worldNDC = get_ndc();
		glm::vec2 delta = (worldNDC - mousePosition) * 4.f;
		mousePosition = worldNDC;

		// Movement using right click and drag
		if (inputSystem.is_mousebutton_pressed(1) || inputSystem.is_mousebutton_pressed(2))
		{
			glm::vec2 speed = get_pan_speed();
			focalPoint += -get_up_direction() * delta.y * speed.y;
			focalPoint += -get_right_direction() * delta.x * speed.x;
		}

		//if (inputSystem.is_key_held(GLFW_KEY_LEFT_CONTROL))
		//{
		//	glm::vec2 speed = get_pan_speed();
		//	// Bean: Zoomlevel should be positive
		//	if (inputSystem.is_key_held(GLFW_KEY_W)) // Up
		//	{
		//		focalPoint += get_up_direction() * 0.1f * speed.y * zoomLevel;
		//	}
		//	if (inputSystem.is_key_held(GLFW_KEY_A)) // Left
		//	{
		//		focalPoint += -get_right_direction() * 0.1f * speed.x * zoomLevel;
		//	}
		//	if (inputSystem.is_key_held(GLFW_KEY_S)) // Down
		//	{
		//		focalPoint += get_up_direction() * -0.1f * speed.y * zoomLevel;
		//	}
		//	if (inputSystem.is_key_held(GLFW_KEY_D)) // Right
		//	{
		//		focalPoint += -get_right_direction() * -0.1f * speed.x * zoomLevel;
		//	}
		//}

		// Rotation
		/*if (inputSystem.is_key_held(GLFW_KEY_LEFT_ALT))
		{
			glm::vec2 mouse{ inputSystem.get_mouseX(), inputSystem.get_mouseY() };
			glm::vec2 delta = (mouse - mousePosition) * 0.003f;
			mousePosition = mouse;

			if (inputSystem.is_mousebutton_pressed(GLFW_MOUSE_BUTTON_RIGHT))
			{
				float yawSign = (get_up_direction().y < 0.f) ? -1.f : 1.f;
				yaw += yawSign * delta.x * 0.8f;
				pitch += delta.y * 0.8f;
			}
		}*/

		// Zoom In and Out
		int scroll = (int) inputSystem.get_mousescroll();
		if (scroll && !enableCamera)
		{
			orthographicSize -= scroll * 0.1f * get_zoom_speed(); // Zoom In
			
			if (orthographicSize <= nearClip)
				orthographicSize = nearClip;
			if (orthographicSize >= farClip)
				orthographicSize = farClip - 1.f;


			update_ortho_projection();
		}

		//scroll = inputSystem.get_mousescroll();
	}

	float EditorCamera::get_zoom_speed() const
	{
		float tempDistance = orthographicSize * 0.2f;
		tempDistance = std::max(orthographicSize, 0.f); // Max distance is 0
		float speed = orthographicSize * orthographicSize;
		speed = std::min(speed, 50.f); // The max speed currently is 50
		return speed;
	}
}