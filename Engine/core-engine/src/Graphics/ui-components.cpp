
#include "pch.h"
#include "Windows/windows-input.h"
#include "Graphics/ui-components.h"
#include "GameObject/game-object.h"
#include "Physics/collision.h"
#include "Editor/editor-system.h"

namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
}

namespace Copium
{
	const UIButtonComponent* UIButtonComponent::hoveredBtn{nullptr};

	UIComponent::UIComponent(GameObject& _gameObj, ComponentType _componentType) 
		: Component(_gameObj,_componentType)
	{

	}

	UIButtonComponent::UIButtonComponent(GameObject& _gameObj,Math::Vec2 _min, Math::Vec2 _max) 
		: Component(_gameObj, ComponentType::UIButton)
	{
		min = _min;
		max = _max;
		state = UIButtonState::None;
		for (int i = 0; i <= int(UIButtonState::None); ++i)
		{
			mapStateCallbacks.insert({UIButtonState(i),nullptr});
		}
	}


	void UIButtonComponent::update()
	{
		state = getInternalState();

		UIButtonCallback callback = mapStateCallbacks[state];
		if (callback != nullptr)
		{
			callback();
		}
		//switch (state)
		//{
		//case UIButtonState::OnClick:
		//{

		//	PRINT("Clicking");
		//	break;
		//}
		//case UIButtonState::OnHover:
		//{
		//	PRINT("Hovering");
		//	break;
		//}
		//case UIButtonState::OnRelease:
		//{
		//	PRINT("Released");
		//	break;
		//}
		//case UIButtonState::None:
		//{
		//	PRINT("None");
		//	break;
		//}
		//}
	}

	UIButtonState UIButtonComponent::getInternalState() const
	{
		static const Math::Vec3& pos{ gameObj.Transform().position };
		Collision::AABB newBounds{ pos + min, pos + max };
		glm::vec2 scenePos = EditorSystem::Instance()->get_camera()->get_ndc();
		if (hoveredBtn == nullptr)
		{
			if (Collision::static_collision_pointrect(scenePos, newBounds))
			{
				if (inputSystem.is_mousebutton_pressed(0))
				{
					hoveredBtn = this;
					return UIButtonState::OnClick;
				}
				return UIButtonState::OnHover;
			}
		}
		else if (hoveredBtn == this)
		{
			if (!inputSystem.is_mousebutton_pressed(0))
			{
				hoveredBtn = nullptr;
				return UIButtonState::OnRelease;
			}
			return UIButtonState::OnClick;
		}
		return UIButtonState::None;
	}

	UITextComponent::UITextComponent(GameObject& _gameObj)
		: UIComponent(_gameObj, ComponentType::UIText), font{Font::getFont("corbel")}
	{
	}

	void UITextComponent::render()
	{
		if (!font)
			return;
		font->draw_text("Hello bby", gameObj.Transform().position.to_glm(), { 1.f, 1.f, 1.f, 1.f }, 1.f, 0);
	}

	UIImageComponent::UIImageComponent(GameObject& _gameObj)
		: UIComponent(_gameObj, ComponentType::UIImage)
	{

	}

	void UIImageComponent::render()
	{

	}
}