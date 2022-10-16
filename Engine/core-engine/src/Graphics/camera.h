/*!***************************************************************************************
\file			camera.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			15/10/2022

\brief
	

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "Math/transform.h"

namespace Copium
{
	// Base camera which contains data
	class Camera
	{
	public:
		glm::vec3 get_position() const { return position; }
		void set_position(glm::vec3 _position) { position = _position; }

		Transform* get_transform() { return transform; }
		void set_transform(Transform* _transform) { transform = _transform; }

	private:
		/* Camera Data ******************************************************************/
		glm::vec3 position;
		Transform* transform; // Transform of the camera

	};
}
#endif // !CAMERA_H