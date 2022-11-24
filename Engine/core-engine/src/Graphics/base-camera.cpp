/*!***************************************************************************************
\file			base-camera.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/11/2022

\brief
	This file contains the definitions of the functions of the base camera class.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include <glm/gtx/quaternion.hpp>

#include "Graphics/base-camera.h"
#include "Editor/editor-system.h"

namespace Copium
{
	namespace
	{
		EditorSystem& editorSystem{ *EditorSystem::Instance() };
	}

	bool BaseCamera::deserialize(rapidjson::Value& _value)
	{
		if (!_value.HasMember("Color"))
			return false;

		rapidjson::Value& _v = _value["Color"].GetObj();
		backgroundColor.x = _v["X"].GetFloat();
		backgroundColor.y = _v["Y"].GetFloat();
		backgroundColor.z = _v["Z"].GetFloat();
		backgroundColor.w = _v["W"].GetFloat();

		if (!_value.HasMember("Projection"))
			return false;

		orthographic = _value["Projection"].GetBool();

		if (!_value.HasMember("Clipping Planes"))
			return false;

		_v = _value["Clipping Planes"].GetObj();
		nearClip = _v["Near"].GetFloat();
		farClip = _v["Far"].GetFloat();

		return true;
	}

	bool BaseCamera::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::Value color(rapidjson::kObjectType);
		color.AddMember("X", backgroundColor.x, _doc.GetAllocator());
		color.AddMember("Y", backgroundColor.y, _doc.GetAllocator());
		color.AddMember("Z", backgroundColor.z, _doc.GetAllocator());
		color.AddMember("W", backgroundColor.w, _doc.GetAllocator());
		_value.AddMember("Color", color, _doc.GetAllocator());

		_value.AddMember("Projection", orthographic, _doc.GetAllocator());

		rapidjson::Value clippingPlanes(rapidjson::kObjectType);
		clippingPlanes.AddMember("Near", nearClip, _doc.GetAllocator());
		clippingPlanes.AddMember("Far", farClip, _doc.GetAllocator());
		_value.AddMember("Clipping Planes", clippingPlanes, _doc.GetAllocator());

		return true;
	}


	void BaseCamera::init(float _width, float _height, CameraType _cameraType, bool _orthographic)
	{
		// Setting up data
		viewer = { 0.f, 0.f, -10.f };
		focalPoint = { viewer.x, viewer.y, 0.f };
		/*viewer = { 0.f, 0.f, 0.f };
		focalPoint = { viewer.x, viewer.y, 0.f };*/
		upVector = { 0.f, 1.f, 0.f };

		width = _width;
		height = _height;

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
		projMatrix = glm::ortho(width, height, nearClip, farClip);
		viewProjMatrix = projMatrix * viewMatrix;

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
		if (cameraType == CameraType::GAME && !editorSystem.is_enabled())
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

	void BaseCamera::update_ortho_projection()
	{
		float ar = aspect;
		float zl = orthographicSize;
		projMatrix = glm::ortho(-ar * zl, ar * zl, -zl, zl, nearClip, farClip);
		viewProjMatrix = projMatrix * viewMatrix;
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
		focalPoint = { viewer.x, viewer.y, 0.f };
		upVector = { 0.f, 1.f, 0.f };

		// Update view matrix
		viewMatrix = glm::lookAt(viewer, focalPoint, upVector);

		// Update view projection matrix
		viewProjMatrix = projMatrix * viewMatrix;
		//viewProjMatrix = viewMatrix * projMatrix;
	}

	void BaseCamera::on_resize(float _width, float _height)
	{
		framebuffer.resize((GLuint)_width, (GLuint) _height);

		width = _width;
		height = _height;
		aspect = _width / _height;
		update_ortho_projection();
	}

	glm::vec3 BaseCamera::calculate_position()
	{
		return focalPoint - get_forward_direction() * orthographicSize;
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
}