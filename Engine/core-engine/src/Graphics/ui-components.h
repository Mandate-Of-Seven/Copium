#pragma once

#include "GameObject/component.h"
#include "Math/math-library.h"
#include "Graphics/fonts.h"

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
			static const UIButtonComponent* hoveredBtn;
			std::unordered_map<UIButtonState, UIButtonCallback> mapStateCallbacks;
			Math::Vec2 min;
			Math::Vec2 max;
			UIButtonState state;
			UIButtonState getInternalState() const;

	};

	class UITextComponent final : public UIComponent
	{
		public:
			UITextComponent(GameObject& _gameObj);
			void inspector_view() {};
			void render();
		private:
			std::string content;
			Font* font;
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