/*!***************************************************************************************
\file			sprite-renderer.cpp
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
#include "pch.h"
#include "Graphics/sprite-renderer.h"

#include <rttr/registration>

namespace Copium::Graphics
{
	RTTR_REGISTRATION{
		using namespace rttr;
	/*registration::class_<glm::vec4>("Vec4")
		.property("X", &glm::vec4::x)
		.property("Y", &glm::vec4::y)
		.property("Z", &glm::vec4::z)
		.property("W", &glm::vec4::w);*/

	registration::class_<SpriteRenderer>("Sprite Renderer")
		.property("Sprite ID", &SpriteRenderer::get_sprite_id, &SpriteRenderer::set_sprite_id);
		//.property("Color", &SpriteRenderer::get_color, &SpriteRenderer::set_color);
	}

	bool SpriteRenderer::deserialize(rapidjson::Value& _value)
	{
		if (!_value.HasMember("Sprite ID"))
			return false;

		spriteID = _value["Sprite ID"].GetUint();

		/*if (!_value.HasMember("Color"))
			return false;

		rapidjson::Value& _v = _value["Color"].GetObj();
		color.x = _v["X"].GetFloat();
		color.y = _v["Y"].GetFloat();
		color.w = _v["Z"].GetFloat();
		color.z = _v["W"].GetFloat();*/

		return true;
	}
}