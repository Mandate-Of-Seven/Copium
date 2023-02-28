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
#include <Graphics/graphics-system.h>
#include "glm/gtc/matrix_transform.hpp"

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
			if (MyInputSystem.is_mousebutton_pressed(0))
			{
				pHoveredBtn = &btn;
				if (btn.state == ButtonState::OnClick || btn.state == ButtonState::OnHeld)
					return ButtonState::OnHeld;
				return ButtonState::OnClick;
			}
			else if (btn.state == ButtonState::OnClick || btn.state == ButtonState::OnHeld)
			{
				return ButtonState::OnRelease;
			}
			return ButtonState::OnHover;
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
			//PRINT("UI: Hover " << btn.gameObj.name);
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

	GameObject* GetSelectedGameObject()
	{
		static GameObject* selectedGameObject{nullptr};
		selectedGameObject = nullptr;
		Scene* pScene = sceneManager.get_current_scene();
		if (!pScene)
			return nullptr;
		if (pScene->componentArrays.GetArray<Camera>().empty())
			return nullptr;

		GameObjectsPtrArray pGameObjs; // Possible selectable gameobjects
		Camera& gameCamera{pScene->componentArrays.GetArray<Camera>()[0]};
		glm::vec2 mousePosition = glm::vec3(gameCamera.get_game_ndc(), 0.f);
		//for (GameObject& gameObject : pScene->gameObjects)
		//{
		//	if (!gameObject.IsActive())
		//		continue;
		//	Transform& t = gameObject.transform;
		//	Math::Vec3 worldPos{ t.GetWorldPosition() };
		//	Math::Vec3 worldScale{ t.GetWorldScale() };

		//	glm::vec2 objPosition = { worldPos.x, worldPos.y };

		//	// Not Within bounds
		//	if (glm::distance(objPosition, mousePosition)
		//		> glm::length(gameCamera.get_dimension()))
		//		continue;

		//	glm::vec2 min, max;
		//	float tempX = 0.f, tempY = 0.f;

		//	if (gameObject.HasComponent<Button>())
		//	{
		//		Button* button = gameObject.GetComponent<Button>();

		//		if (!button->enabled)
		//			continue;
		//		//button->bounds.GetRelativeBounds(gameObject.transform.GetWorldPosition(), gameObject.transform.GetWorldScale());
		//		//min = bound.min;
		//		//max = bound.max;
		//	}

		//	if (gameObject.HasComponent<Image>())
		//	{
		//		Image* image = gameObject.GetComponent<Image>();

		//		if (!image->enabled)
		//			continue;
		//		Texture* texture = image->sprite.refTexture;

		//		if (texture != nullptr)
		//		{
		//			//tempX = tempScale.x * texture->get_pixel_width();
		//			//tempY = tempScale.y * texture->get_pixel_height();

		//			min = glm::vec2(objPosition.x - tempX * 0.5f, objPosition.y - tempY * 0.5f);
		//			max = glm::vec2(objPosition.x + tempX * 0.5f, objPosition.y + tempY * 0.5f);
		//		}

		//	}
		//	// Check AABB
		//	if (mousePosition.x > min.x && mousePosition.x < max.x)
		//	{
		//		if (mousePosition.y > min.y && mousePosition.y < max.y)
		//		{
		//			pGameObjs.push_back(&gameObject);
		//		}
		//	}
		//}


		if (pGameObjs.empty())
			return nullptr;
		// Ensure that the container is not empty
		// Sort base on depth value
		// If there is no selected gameobject
		// Update button behaviour for layered game objects
		// Update from back to front within layer
		std::vector<Layer>& sortingLayers{ MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers() };
		//Iterate through list of possible gameObjects

		for (GameObject* pGameObject : pGameObjs)
		{
			Button* button = pGameObject->GetComponent<Button>();
			SpriteRenderer* sr = pGameObject->GetComponent<SpriteRenderer>();
			SortingGroup * sg = pGameObject->GetComponent<SortingGroup>();
			SortingGroup * selectedSg{ pGameObject->GetComponent<SortingGroup>() };
			if ((sr && sr->enabled) || (button && button->enabled))
			{
				//Selected has no SG but new one has
				if (!selectedSg && sg)
				{
					selectedGameObject = pGameObject;
					continue;
				}
				//Object is on a higher layer
				else if (sg && selectedSg)
				{
					if (sg->sortingLayer > selectedSg->sortingLayer)
					{
						selectedGameObject = pGameObject;
						continue;
					}
					else if (sg->orderInLayer > selectedSg->orderInLayer)
					{
						selectedGameObject = pGameObject;
						continue;
					}
				}
				//Object is in same layer
				//No conditions met, skip this gameObject
			}
		}
		return selectedGameObject;
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


		GameObject* selected = GetSelectedGameObject();
		if (selected)
		{
			PRINT("SELECTED " << selected->name);
		}

		for (Button& button : pScene->componentArrays.GetArray<Button>())
		{
			if (!button.gameObj.IsActive() && !button.enabled)
			{
				button.state = ButtonState::None;
			}
			if (&button.gameObj == selected)
			{
				PRINT("HOVERING " << button.gameObj.name);
				button.state = GetButtonState(button);
				//Selected button state
			}
			ButtonBehavior(button);
		}

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