/*!***************************************************************************************
\file			base-camera.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/11/2022

\brief
	This file contains the declarations of the base camera class and its data and 
	properities of a camera in the engine. 

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include <rapidjson/document.h>

#include "Graphics/graphics-typedef.h"
#include "Graphics/framebuffer.h"
#include "Graphics/graphics-draw.h"

namespace Copium
{
	// Base camera which contains data
	class BaseCamera
	{
	public:
		/*******************************************************************************
		/*!
		\brief
			Deserialize this renderer component's data from the specified rapidjson Value
		\param _value
			reference to the rapidjson Value which the renderer component's data
			deserializes its data
		*/
		/*******************************************************************************/
		bool deserialize(rapidjson::Value& _value);

		/*******************************************************************************
		/*!
		\brief
			Serialize this renderer component's data to the specified rapidjson Value
		\param _value
			reference to the rapidjson Value which the renderer component's data is to
			serialize its data to
		\param _doc
			reference to the rapidjson Document which is associated to the save file
			which the data is being saved to
		*/
		/*******************************************************************************/
		bool serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

		/***************************************************************************/
		/*!
		\brief
			Initialize the base camera properties
		\param _width
			The width of the screen
		\param _height
			The height of the screen
		\param _cameraType
			The type of camera to initialize the camera to
		\param _orthographic
			Is it orthographic or perspective projection
		*/
		/**************************************************************************/
		void init(float _width, float _height, CameraType _cameraType = NONE, bool _orthographic = true);
		
		/***************************************************************************/
		/*!
		\brief
			Update the camera matrices and draw the objects within the camera
		*/
		/**************************************************************************/
		void update();

		/***************************************************************************/
		/*!
		\brief
			Draw the objects within the camera
		*/
		/**************************************************************************/
		void draw_camera();

		/***************************************************************************/
		/*!
		\brief
			Exits the camera and the framebuffer
		*/
		/**************************************************************************/
		void exit();

		const glm::vec3& get_eye() const { return viewer; }
		const glm::vec2& get_dimension() { return dimension = glm::vec2(width, height); }

		// Directional Properties
		/***************************************************************************/
		/*!
		\brief
			Get the right vector of the camera
		*/
		/**************************************************************************/
		glm::vec3 get_right_direction() const;

		/***************************************************************************/
		/*!
		\brief
			Get the up vector of the camera
		*/
		/**************************************************************************/
		glm::vec3 get_up_direction() const;

		/***************************************************************************/
		/*!
		\brief
			Get the forward direction of the camera
		*/
		/**************************************************************************/
		glm::vec3 get_forward_direction() const;

		/***************************************************************************/
		/*!
		\brief
			Get the orientation of the camera
		*/
		/**************************************************************************/
		glm::quat get_orientation() const;

		/***************************************************************************/
		/*!
		\brief
			Get the panning speed of the camera
		*/
		/**************************************************************************/
		glm::vec2 get_pan_speed() const;

		float get_pitch() const { return pitch; }
		float get_yaw() const { return yaw; }

		CameraType get_type() { return cameraType; }
		glm::vec2 get_game_ndc();

		/***************************************************************************/
		/*!
		\brief
			Resize the camera to the dimension specified in the parameters
		\param _width
			The width to resize the camera to
		\param _height
			The height to resize the camera to
		*/
		/**************************************************************************/
		void on_resize(float _width, float _height);

		/***************************************************************************/
		/*!
		\brief
			Update the orthographic projection of the camera
		*/
		/**************************************************************************/
		void update_ortho_projection();

		// Matrices
		glm::mat4 get_projection() const { return projMatrix; }
		glm::mat4 get_view_matrix() const { return viewMatrix; }
		glm::mat4 get_view_proj_matrix() const { return viewProjMatrix; }

		Framebuffer* get_framebuffer() { return &framebuffer; }

		void set_bg_color(glm::vec4 const& _color) { backgroundColor = _color; }
		const glm::vec4& get_bg_color() const { return backgroundColor; }

	protected:
		
		/***************************************************************************/
		/*!
		\brief
			Update the orthographic projection of the camera
		\param _left
			The left most view of the camera
		\param _right
			The right most view of the camera
		\param _bottom
			The bottom most view of the camera
		\param _top
			The top most view of the camera
		*/
		/**************************************************************************/
		void update_ortho_projection(float _left, float _right, float _bottom, float _top);

		/***************************************************************************/
		/*!
		\brief
			Update the position of the camera and update the view projection and 
			view matrices
		*/
		/**************************************************************************/
		void update_view_matrix();

		/***************************************************************************/
		/*!
		\brief
			Calculates the position of the camera
		*/
		/**************************************************************************/
		glm::vec3 calculate_position();

	protected:

		/* Camera Data ******************************************************************/
		glm::vec3 viewer;					// The location of the viewer / eye (Center of the screen, 10 units away)
		glm::vec3 focalPoint;				// The look-at point / target point where the viewer is looking (Center of screen)
		glm::vec3 upVector;					// Defines which way is up (Default is Y-direction)
		glm::vec2 dimension;				// The dimension of the camera in width and height defined in pixels

		float width = 0.f;					// Pixel width of the camera
		float height = 0.f;					// Pixel height of the camera
		float pitch = 0.f;					// For rotation
		float yaw = 0.f;					// For rotation

		CameraType cameraType;				// Type of camera
		
		/* Camera Properties ************************************************************/
		glm::vec4 backgroundColor;			// Default solid color when rendering
		bool cullingMask = false;			// Bean: A culling mask to prevent rendering of specific layers, to be implemented in the future
		bool orthographic = true;			// Orthographic by default until perspective camera has been implemented
		bool useOcclusionCulling = false;	// Bean: A feature to be implemented in the future

		/* Projection Data **************************************************************/
		float aspect = 0.f;					// The aspect ratio of the camera in width/height (Automatically calculated by screen's aspect ratio)
		float nearClip = 0.f;				// Distance of near clipping plane from the camera
		float farClip = 0.f;				// Distance of far clipping plane from the camera
		float fieldOfView = 0.f;			// The vertical field of view in degrees
		float orthographicSize = 0.f;		// Half-size of camera in orthographic mode

		/* Matrices & Projection ********************************************************/
		glm::mat4 projMatrix{ 0 };			// The projection matrix to use, either orthographic or perspective
		glm::mat4 viewMatrix{ 0 };			// The view matrix -> worldToCamera matrix
		glm::mat4 viewProjMatrix{ 0 };		// The view projection matrix -> viewMatrix * projMatrix

		glm::mat4 cameraToNDC{ 0 };
		glm::mat4 ndcToViewport{ 0 };

		/* Stored Information ***********************************************************/
		Framebuffer framebuffer;
		Draw draw;
	};
}
#endif // !BASE_CAMERA_H