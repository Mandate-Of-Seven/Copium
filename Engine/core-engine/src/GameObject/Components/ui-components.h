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

	//Runs after InputSystem
	class IUIComponent
	{
		public:
			void virtual render() = 0;
		protected:
			Math::Vec2 offset;
			bool percentage;
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
			void inspector_view() {};
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
		private:
			std::string content;
			Font* font;
			Sprite Sprite;
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