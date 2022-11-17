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

#include <glm/glm.hpp>

namespace Copium
{
	enum CameraType
	{
		NONE,
		GAME,		// In game camera / play mode
		SCENEVIEW,	// The editor camera
		PREVIEW		// The preview camera in the editor
	};

	// Base camera which contains data
	class BaseCamera
	{
	public:
		void init(float _width, float _height, bool _orthographic = true);
		
		void update();

		const glm::vec3& get_eye() const { return viewer; }

		// Directional Properties
		glm::vec3 get_right_direction() const;
		glm::vec3 get_up_direction() const;
		glm::vec3 get_forward_direction() const;
		glm::quat get_orientation() const;
		glm::vec2 get_pan_speed() const;

		float get_pitch() const { return pitch; }
		float get_yaw() const { return yaw; }

		// Matrices
		void on_resize(float _width, float _height);

		glm::mat4 get_projection() const { return projMatrix; }
		glm::mat4 get_view_matrix() const { return viewMatrix; }
		glm::mat4 get_view_proj_matrix() const { return viewProjMatrix; }

	protected:
		void update_ortho_projection();
		void update_ortho_projection(float _left, float _right, float _bottom, float _top);

		void update_view_matrix();

		glm::vec3 calculate_position();

		void set_bg_color(glm::vec4 const& _color) { backgroundColor = _color; }
		const glm::vec4& get_bg_color() const { return backgroundColor; }

	protected:

		/* Camera Data ******************************************************************/
		glm::vec3 viewer;					// The location of the viewer / eye (Center of the screen, 10 units away)
		glm::vec3 focalPoint;				// The look-at point / target point where the viewer is looking (Center of screen)
		glm::vec3 upVector;					// Defines which way is up (Default is Y-direction)

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
	};
}
#endif // !BASE_CAMERA_H