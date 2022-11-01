/*!***************************************************************************************
\file			sprite-renderer.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			14/09/2022

\brief
	This file holds the sprite's data which is used to render using the Graphics system
	on to the scene.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <string>
#include <rttr/type>
#include <rapidjson/document.h>

namespace Copium
{
	// Depends on Transform
	class SpriteRenderer final
	{
	public:
		// Serialization inherited from Component
		bool deserialize(rapidjson::Value& _value);
		bool serialize(rapidjson::Value& _value, rapidjson::Document& _doc);
		// Accessing Properties

		// Name
		std::string const get_name() { return sprite_name; }
		void const set_name(std::string _name) { sprite_name = _name; }

		// Sprite
		const unsigned int& get_sprite_id() { return spriteID; }
		void set_sprite_id(const unsigned int& _index) { spriteID = _index; }
		unsigned int* access_sprite_id() { return &spriteID; }

		// Flip
		bool* access_flip_x() { return &flip.x; }
		bool* access_flip_y() { return &flip.y; }

		// Position
		glm::vec3 const get_position() { return position; }
		void const set_position(glm::vec3 _position) { position = _position; }

		// Rotation
		glm::vec3 const get_rotation() { return rotation; }
		void const set_rotation(glm::vec3 _rotation) { rotation = _rotation; }

		// Size
		glm::vec2 const get_size() { return size; }
		void const set_size(glm::vec2 _size) { size = _size; }

		// Color
		glm::vec4 const get_color() { return color; }
		void const set_color(glm::vec4 _color) { color = _color; }
		float* access_color() { return &color.x; }

	private:

		/* Sprite Information ***********************************************************/
		std::string sprite_name; // Name of sprite

		unsigned int spriteID = 0; // The index of the sprite

		glm::bvec2 flip{ 0 };
		glm::vec3 position{ 0 }; // Temporary variable to access the position
		glm::vec3 rotation{ 0 }; // Temporary variable for the rotation of the sprite
		glm::vec2 size{ 0 }; // The size of the sprite in pixels ( Bean: different from the scale of the gameobject )
		glm::vec4 color{ 0 }; // The blended color of this sprite
	
	public:
		RTTR_ENABLE();
	};

}
#endif // !SPRITE_RENDERER_H
