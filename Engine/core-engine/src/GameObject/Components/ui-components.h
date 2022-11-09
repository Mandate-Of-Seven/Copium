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
#include "GameObject/Components/renderer-component.h"

#include <unordered_map>

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

	//Runs after InputSystem
	class IUIComponent
	{
		protected:
			Math::Vec2 offset;
			HorizontalAlignment hAlignment{HorizontalAlignment::Center};
			VerticalAlignment vAlignment{VerticalAlignment::Center};
	};

	class ButtonComponent final: public Component
	{
		//A screen space box collider
		public:
			/**************************************************************************/
			/*!
				\brief
					Constructs a ButtonComponent

				\param gameObj
					Owner of this
				\param _max
					Max of bounding box
				\param _min
					Min of bounding box
			*/
			/**************************************************************************/
			ButtonComponent(GameObject& _gameObj,Math::Vec2 _min = {-0.5,-0.5}, Math::Vec2 _max = {0.5,0.5});


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
			void inspector_view() {};

			/*******************************************************************************
			/*!
			*
			\brief
				Deep copies a ButtonComponent into another
			\param rhs
				Reference to another ButtonComponent
			\return
				Reference to this ButtonComponent
			*/
			/*******************************************************************************/
			ButtonComponent& operator=(const ButtonComponent& rhs);
		private:
			static const ButtonComponent* hoveredBtn;
			std::unordered_map<ButtonState, ButtonCallback> mapStateCallbacks;
			Math::Vec2 min;
			Math::Vec2 max;
			ButtonState state;
			ButtonState getInternalState() const;

	};

	class TextComponent final : public Component, IUIComponent
	{
		public:
			/**************************************************************************/
			/*!
				\brief
					Constructs a TextComponent
				\param gameObj
					Owner of this
			*/
			/**************************************************************************/
			TextComponent(GameObject& _gameObj);
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

			/*******************************************************************************
			/*!
			*
			\brief
				Deep copies a TextComponent into another
			\param rhs
				Reference to another TextComponent
			\return
				Reference to this TextComponent
			*/
			/*******************************************************************************/
			TextComponent& operator=(const TextComponent& rhs);

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
					font = Font::getFont(fontName);
				}
				if (_value.HasMember("V_Align"))
				{
					vAlignment = (VerticalAlignment)_value["V_Align"].GetInt();
				}
				if (_value.HasMember("Content"))
				{
					strcpy(content, _value["Content"].GetString());
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
			}
		private:
			char content[TEXT_BUFFER_SIZE];
			std::string fontName;
			Font* font;
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
			void render();
			const Sprite& get_sprite_renderer() const { return Sprite; }
			void set_sprite_renderer(const Sprite& _Sprite) { Sprite = _Sprite; }


			/*******************************************************************************
			/*!
			*
			\brief
				Deep copies a ImageComponent into another
			\param rhs
				Reference to another ImageComponent
			\return
				Reference to this ImageComponent
			*/
			/*******************************************************************************/
			ImageComponent& operator=(const ImageComponent& rhs);
		protected:
			Sprite Sprite;
		//Display an image
	};
}
#endif // !UI_COMPONENTS_H