/*!***************************************************************************************
\file			editor-camera.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			15/10/2022

\brief


All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include <GLFW/glfw3.h>

#include "Windows/input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Editor/editor-camera.h"
#include "Editor/editor-system.h"

namespace Copium::Editor
{
	void EditorCamera::init(float _aspectRatio, bool _rotation)
	{
		aspectRatio = _aspectRatio;
		update_ortho_projection(aspectRatio, zoomLevel);
	}

	void EditorCamera::update()
	{
		mouse_controls();
		update_view_matrix();
	}

	void EditorCamera::update_ortho_projection(float _aspectRatio, float _zoomLevel)
	{
		float ar = _aspectRatio;
		float zl = _zoomLevel;
		orthoProjMatrix = glm::ortho(-ar * zl, ar * zl, -zl, zl, nearClip, farClip);
		projMatrix = orthoProjMatrix * viewMatrix;
	}

	void EditorCamera::update_ortho_projection(float _left, float _right, float _bottom, float _top)
	{
		orthoProjMatrix = glm::ortho(_left, _right, _bottom, _top, nearClip, farClip);
		projMatrix = orthoProjMatrix * viewMatrix;
	}

	void EditorCamera::update_view_matrix()
	{
		set_position(calculate_position());

		glm::vec3 pos = get_position();

		glm::mat4 translate = {
			glm::vec4(1.f, 0.f, 0.f, 0.f),
			glm::vec4(0.f, 1.f, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(pos.x, pos.y, pos.z, 1.f)
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
	}

	void EditorCamera::on_resize(float _width, float _height)
	{
		width = (int)_width;
		height = (int)_height;
		aspectRatio = _width / _height;
		update_ortho_projection(aspectRatio, zoomLevel);
	}

	void EditorCamera::mouse_controls()
	{
		glm::vec3 pos = get_position();
		
		// Movement WASD
		if(Input::get_input_instance()->mouseButtons[1] == GLFW_PRESS && Input::get_input_instance()->mouseButtons[1] != GLFW_RELEASE)
		{
			glm::vec2 speed = get_pan_speed();
			EditorSceneView* sceneView = EditorSystem::Instance()->get_scene_view();
			glm::vec2 scenePos = sceneView->get_position();
			glm::vec2 sceneDim = sceneView->get_dimension();
			glm::vec2 cameraPos = get_position();

			glm::vec2 mousePos = { Input::get_mouse_position().first, Input::get_mouse_position().second };
			glm::vec2 centreOfScene = { scenePos.x + sceneDim.x / 2, scenePos.y + sceneDim.y / 2 };
			glm::vec2 mouseScenePos = { mousePos.x - centreOfScene.x, centreOfScene.y - mousePos.y };
			glm::vec2 mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 + 0.1f };
			mouseToNDC *= zoomLevel;
			glm::vec2 worldNDC = { mouseToNDC.x - cameraPos.x, mouseToNDC.y - cameraPos.y };
			glm::vec2 delta = (worldNDC - mousePosition);
			PRINT("Delta position: " << delta.x << " " << delta.y);
			mousePosition = worldNDC;

			focalPoint += get_up_direction() * delta.y * speed.y * zoomLevel;
			focalPoint += -get_right_direction() * delta.x * speed.x * zoomLevel;

			// Bean: Zoomlevel should be positive
			//if (Input::is_key_held(GLFW_KEY_W)) // Up
			//{
			//	focalPoint += get_up_direction() * 0.1f * speed.y * zoomLevel;
			//}
			//if (Input::is_key_held(GLFW_KEY_A)) // Left
			//{
			//	focalPoint += -get_right_direction() * 0.1f * speed.x * zoomLevel;
			//}
			//if (Input::is_key_held(GLFW_KEY_S)) // Down
			//{
			//	focalPoint += get_up_direction() * -0.1f * speed.y * zoomLevel;
			//}
			//if (Input::is_key_held(GLFW_KEY_D)) // Right
			//{
			//	focalPoint += -get_right_direction() * -0.1f * speed.x * zoomLevel;
			//}
		}

		// Rotation
		if (Input::is_key_held(GLFW_KEY_LEFT_ALT))
		{
			glm::vec2 mouse{ Input::get_mouseX(), Input::get_mouseY() };
			glm::vec2 delta = (mouse - mousePosition) * 0.003f;
			mousePosition = mouse;

			if (Input::is_mousebutton_pressed(GLFW_MOUSE_BUTTON_RIGHT))
			{
				float yawSign = (get_up_direction().y < 0.f) ? -1.f : 1.f;
				yaw += yawSign * delta.x * 0.8f;
				pitch += delta.y * 0.8f;
			}
		}

		// Zoom In and Out
		if (Input::is_key_held(GLFW_KEY_Z) && Input::is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			// 0.01f to be changed to scoll
			zoomLevel -= 0.01f * get_zoom_speed(); // Zoom In
			if (zoomLevel <= nearClip)
				zoomLevel = nearClip;

			update_ortho_projection(aspectRatio, zoomLevel);
		}
		else if (Input::is_key_held(GLFW_KEY_Z))
		{
			// -0.01f to be changed to scoll
			zoomLevel -= -0.01f * get_zoom_speed(); // Zoom Out
			if (zoomLevel >= farClip)
				zoomLevel = farClip;

			update_ortho_projection(aspectRatio, zoomLevel);
		}
	}

	glm::vec3 EditorCamera::calculate_position()
	{
		return focalPoint - get_forward_direction() * zoomLevel;
	}

	glm::vec3 EditorCamera::get_right_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(1.f, 0.f, 0.f));
	}

	glm::vec3 EditorCamera::get_up_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.f, 1.f, 0.f));
	}

	glm::vec3 EditorCamera::get_forward_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.f, 0.f, -1.f));
	}

	glm::quat EditorCamera::get_orientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.f));
	}

	glm::vec2 EditorCamera::get_pan_speed() const
	{
		glm::vec2 speed;
		float x = std::min(width / 1000.f, 2.4f); // Max speed is 2.4
		speed.x = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		float y = std::min(height / 1000.f, 2.4f); // Max speed is 2.4
		speed.y = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
		return speed;
	}

	float EditorCamera::get_zoom_speed() const
	{
		float tempDistance = zoomLevel * 0.2f;
		tempDistance = std::max(zoomLevel, nearClip); // Max distance is 0
		float speed = zoomLevel * zoomLevel;
		speed = std::min(speed, 50.f); // The max speed currently is 50
		return speed;
	}
}