/*!***************************************************************************************
\file			scripting-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	This file holds the declaration of UI components

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "GameObject/Components/component.h"
#include "Math/math-library.h"
#include "Graphics/fonts.h"
#include <Physics/collision.h>
#include <unordered_map>
#include <Graphics/sprite.h>

#define TEXT_BUFFER_SIZE 128

namespace Copium
{
	using ButtonCallback = void (*)();

	enum class ButtonState
	{
		OnHover,
		OnClick,
		OnRelease,
		None,
	};

	enum class HorizontalAlignment : int
	{
		Left = 0,
		Center,
		Right,
	};

	enum class VerticalAlignment : int
	{
		Top = 0,
		Center,
		Bottom,
	};

	class Text;
	class Image;

	//Runs after InputSystem
	class IUIComponent
	{
		protected:
			Math::Vec2 offset;
			HorizontalAlignment hAlignment{HorizontalAlignment::Center};
			VerticalAlignment vAlignment{VerticalAlignment::Center};
			glm::fvec4 color{1.f};
			glm::fvec4 layeredColor{ 1.f };
	};

	class Button final: public Component
	{
		//A screen space box collider
		public:
			/**************************************************************************/
			/*!
				\brief
					Constructs a Button

				\param gameObj
					Owner of this
				\param _max
					Max of bounding box
				\param _min
					Min of bounding box
			*/
			/**************************************************************************/
			Button(GameObject& _gameObj,Math::Vec2 _min = {-0.5,-0.5}, Math::Vec2 _max = {0.5,0.5});


			/*******************************************************************************
			/*!
			*
			\brief
				Logic function to check for collision with mouse pos

			*/
			/*******************************************************************************/
			void update();

			/*******************************************************************************
			/*!
			*
			\brief
				Displays the inspector view with its fields

			*/
			/*******************************************************************************/
			void inspector_view();

			Button& operator=(const Button& rhs);

			Component* clone(GameObject& _gameObj) const;

			void deserialize(rapidjson::Value& _value)
			{
			}

			const AABB& getRelativeBounds() const;

			void serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
			{
				rapidjson::Value type;
				std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
				type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
				_value.AddMember("Type", type, _doc.GetAllocator());
			}
		private:
			static const Button* hoveredBtn;
			std::unordered_map<ButtonState, ButtonCallback> mapStateCallbacks;
			AABB bounds;
			void updateBounds();
			AABB relativeBounds;
			ButtonState state;
			ButtonState getInternalState() const;
			glm::fvec4 normalColor;
			glm::fvec4 hoverColor;
			glm::fvec4 clickedColor;
			Text* targetGraphic;
			ButtonState previousState{ButtonState::None};
			glm::fvec4 previousColor;
			float timer{0};
			float fadeDuration{0.1};
	};

	class Text final : public Component, IUIComponent
	{
		public:

			char content[TEXT_BUFFER_SIZE];
			/**************************************************************************/
			/*!
				\brief
					Constructs a Text
				\param gameObj
					Owner of this
			*/
			/**************************************************************************/
			Text(GameObject& _gameObj);
			/*******************************************************************************
			/*!
			*
			\brief
				Displays the inspector view with its fields

			*/
			/*******************************************************************************/
			void inspector_view();
			/*******************************************************************************
			/*!
			*
			\brief
				Called by graphics to display this

			*/
			/*******************************************************************************/
			void render();

			Component* clone(GameObject& _gameObj) const;

			void deserialize(rapidjson::Value& _value)
			{
				if (_value.HasMember("FontName"))
				{
					fontName = _value["FontName"].GetString();
					font = Font::getFont(fontName);
				}
				if (_value.HasMember("H_Align"))
				{
					hAlignment = (HorizontalAlignment)_value["H_Align"].GetInt();
				}
				if (_value.HasMember("V_Align"))
				{
					vAlignment = (VerticalAlignment)_value["V_Align"].GetInt();
				}
				if (_value.HasMember("Content"))
				{
					strcpy(content, _value["Content"].GetString());
				}
				if (_value.HasMember("Font Size"))
				{
					fSize = _value["Font Size"].GetFloat();
				}
				if (_value.HasMember("r"))
				{
					color.r = _value["r"].GetFloat();
				}
				if (_value.HasMember("g"))
				{
					color.g = _value["g"].GetFloat();
				}
				if (_value.HasMember("b"))
				{
					color.b = _value["b"].GetFloat();

				}
				if (_value.HasMember("a"))
				{
					color.a = _value["a"].GetFloat();
				}
			}

			void serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
			{
				rapidjson::Value type;
				std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
				type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
				_value.AddMember("Type", type, _doc.GetAllocator());

				type.SetString(fontName.c_str(), rapidjson::SizeType(fontName.length()), _doc.GetAllocator());
				_value.AddMember("FontName", type, _doc.GetAllocator());
				_value.AddMember("H_Align", (int)hAlignment, _doc.GetAllocator());
				_value.AddMember("V_Align", (int)vAlignment, _doc.GetAllocator());

				type.SetString(content, rapidjson::SizeType(strlen(content)), _doc.GetAllocator());
				_value.AddMember("Content", type, _doc.GetAllocator());

				_value.AddMember("Font Size", fSize, _doc.GetAllocator());
				_value.AddMember("r", color.r, _doc.GetAllocator());
				_value.AddMember("g", color.g, _doc.GetAllocator());
				_value.AddMember("b", color.b, _doc.GetAllocator());
				_value.AddMember("a", color.a, _doc.GetAllocator());
			}
		private:
			std::string fontName;
			Font* font;
			float fSize;
			friend class Button;
		//Display a text
	};

	class ImageComponent final : public Component, IUIComponent
	{
		public:
			/**************************************************************************/
			/*!
				\brief
					Constructs a ImageComponent
				\param gameObj
					Owner of this
			*/
			/**************************************************************************/
			ImageComponent(GameObject& _gameObj);
			/*******************************************************************************
			/*!
			*
			\brief
				Displays the inspector view with its fields

			*/
			/*******************************************************************************/
			void inspector_view();
			/*******************************************************************************
			/*!
			*
			\brief
				Called by graphics to display this

			*/
			/*******************************************************************************/
			glm::fvec2 Offset();
			Sprite& get_sprite_renderer() { return sprite; }
			void set_sprite_renderer(Sprite& _sprite) { sprite = _sprite; }

			Component* clone(GameObject& _gameObj) const;

			void deserialize(rapidjson::Value& _value)
			{
				if (_value.HasMember("H_Align"))
				{
					hAlignment = (HorizontalAlignment)_value["H_Align"].GetInt();
				}
				if (_value.HasMember("V_Align"))
				{
					vAlignment = (VerticalAlignment)_value["V_Align"].GetInt();
				}
				sprite.deserialize(_value);
			}

			void serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
			{
				rapidjson::Value type;
				std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
				type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
				_value.AddMember("Type", type, _doc.GetAllocator());

				_value.AddMember("H_Align", (int)hAlignment, _doc.GetAllocator());
				_value.AddMember("V_Align", (int)vAlignment, _doc.GetAllocator());
				sprite.serialize(_value,_doc);
			}
		protected:
			Sprite sprite;
			friend class Button;
		//Display an image
	};
}
#endif // !UI_COMPONENTS_H