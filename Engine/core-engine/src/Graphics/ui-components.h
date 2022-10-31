#pragma once

#include "GameObject/component.h"
#include "Math/math-library.h"

namespace Copium
{
	enum class UIButtonState
	{
		None,
		OnHover,
		OnClick,
		OnRelease,
	};

	//Runs after InputSystem
	class UIComponent : public Component
	{
		public:
			void virtual render() = 0;
		protected:
			UIComponent(GameObject& _gameObj, ComponentType _componentType);
		private:
			Math::Vec2 offset;
			bool percentage;
	};

	class UIButtonComponent final: public Component
	{
		//A screen space box collider
		public:
			UIButtonComponent(GameObject& _gameObj,Math::Vec2 _min = {-0.5,-0.5}, Math::Vec2 _max = {0.5,0.5});
			void update();
			void inspector_view() {};
		private:		
			Math::Vec2 min;
			Math::Vec2 max;
			UIButtonState state;
			void updateState();
	};

	class UITextComponent final : public UIComponent
	{
		public:
			UITextComponent(GameObject& _gameObj);
			void inspector_view() {};
			void render();
		//Display a text
	};

	class UIImageComponent final : public UIComponent
	{
		public:
			UIImageComponent(GameObject& _gameObj);
			void inspector_view() {};
			void render();
		//Display an image
	};
}