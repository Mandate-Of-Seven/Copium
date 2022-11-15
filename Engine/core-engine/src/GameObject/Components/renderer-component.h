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

#include "GameObject/Components/component.h"
#include "GameObject/Components/transform-component.h"
#include "Graphics/sprite-renderer.h"
#include "GameObject/game-object.h"

namespace Copium
{
	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer(GameObject& _gameObj);

		/*******************************************************************************
		/*!
		*
		\brief
			Deserialize this renderer component's data from the specified rapidjson Value

		\param _value
			reference to the rapidjson Value which the renderer component's data deserializes its data from

		\return
			void

		*/
		/*******************************************************************************/
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

		/*******************************************************************************
		/*!
		*
		\brief
			Displays the inspector view with its fields

		*/
		/*******************************************************************************/
		void inspector_view();

		Sprite& get_sprite_renderer() { return sprite; }
		void set_sprite_renderer(const Sprite& _sprite) { sprite = _sprite; }

		Component* clone(GameObject& _gameObj) const;
	protected:
		Sprite sprite;
	};
}
#endif // !RENDERER_COMPONENT_H