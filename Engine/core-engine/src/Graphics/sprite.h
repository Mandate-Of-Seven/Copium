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
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <string>
#include <rapidjson/document.h>

#include "Graphics/textures.h"

namespace Copium
{
	class Sprite final
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Loads the values into the engine
		\param _value
			The values to reference from
		\return
			A boolean to check if all values are loaded
		*/
		/***************************************************************************/
		bool deserialize(rapidjson::Value& _value);

		/***************************************************************************/
		/*!
		\brief
			Saves the values into the file
		\param _value
			The values to reference from
		\param _doc
			The document to save to
		\return
			A boolean to check if all values are saved
		*/
		/***************************************************************************/
		bool serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

		// Accessing Properties

		// Name
		std::string const get_name() { return sprite_name; }
		void const set_name(std::string _name) { sprite_name = _name; }

		// Sprite
		const unsigned int& get_sprite_id() const { return spriteID; }
		void set_sprite_id(const unsigned int& _index) { spriteID = _index; }
		unsigned int* access_sprite_id() { return &spriteID; }

		const Texture* get_texture() const { return refTexture; }
		void set_texture(Texture* _texture) { refTexture = _texture; }

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
		const glm::vec4& get_color() const { return color; }
		void set_color(const glm::vec4& _color) { color = _color; }
		float* access_color() { return &color.x; }

	private:
		/* Sprite Information ***********************************************************/
		std::string sprite_name = "empty"; // Name of sprite

		Texture* refTexture; // Pointer to the texture
		unsigned int spriteID = 0; // The index of the sprite

		glm::bvec2 flip{ 0 };
		glm::vec3 position{ 0 }; // Temporary variable to access the position
		glm::vec3 rotation{ 0 }; // Temporary variable for the rotation of the sprite
		glm::vec2 size{ 0 }; // The size of the sprite in pixels ( Bean: different from the scale of the gameobject )
		glm::vec4 color{1}; // The blended color of this sprite
	};

}
#endif // !SPRITE_RENDERER_H
