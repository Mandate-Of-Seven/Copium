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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include "Graphics/renderer.h"
#include <cstring>

namespace Copium::Graphics
{
	class Transform;
	class Texture;

	// Depends on Transform
	class SpriteRenderer final : public Graphics::Renderer
	{
	public:
		// Serialization inherited from Component

		// Accessing Properties

		// Name
		std::string const get_name() { return sprite_name; }
		void const set_name(std::string _name) { sprite_name = _name; }

		// Texture
		void const bind_texture(Texture* _texture) { texture = _texture; }
		Texture* const get_texture() { return texture; }

		// Position
		glm::vec2 const get_position() { return position; }
		void const set_position(glm::vec2 _position) { position = _position; }

		// Rotation
		glm::vec3 const get_rotation() { return rotation; }
		void const set_rotation(glm::vec3 _rotation) { rotation = _rotation; }

		// Size
		glm::vec2 const get_size() { return size; }
		void const set_size(glm::vec2 _size) { size = _size; }

		// Color
		glm::vec4 const get_color() { return color; }
		void const set_color(glm::vec4 _color) { color = _color; }

	private:

		/* Sprite Information ***********************************************************/
		std::string sprite_name; // Name of sprite

		Texture* texture = nullptr; // The texture used for this sprite ( Bean: May be temporary because we can set a vector of textures in graphics to store all textures)
		Transform* transform = nullptr; // The transform of this sprite ( Bean: it refer to the component in the gameobject )

		glm::vec2 position{ 0 }; // Temporary variable to access the position
		glm::vec3 rotation{ 0 }; // Temporary variable for the rotation of the sprite
		glm::vec2 size{ 0 }; // The size of the sprite in pixels ( Bean: different from the scale of the gameobject )
		glm::vec4 color{ 0 }; // The blended color of this sprite
	};
}
#endif // !SPRITE_RENDERER_H
