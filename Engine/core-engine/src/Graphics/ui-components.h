#pragma once

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
			UIButtonComponent(GameObject& _gameObj,Math::Vec2 _min = {-0.5,-0.5}, Math::Vec2 _max = {0.5,0.5});
			void update();
			void inspector_view() {};
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
			UITextComponent(GameObject& _gameObj);
			void inspector_view() {};
			void render();
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
			UIImageComponent(GameObject& _gameObj);
			void inspector_view();
			void render();
			const SpriteRenderer& get_sprite_renderer() const { return spriteRenderer; }
			void set_sprite_renderer(const SpriteRenderer& _spriteRenderer) { spriteRenderer = _spriteRenderer; }
			UIImageComponent& operator=(const UIImageComponent& rhs);
		protected:
			SpriteRenderer spriteRenderer;
		//Display an image
	};
}