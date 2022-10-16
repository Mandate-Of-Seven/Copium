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

#include "Windows/input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Editor/editor-camera.h"
#include "Editor/editor-system.h"

namespace Copium::Editor
{
	EditorSceneView sceneView;

	void EditorCamera::init(float _aspectRatio, float _bottom, float _top)
	{
		EditorSystem* editor = EditorSystem::Instance();
		sceneView = editor->get_scene_view();
		aspectRatio = _aspectRatio;
		float vfHeight = _top - _bottom;
		float vfWidth = aspectRatio * vfHeight;
		float _left = -vfWidth * 0.5f;
		float _right = vfWidth * 0.5f;
		update_ortho_projection(_left, _right, _bottom, _top);
	}

	void EditorCamera::update()
	{
		mouse_controls();

		/*float ar = aspectRatio;
		float zl = zoomLevel;
		update_ortho_projection(-ar * zl, ar * zl, -zl, zl);*/

		update_view_matrix();
	}

	void EditorCamera::update_ortho_projection(float _left, float _right, float _bottom, float _top)
	{
		aspectRatio = sceneView.get_scene_width() / sceneView.get_scene_height();
		orthoProjMatrix = glm::ortho(_left, _right, _bottom, _top, nearClip, farClip);

		//float w = sceneView.get_scene_width() / 100.f;
		//float h = sceneView.get_scene_height() / 100.f;

		//// Camera to NDC Transform
		//glm::mat4 ndcMatrix = {
		//	glm::vec4(2 / w, 0.f, 0.f, 0.f),
		//	glm::vec4(0.f, 2 / h, 0.f, 0.f),
		//	glm::vec4(0.f, 0.f, 1.f, 0.f),
		//	glm::vec4(0.f, 0.f, 0.f, 1.f)
		//};

		//viewProjMatrix = ndcMatrix * orthoProjMatrix * viewMatrix;
	}

	void EditorCamera::on_resize(float _width, float _height)
	{
		aspectRatio = _width / _height;

		float ar = aspectRatio;
		float zl = zoomLevel;
		update_ortho_projection(-ar * zl, ar * zl, -zl, zl);
	}

	void EditorCamera::update_view_matrix()
	{
		set_position(calculate_position());

		glm::vec3 pos = get_position();

		viewMatrix = glm::mat4{
			glm::vec4(1.f, 0.f, 0.f, 0.f),
			glm::vec4(0.f, 1.f, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(pos.x, pos.y, pos.z, 1.f)
		};

		viewMatrix = glm::inverse(viewMatrix);

		/*for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::cout << std::setprecision(2) << viewMatrix[j][i] << " ";
			}
			std::cout << "\n";
		}*/

		float w = sceneView.get_scene_width() / 100.f;
		float h = sceneView.get_scene_height() / 100.f;
		float widthVP = w * 0.5f;
		float heightVP = h * 0.5f;

		// Camera to NDC Transform
		glm::mat4 ndcMatrix = {
			glm::vec4(2 / w, 0.f, 0.f, 0.f),
			glm::vec4(0.f, 2 / h, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		// NDC to Viewport Transform
		/*glm::mat4 viewportMatrix = {
			glm::vec4(widthVP, 0.f, 0.f, 0.f),
			glm::vec4(0.f, heightVP, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 0.5f, 0.f),
			glm::vec4(widthVP, heightVP, 0.5f, 1.f)
		};*/

		viewProjMatrix = ndcMatrix * orthoProjMatrix * viewMatrix;

		//PRINT("View Projection Matrix:");
		/*for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::cout << std::setprecision(2) << viewProjMatrix[j][i] << " ";
			}
			std::cout << "\n";
		}*/

		//viewProjMatrix = projMatrix * viewMatrix;
	}

	void EditorCamera::mouse_controls()
	{
		glm::vec3 pos = get_position();
		// Movement WASD
		if(Input::is_key_held(GLFW_KEY_LEFT_CONTROL))
		//if (Input::is_mousebutton_pressed(GLFW_MOUSE_BUTTON_RIGHT)) //cannot use rn
		{
			PRINT("Camera pos:" << pos.x << "," << pos.y << "," << pos.z);
			glm::vec2 speed = get_pan_speed();

			// Bean: Zoomlevel should be positive
			if (Input::is_key_held(GLFW_KEY_W)) // Up
			{
				focalPoint += get_up_direction() * 0.1f * speed.y * -zoomLevel;
			}
			if (Input::is_key_held(GLFW_KEY_A)) // Left
			{
				focalPoint += -get_right_direction() * 0.1f * speed.x * -zoomLevel;
			}
			if (Input::is_key_held(GLFW_KEY_S)) // Down
			{
				focalPoint += get_up_direction() * -0.1f * speed.y * -zoomLevel;
			}
			if (Input::is_key_held(GLFW_KEY_D)) // Right
			{
				focalPoint += -get_right_direction() * -0.1f * speed.x * -zoomLevel;
			}
		}

		if (Input::is_key_held(GLFW_KEY_LEFT_ALT))
		{
			glm::vec2 mouse{ Input::get_mouseX(), Input::get_mouseY() };
			glm::vec2 delta = (mouse - mousePosition) * 0.003f;
			mousePosition = mouse;

			// Rotation
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
			PRINT("Camera pos:" << pos.x << "," << pos.y << "," << pos.z);
			zoomLevel -= 0.001f * get_zoom_speed(); // Zoom In
			if (zoomLevel <= nearClip)
				zoomLevel = nearClip;

			float ar = aspectRatio;
			float zl = zoomLevel;
			update_ortho_projection(-ar * zl, ar * zl, -zl, zl);

		}
		else if (Input::is_key_held(GLFW_KEY_Z))
		{
			PRINT("Camera pos:" << pos.x << "," << pos.y << "," << pos.z);
			zoomLevel -= -0.001f * get_zoom_speed(); // Zoom Out
			if (zoomLevel >= farClip)
				zoomLevel = farClip;

			float ar = aspectRatio;
			float zl = zoomLevel;
			update_ortho_projection(-ar * zl, ar * zl, -zl, zl);
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
		float x = std::min(sceneView.get_scene_width() / 1000.f, 2.4f); // Max speed is 2.4
		speed.x = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		float y = std::min(sceneView.get_scene_height() / 1000.f, 2.4f); // Max speed is 2.4
		speed.y = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
		return speed;
	}

	float EditorCamera::get_zoom_speed() const
	{
		float tempDistance = zoomLevel * 0.2f;
		tempDistance = std::max(zoomLevel, 0.f); // Max distance is 0
		float speed = zoomLevel * zoomLevel;
		speed = std::min(speed, 25.f); // The max speed currently is 100
		return speed;
	}
}