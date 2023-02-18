#pragma once
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
		// Accessing Properties
		/* Sprite Information ***********************************************************/
		std::string sprite_name = "empty"; // Name of sprite

		Texture* refTexture; // Pointer to the texture
		uint64_t spriteID = 0; // The index of the sprite

		glm::bvec2 flip{ 0 };
		glm::vec2 size{ 0 }; // The size of the sprite in pixels ( Bean: different from the scale of the gameobject )
		glm::vec4 color{ 1 }; // The blended color of this sprite
	};

}
#endif // !SPRITE_RENDERER_H