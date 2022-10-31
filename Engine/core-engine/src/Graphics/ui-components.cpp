
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
	}

	void UIButtonComponent::update()
	{
		updateState();
		switch (state)
		{
		case UIButtonState::None:
		{
			PRINT("NONE");
			break;
		}
		case UIButtonState::OnHover:
		{
			PRINT("HOVER");
			break;
		}
		case UIButtonState::OnClick:
		{
			PRINT("CLICK");
			break;
		}
		case UIButtonState::OnRelease:
		{
			PRINT("RELEASE");
			break;
		}
		}
	}

	void UIButtonComponent::updateState()
	{
		static const Math::Vec3& pos{ gameObj.Transform().position };
		Collision::AABB newBounds{ pos + min, pos + max };
		glm::vec2 scenePos = EditorSystem::Instance()->get_camera()->get_ndc();
		if (Collision::static_collision_pointrect(scenePos, newBounds))
		{
			if (inputSystem.is_mousebutton_pressed(0))
			{
				state = UIButtonState::OnClick;
				return;
			}
			state = UIButtonState::OnHover;
			return;
		}
		else
		{
			if (!inputSystem.is_mousebutton_pressed(0))
			{
				state = UIButtonState::OnRelease;
				return;
			}
			state = UIButtonState::OnClick;
			return;
		}
		state = UIButtonState::None;
	}

	UITextComponent::UITextComponent(GameObject& _gameObj)
		: UIComponent(_gameObj, ComponentType::UIText)
	{

	}

	void UITextComponent::render()
	{

	}

	UIImageComponent::UIImageComponent(GameObject& _gameObj)
		: UIComponent(_gameObj, ComponentType::UIImage)
	{

	}

	void UIImageComponent::render()
	{

	}
}