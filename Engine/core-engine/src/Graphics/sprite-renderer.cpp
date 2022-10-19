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
	/*RTTR_REGISTRATION{
		using namespace rttr;
	registration::class_<SpriteRenderer>("Sprite Renderer")
		
	}*/

	void SpriteRenderer::deserialize(rapidjson::Value& _value)
	{
		
	}
}