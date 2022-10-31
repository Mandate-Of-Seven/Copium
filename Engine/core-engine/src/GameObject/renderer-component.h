/*!***************************************************************************************
\file			renderer-component.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	This file holds the declaration of the Renderer component class.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include "GameObject/component.h"
#include "GameObject/transform-component.h"
#include "Graphics/sprite-renderer.h"
#include "GameObject/game-object.h"

namespace Copium
{
	class RendererComponent : public Component
	{
	public:
		RendererComponent(GameObject& _gameObj);

		void deserialize(rapidjson::Value& _value);
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);


        void inspector_view();

		const Copium::SpriteRenderer& get_sprite_renderer() const { return spriteRenderer; }
		void set_sprite_renderer(const Copium::SpriteRenderer& _spriteRenderer) { spriteRenderer = _spriteRenderer; }

	protected:
		Copium::SpriteRenderer spriteRenderer;
	};
}

#endif // !RENDERER_COMPONENT_H