/*!***************************************************************************************
\file			sprite.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			14/09/2022

\brief
	This file holds the sprite's data which is used to render using the Graphics system
	on to the scene.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>
#include <component.h>
#include <cstring>

namespace Copium::Component
{
	class Transform;
	class Texture;

	// Depends on Transform
	class Sprite final
	{
	public:
		// Serialization inherited from Component
		std::string const get_name() { return sprite_name; }
		void const set_name(std::string _name) { sprite_name = _name; }

		glm::vec2 const get_position() { return position; }
		void const set_position(glm::vec2 _position) { position = _position; }

		glm::vec2 const get_size() { return size; }
		void const set_size(glm::vec2 _size) { size = _size; }

		glm::vec4 const get_color() { return color; }
		void const set_color(glm::vec4 _color) { color = _color; }

	private:

		/* Sprite Information ***********************************************************/
		std::string sprite_name; // Name of sprite

		Texture* texture; // The texture used for this sprite
		Transform* transform; // The transform of this sprite ( Bean: it refer to the component in the gameobject )

		glm::vec2 position; // Temporary variable to access the position
		glm::vec2 size; // The size of the sprite in pixels ( Bean: different from the scale of the gameobject )
		glm::vec4 color; // The blended color of this sprite
	};
}
#endif // !SPRITE_H
