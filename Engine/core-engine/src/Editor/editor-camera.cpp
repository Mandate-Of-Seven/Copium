/*!***************************************************************************************
\file			editor-camera.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			15/10/2022

\brief


All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Windows/windows-input.h"
#include "Editor/editor-camera.h"
#include "Editor/editor-system.h"

namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
}

namespace Copium
{
	void EditorCamera::init(float _width, float _height, bool _rotation)
	{
		aspectRatio = _width / _height;
		width = _width;
		height = _height;
		update_ortho_projection(aspectRatio, zoomLevel);
	}

	void EditorCamera::update()
	{
		EditorSceneView* sceneView = EditorSystem::Instance()->get_scene_view();
		if (sceneView->is_window_hovered())
		{
			mouse_controls();
		}

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

	glm::vec2 EditorCamera::get_ndc() const
	{
		glm::vec3 pos = get_position();
		EditorSceneView* sceneView = EditorSystem::Instance()->get_scene_view();
		glm::vec2 scenePos = sceneView->get_position();
		glm::vec2 sceneDim = sceneView->get_dimension();
		Math::Vec2 mousePos = inputSystem.get_mouseposition();
		glm::vec2 centreOfScene = { scenePos.x + sceneDim.x / 2, scenePos.y + sceneDim.y / 2 };
		glm::vec2 mouseScenePos = { mousePos.x - centreOfScene.x, centreOfScene.y - mousePos.y };
		glm::vec2 mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 + 0.1f };
		mouseToNDC *= zoomLevel;
		glm::vec2 worldNDC = { mouseToNDC.x + pos.x, mouseToNDC.y + pos.y };
		return worldNDC;
	}

	void EditorCamera::mouse_controls()
	{
		glm::vec3 pos = get_position();

		// Bean: shouldnt be necessary here
		// Clamping camera within boundary
		if (pos.x > 100.f)
		{
			focalPoint = glm::vec3(100.f, pos.y, 0.f);
		}
		if (pos.x < -100.f)
		{
			focalPoint = glm::vec3(-100.f, pos.y, 0.f);
		}
		if (pos.y > 100.f)
		{
			focalPoint = glm::vec3(pos.x, 100.f, 0.f);
		}
		if (pos.y < -100.f)
		{
			focalPoint = glm::vec3(pos.x, -100.f, 0.f);
		}
		
		// Bean: This needs to be refactored to some function
		// Calculate delta drag
		EditorSceneView* sceneView = EditorSystem::Instance()->get_scene_view();
		glm::vec2 scenePos = sceneView->get_position();
		glm::vec2 sceneDim = sceneView->get_dimension();
		Math::Vec2 mousePos = inputSystem.get_mouseposition();
		glm::vec2 centreOfScene = { scenePos.x + sceneDim.x / 2, scenePos.y + sceneDim.y / 2 };
		glm::vec2 mouseScenePos = { mousePos.x - centreOfScene.x, centreOfScene.y - mousePos.y };
		glm::vec2 mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 + 0.1f };
		mouseToNDC *= zoomLevel;
		glm::vec2 worldNDC = { mouseToNDC.x + pos.x, mouseToNDC.y + pos.y };
		glm::vec2 delta = (worldNDC - mousePosition) * 4.f;
		mousePosition = worldNDC;

		// Movement using right click and drag
		if (inputSystem.is_mousebutton_pressed(1))
		{
			glm::vec2 speed = get_pan_speed();
			focalPoint += -get_up_direction() * delta.y * speed.y;
			focalPoint += -get_right_direction() * delta.x * speed.x;
		}
		

		//if (Input::is_key_held(GLFW_KEY_LEFT_CONTROL))
		//{
		//	glm::vec2 speed = get_pan_speed();
		//	// Bean: Zoomlevel should be positive
		//	if (Input::is_key_held(GLFW_KEY_W)) // Up
		//	{
		//		focalPoint += get_up_direction() * 0.1f * speed.y * zoomLevel;
		//	}
		//	if (Input::is_key_held(GLFW_KEY_A)) // Left
		//	{
		//		focalPoint += -get_right_direction() * 0.1f * speed.x * zoomLevel;
		//	}
		//	if (Input::is_key_held(GLFW_KEY_S)) // Down
		//	{
		//		focalPoint += get_up_direction() * -0.1f * speed.y * zoomLevel;
		//	}
		//	if (Input::is_key_held(GLFW_KEY_D)) // Right
		//	{
		//		focalPoint += -get_right_direction() * -0.1f * speed.x * zoomLevel;
		//	}
		//}

		// Rotation
		/*if (Input::is_key_held(GLFW_KEY_LEFT_ALT))
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
		if (scroll)
		{
			zoomLevel -= scroll * 0.1f * get_zoom_speed(); // Zoom In

			if (zoomLevel <= nearClip)
				zoomLevel = nearClip;
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
		speed.x = 0.04f * (x * x) - 0.1778f * x + 0.3f;
		float y = std::min(height / 1000.f, 2.4f); // Max speed is 2.4
		speed.y = 0.04f * (y * y) - 0.1778f * y + 0.3f;
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