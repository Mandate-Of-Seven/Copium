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

	// Bean: Temporary forward declaration of BaseCamera class to render text
	class BaseCamera;
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
			glm::fvec4 layeredColor{ 0.f };
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

			void deserializeLink (rapidjson::Value& _value);

			void previewLink(Component* rhs);

			Button& operator=(const Button& rhs);

			Component* clone(GameObject& _gameObj) const;

			/*******************************************************************************
			/*!
			*
			\brief
				Deserialize this button component's data from the specified rapidjson Value

			\param _value
				reference to the rapidjson Value which the button component's data deserializes its data from

			\return
				void

			*/
			/*******************************************************************************/
			void deserialize(rapidjson::Value& _value);
			/*******************************************************************************
			/*!
			*
			\brief
				Serialize this button component's data to the specified rapidjson Value

			\param _value
				reference to the rapidjson Value which the button component's data is to serialize its data to

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
				Set the target graphic for this button

			\param _txt
				ptr to the Text component which will serve as the target graphic for this Button component

			\return
				void
			*/
			/*******************************************************************************/
			void set_targetgraphic(Text* _txt);
			/*******************************************************************************
			/*!
			*
			\brief
				Get the target graphic for this button

			\return
				the target graphic attached to this button component
			*/
			/*******************************************************************************/
			Text* get_targetgraphic();

			const AABB& getRelativeBounds() const;

		private:
			static const Button* hoveredBtn;
			std::string callbackName;
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
			float fadeDuration{0.1f};
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
			void render(BaseCamera* _camera);

			Component* clone(GameObject& _gameObj) const;

			void deserialize(rapidjson::Value& _value);
			void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);
		private:
			std::string fontName;
			Font* font;
			float fSize;
			friend class Button;
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
				if (_value.HasMember("ID"))
				{
					id = _value["ID"].GetUint64();
				}

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

				_value.AddMember("ID", id, _doc.GetAllocator());

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