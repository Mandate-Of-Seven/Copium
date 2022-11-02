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

#include "GameObject/component.h"
#include "Math/math-library.h"
#include "Graphics/fonts.h"
#include "GameObject/renderer-component.h"

#include <unordered_map>
namespace Copium
{
	using UIButtonCallback = void (*)();

	enum class UIButtonState
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

	class UIButtonComponent final: public Component
	{
		//A screen space box collider
		public:
			/**************************************************************************/
			/*!
				\brief
					Constructs a UIButtonComponent

				\param gameObj
					Owner of this
				\param _max
					Max of bounding box
				\param _min
					Min of bounding box
			*/
			/**************************************************************************/
			UIButtonComponent(GameObject& _gameObj,Math::Vec2 _min = {-0.5,-0.5}, Math::Vec2 _max = {0.5,0.5});


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
				Deep copies a UIButtonComponent into another
			\param rhs
				Reference to another UIButtonComponent
			\return
				Reference to this UIButtonComponent
			*/
			/*******************************************************************************/
			UIButtonComponent& operator=(const UIButtonComponent& rhs);
		private:
			static const UIButtonComponent* hoveredBtn;
			std::unordered_map<UIButtonState, UIButtonCallback> mapStateCallbacks;
			Math::Vec2 min;
			Math::Vec2 max;
			UIButtonState state;
			UIButtonState getInternalState() const;

	};

	class UITextComponent final : public Component, IUIComponent
	{
		public:
			/**************************************************************************/
			/*!
				\brief
					Constructs a UITextComponent
				\param gameObj
					Owner of this
			*/
			/**************************************************************************/
			UITextComponent(GameObject& _gameObj);
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
				Deep copies a UITextComponent into another
			\param rhs
				Reference to another UITextComponent
			\return
				Reference to this UITextComponent
			*/
			/*******************************************************************************/
			UITextComponent& operator=(const UITextComponent& rhs);
		private:
			std::string content;
			Font* font;
			SpriteRenderer spriteRenderer;
		//Display a text
	};

	class UIImageComponent final : public Component, IUIComponent
	{
		public:
			/**************************************************************************/
			/*!
				\brief
					Constructs a UIImageComponent
				\param gameObj
					Owner of this
			*/
			/**************************************************************************/
			UIImageComponent(GameObject& _gameObj);
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
			const SpriteRenderer& get_sprite_renderer() const { return spriteRenderer; }
			void set_sprite_renderer(const SpriteRenderer& _spriteRenderer) { spriteRenderer = _spriteRenderer; }


			/*******************************************************************************
			/*!
			*
			\brief
				Deep copies a UIImageComponent into another
			\param rhs
				Reference to another UIImageComponent
			\return
				Reference to this UIImageComponent
			*/
			/*******************************************************************************/
			UIImageComponent& operator=(const UIImageComponent& rhs);
		protected:
			SpriteRenderer spriteRenderer;
		//Display an image
	};
}
#endif // !UI_COMPONENTS_H