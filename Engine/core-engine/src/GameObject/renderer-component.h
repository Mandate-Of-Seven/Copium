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
		/*******************************************************************************
		/*!
		*
		\brief
			Serialize this renderer component's data to the specified rapidjson Value

		\param _value
			reference to the rapidjson Value which the renderer component's data is to serialize its data to

		\param _doc
			reference to the rapidjson Document which is associated to the save file which the data is being saved to

		\return
			void

		*/
		/*******************************************************************************/
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);


		void inspector_view();

		const SpriteRenderer& get_sprite_renderer() const { return spriteRenderer; }
		void set_sprite_renderer(const SpriteRenderer& _spriteRenderer) { spriteRenderer = _spriteRenderer; }

		RendererComponent& operator=(const RendererComponent& rhs);
	protected:
		SpriteRenderer spriteRenderer;
	};
}
#endif // !RENDERER_COMPONENT_H