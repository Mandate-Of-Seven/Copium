/*!***************************************************************************************
\file			editor-camera.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			15/10/2022

\brief


All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Graphics/camera.h"

namespace Copium
{
	// Editor camera which moves around in the scene
	class EditorCamera : public Camera
	{
	public:
		void init(float _width, float _height, bool _rotation = false);
		
		void update();

		// Accessing Properties

		// Directional Properties
		glm::vec3 get_right_direction() const;
		glm::vec3 get_up_direction() const;
		glm::vec3 get_forward_direction() const;
		glm::quat get_orientation() const;
		glm::vec2 get_pan_speed() const;

		float get_zoom() const { return zoomLevel; }
		float get_zoom_speed() const;
		float get_pitch() const { return pitch; }
		float get_yaw() const { return yaw; }

		// Matrices
		void on_resize(float _width, float _height);

		glm::mat4 get_projection() const { return projMatrix; }
		glm::mat4 get_ortho_projection() const { return orthoProjMatrix; }
		glm::mat4 get_view_matrix() const { return viewMatrix; }

	private:
		void update_ortho_projection(float _aspectRatio, float _zoomLevel);
		void update_ortho_projection(float _left, float _right, float _bottom, float _top);

		void update_view_matrix();
		void mouse_controls();

		glm::vec3 calculate_position();

	private:
		/* Camera Data ******************************************************************/
		glm::vec3 focalPoint{0};
		glm::vec2 mousePosition{0};

		float aspectRatio = 0.f;
		float nearClip = -1.f, farClip = 100.f; // Orthographic projection's view box
		float zoomLevel = 4.f; // To zoom in and out
		float pitch = 0.f, yaw = 0.f; // For rotation
		int width = 0, height = 0;

		/* Matrices & Projection ********************************************************/
		glm::mat4 projMatrix{0};
		glm::mat4 viewMatrix{0};
		glm::mat4 orthoProjMatrix{0};
	};
}
#endif // !EDITOR_CAMERA_H