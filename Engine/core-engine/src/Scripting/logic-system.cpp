/*!***************************************************************************************
\file			logic-system.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	This file defines the functions for the Logic System

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "GameObject/game-object.h"
#include "Scripting/logic-system.h"
#include <Windows/windows-input.h>
#include "SceneManager/scene-manager.h"
#include "Messaging/message-system.h"
#include <Windows/windows-system.h>
#include <Debugging/frame-rate-controller.h>
#include <Events/events-system.h>
#include <GameObject/components.h>
#include <Utilities/easing.h>
#include "Editor/editor-system.h"

namespace Copium
{
	namespace
	{
		SceneManager& sceneManager {*SceneManager::Instance()};
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
		double timeElasped;
		bool inPlayMode{ false };
		Button* pHoveredBtn{ nullptr };
	}

	ButtonState GetButtonState(Button& btn)
	{
		glm::vec2 scenePos = MySceneManager.mainCamera->get_game_ndc();
		Transform& transform{ btn.gameObj.transform };
		if (pHoveredBtn == nullptr)
		{
			if (static_collision_pointrect(scenePos, btn.bounds.GetRelativeBounds(transform.GetWorldPosition(),transform.GetWorldScale())))
			{
				if (MyInputSystem.is_mousebutton_pressed(0))
				{
					if (pHoveredBtn)
					{
						pHoveredBtn = nullptr;
					}
					pHoveredBtn = &btn;
					if (btn.state == ButtonState::OnClick || btn.state == ButtonState::OnHeld)
						return ButtonState::OnHeld;
					return ButtonState::OnClick;
				}
				return ButtonState::OnHover;
			}
		}
		else if (pHoveredBtn == &btn)
		{
			if (!MyInputSystem.is_mousebutton_pressed(0))
			{
				pHoveredBtn->state = ButtonState::None;
				pHoveredBtn = nullptr;
				return ButtonState::OnRelease;
			}
			if (btn.state == ButtonState::OnClick || btn.state == ButtonState::OnHeld)
				return ButtonState::OnHeld;
			return ButtonState::OnClick;
		}
		return ButtonState::None;
	}

	void ButtonBehavior(Button& btn)
	{
		btn.state = GetButtonState(btn);
		if (btn.previousState != btn.state)
		{
			btn.timer = 0;
			btn.previousState = btn.state;
			if (btn.targetGraphic)
				btn.previousColor = btn.targetGraphic->layeredColor;
		}
		switch (btn.state)
		{
		case ButtonState::OnClick:
		{
			PRINT("UI: Clicked " << btn.gameObj.name);
			if (btn.targetGraphic)
			{
				btn.targetGraphic->layeredColor = Linear(btn.previousColor, btn.clickedColor, btn.timer / btn.fadeDuration);
			}
			Script* script = btn.gameObj.GetComponent<Script>();
			if (script)
			{
				MyEventSystem->publish(new ScriptInvokeMethodEvent(*script, btn.callbackName, nullptr, 0));
			}
			break;
		}
		case ButtonState::OnHover:
		{
			PRINT("UI: Hover " << btn.gameObj.name);
			if (btn.targetGraphic)
			{
				btn.targetGraphic->layeredColor = Linear(btn.previousColor, btn.hoverColor, btn.timer / btn.fadeDuration);
			}
			break;
		}
		case ButtonState::OnHeld:
		{
			PRINT("UI: Held " << btn.gameObj.name);
			break;
		}
		case ButtonState::OnRelease:
		{
			PRINT("UI: Released " << btn.gameObj.name);
			break;
		}
		default:
		{
			if (btn.targetGraphic)
			{
				btn.targetGraphic->layeredColor = Linear(btn.previousColor, btn.normalColor, btn.timer / btn.fadeDuration);
			}
			break;
		}
		}
		if (btn.targetGraphic == nullptr)
			return;
		if (btn.timer < btn.fadeDuration)
			btn.timer += (float)MyFrameRateController.getDt();
		else if (btn.timer > btn.fadeDuration)
			btn.timer = btn.fadeDuration;
	}

	void HoverFrontButton()
	{
		// Update button behaviour for layered game objects
		// Update from back to front within layer
		Scene* pScene = sceneManager.get_current_scene();
		if (!pScene)
			return;
		std::vector<Layer>& sortingLayers{ MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers() };
		if (!sortingLayers.empty())
			for (size_t i{ sortingLayers.size() - 1}; i != 0; --i)
			{
				Layer& l = sortingLayers[i];
				for (size_t j{ l.gameObjects.size() - 1 }; j != 0; --j)
				{
					GameObject* go = l.gameObjects[j];
					if (!go)
						continue;
					if (!go->IsActive() || !go->HasComponent<Button>())
						continue;
					Button* button = go->GetComponent<Button>();
					if (!button->enabled)
						continue;
					ButtonBehavior(*button);
				}
			}

		for (Button& button : pScene->componentArrays.GetArray<Button>())
		{
			if (!button.enabled)
				continue;
			if (!button.gameObj.IsActive())
				continue;
			if (button.gameObj.HasComponent<SortingGroup>())
				continue;
			ButtonBehavior(button);
			if (pHoveredBtn == &button)
			{
				return;
			}
		}
		if (pHoveredBtn && (!pHoveredBtn->gameObj.IsActive() || !pHoveredBtn->enabled))
		{
			pHoveredBtn->state = ButtonState::None;
			pHoveredBtn = nullptr;
		}
	}

	void LogicSystem::init()
	{
		messageSystem.subscribe(MESSAGE_TYPE::MT_START_PREVIEW,this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
		systemFlags |= FLAG_RUN_ON_PLAY;
	}

	void LogicSystem::update()
	{
		Scene* pScene = sceneManager.get_current_scene();
		if (pScene == nullptr)
			return;

		for (Script& script : pScene->componentArrays.GetArray<Script>())
		{
			if (!script.enabled)
				continue;
			if (!script.gameObj.IsActive())
				continue;
			MyEventSystem->publish(new ScriptInvokeMethodEvent(script, "Update"));
			if (pScene != sceneManager.get_current_scene())
				return;

				//for (size_t j = 0; j < MyFrameRateController.getSteps(); ++j)
				//{
				//	MyEventSystem->publish(new ScriptInvokeMethodEvent(*pScript,"FixedUpdate"));
				//}
				//if (pScene != sceneManager.get_current_scene())
				//	return;
		}


		HoverFrontButton();
	}

	void LogicSystem::exit()
	{
	}

	void LogicSystem::handleMessage(MESSAGE_TYPE mType)
	{
		//MT_START_PREVIEW
		if (mType == MESSAGE_TYPE::MT_START_PREVIEW)
		{
			inPlayMode = true;
			Scene* pScene = sceneManager.get_current_scene();
			if (pScene == nullptr)
				return;
			for (Script& script : pScene->componentArrays.GetArray<Script>() )
			{
				MyEventSystem->publish(new ScriptInvokeMethodEvent(script, "Awake"));
			}

			for (Script& script : pScene->componentArrays.GetArray<Script>())
			{
				MyEventSystem->publish(new ScriptInvokeMethodEvent(script, "Start"));
			}
			timeElasped = MyFrameRateController.getDt();
		}
		else
		{
			inPlayMode = false;
		}
		pHoveredBtn = nullptr;
	}
}