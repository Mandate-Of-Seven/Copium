/*!***************************************************************************************
\file			base-camera.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/11/2022

\brief
	This file contains the definitions of the functions of the base camera class which
	draws the objects within the camera using orthographic or perspective projection

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include <glm/gtx/quaternion.hpp>

#include "Graphics/base-camera.h"
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include <Math/math-library.h>
#include <Editor/editor-system.h>

namespace Copium
{
	namespace
	{
		InputSystem& inputSystem{ *InputSystem::Instance() };
		WindowsSystem& windowSystem{ *WindowsSystem::Instance() };
	}

	void BaseCamera::init(float _width, float _height, CameraType _cameraType, bool _orthographic)
	{
		// Setting up data
		viewer = { 0.f, 0.f, 10.f };
		focalPoint = { viewer.x, viewer.y, 0.f };
		/*viewer = { 0.f, 0.f, 0.f };
		focalPoint = { viewer.x, viewer.y, 0.f };*/
		upVector = { 0.f, 1.f, 0.f };

		width = _width;
		height = _height;

		// For frustum culling
		updateFrustum();

		//pitch = 0.f;
		//yaw = 0.f;

		cameraType = _cameraType;
		
		// Setting up properties
		backgroundColor = { 0.278f, 0.278f, 0.278f, 1.f };
		orthographic = _orthographic;

		// Setting up projection data
		aspect = width / height;
		nearClip = 0.03f;
		farClip = 500.f;

		if (orthographic)
		{
			fieldOfView = 0.f;
			orthographicSize = 5.f;
		}
		else
		{
			fieldOfView = 60.f;
			orthographicSize = 0.f;
		}

		// Calculating projection
		viewMatrix = glm::lookAt(viewer, focalPoint, upVector);
		update_ortho_projection();
		//projMatrix = glm::ortho(width, height, nearClip, farClip);
		//viewProjMatrix = projMatrix * viewMatrix;

		// Initialise Sub systems
		framebuffer.set_size((unsigned int) _width, (unsigned int) _height);
		framebuffer.init();
		draw.init(this);
	}

	void BaseCamera::update()
	{
		update_view_matrix();

		draw_camera();
	}

	void BaseCamera::draw_camera()
	{
		updateFrustum();

		if (cameraType == CameraType::GAME && !MyEditorSystem.is_enabled())
		{
			draw.update(cameraType);
		}
		else
		{
			framebuffer.bind();

			draw.update(cameraType);

			framebuffer.unbind();
		}	
	}

	void BaseCamera::exit()
	{
		framebuffer.exit();
		draw.exit();
	}

	glm::vec3 BaseCamera::get_right_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(1.f, 0.f, 0.f));
	}

	glm::vec3 BaseCamera::get_up_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.f, 1.f, 0.f));
	}

	glm::vec3 BaseCamera::get_forward_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.f, 0.f, -1.f));
	}

	glm::quat BaseCamera::get_orientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.f));
	}

	glm::vec2 BaseCamera::get_pan_speed() const
	{
		glm::vec2 speed;
		float x = std::min(width / 1000.f, 2.4f); // Max speed is 2.4
		speed.x = 0.04f * (x * x) - 0.1778f * x + 0.3f;
		float y = std::min(height / 1000.f, 2.4f); // Max speed is 2.4
		speed.y = 0.04f * (y * y) - 0.1778f * y + 0.3f;
		return speed;
	}

	glm::vec2 BaseCamera::get_game_ndc()
	{
		EditorGame* gameView = MyEditorSystem.get_game_view();

		glm::vec2 scenePos = gameView->get_position();
		scenePos.x += gameView->get_indent();
		glm::vec2 sceneDim = gameView->get_dimension();
		if (!MyEditorSystem.is_enabled())
		{
			scenePos = { 0.f, 0.f };
			sceneDim = glm::vec2(MyWindowSystem.get_window_width(), MyWindowSystem.get_window_height());
		}

		//PRINT("Scene Dimension: " << scenePos.x << " " << scenePos.y);
		Math::Vec2 mousePos = MyInputSystem.get_mouseposition();
		//PRINT("Mouse position : " << mousePos.x << " " << mousePos.y);
		glm::vec2 centreOfScene = { scenePos.x + sceneDim.x / 2, scenePos.y + sceneDim.y / 2 };
		glm::vec2 mouseScenePos = { mousePos.x - centreOfScene.x, centreOfScene.y - mousePos.y };
		glm::vec2 mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 + 0.1f };

		if (!MyEditorSystem.is_enabled())
		{
			mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 };
		}

		mouseToNDC *= orthographicSize;
		//PRINT("ORTHO SIZE: " << orthographicSize);
		glm::vec2 worldNDC = { mouseToNDC.x + viewer.x, mouseToNDC.y + viewer.y };
		//PRINT("~: " << worldNDC.x << ", " << worldNDC.y);
		return worldNDC;
	}

	bool BaseCamera::withinFrustum(const glm::vec3& _position, const glm::vec3& _scale, bool _overlap)
	{
		float left = _overlap ? frustumBottom.x - _scale.x : frustumBottom.x;
		float right = _overlap ? frustumTop.x + _scale.x : frustumTop.x;
		float top = _overlap ? frustumTop.y + _scale.y : frustumTop.x;
		float bottom = _overlap ? frustumBottom.y - _scale.y : frustumBottom.y;

		//PRINT("Frustum Dimensions: " << left << " " << right << " " << top << " " << bottom);

		// Checks to verify that the object is within the frustum
		if (_position.x - _scale.x * 0.5f < left) // Left-most
			return false;
		if (_position.x + _scale.x * 0.5f > right) // Right-most
			return false;
		if (_position.y + _scale.y * 0.5f > top) // Top-most
			return false;
		if (_position.y - _scale.y * 0.5f < bottom) // Bottom-most
			return false;

		return true;
	}

	void BaseCamera::on_resize(float _width, float _height)
	{
		width = _width;
		height = _height;
		aspect = width / height;

		framebuffer.resize((GLuint) width, (GLuint) height);
		update_ortho_projection();
	}

	void BaseCamera::update_ortho_projection()
	{
		float ar = aspect;
		float zl = orthographicSize;
		projMatrix = glm::ortho(-ar * zl, ar * zl, -zl, zl, nearClip, farClip);
		
		viewProjMatrix = projMatrix * viewMatrix;
		//viewProjMatrix = viewMatrix * projMatrix;
	}

	void BaseCamera::update_ortho_projection(float _left, float _right, float _bottom, float _top)
	{
		projMatrix = glm::ortho(_left, _right, _bottom, _top, nearClip, farClip);
		viewProjMatrix = projMatrix * viewMatrix;
	}

	void BaseCamera::update_view_matrix()
	{
		// Get updated viewer / eye location
		viewer = calculate_position();
		viewer.z = 10.f;

		focalPoint = { viewer.x, viewer.y, 0.f };
		upVector = { 0.f, 1.f, 0.f };

		// Update view matrix
		viewMatrix = glm::lookAt(viewer, focalPoint, upVector);

		// Update view projection matrix
		viewProjMatrix = projMatrix * viewMatrix;
		//viewProjMatrix = viewMatrix * projMatrix;
	}

	void BaseCamera::updateFrustum()
	{
		// Calculate the view frustum using the camera's dimensions and zoom level
		glm::vec2 bottomLeft = glm::vec2(- width / height, -1.f ) * orthographicSize;
		glm::vec2 topRight = glm::vec2(width / height, 1.f) * orthographicSize;

		// Include the margin and the camera's current position
		glm::vec2 bottomMargin = glm::vec2(focalPoint.x - frustumMargin, focalPoint.y - frustumMargin);
		glm::vec2 topMargin = glm::vec2(focalPoint.x + frustumMargin, focalPoint.y + frustumMargin);

		frustumBottom = bottomLeft + bottomMargin;
		frustumTop = topRight + topMargin;
	}

	glm::vec3 BaseCamera::calculate_position()
	{ 
		//glm::vec3 viewVector = get_forward_direction();
		return viewer - get_forward_direction() * orthographicSize;
	}
}