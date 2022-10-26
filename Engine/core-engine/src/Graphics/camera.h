/*!***************************************************************************************
\file			camera.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			15/10/2022

\brief
	

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "GameObject/transform-component.h"

namespace Copium
{
	// Base camera which contains data
	class Camera
	{
	public:
		glm::vec3 get_position() const { return position; }
		void set_position(glm::vec3 _position) { position = _position; }

		glm::mat4 get_transform() { return transform; }
		void set_transform(glm::mat4 _transform) { transform = _transform; }

		float get_angle() const { return angle; }
		void set_angle(float _angle) { angle = _angle; }

	private:
		/* Camera Data ******************************************************************/
		glm::vec3 position{0};
		float angle = 0.f;
		glm::mat4 transform{0}; // Transform of the camera

	};
}
#endif // !CAMERA_H