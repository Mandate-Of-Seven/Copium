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

	ButtonState GetButtonState(Button& btn, GameObject* selectedGameObject)
	{
		if (selectedGameObject != &btn.gameObj)
		{
			if (btn.state == ButtonState::None || btn.state == ButtonState::OnRelease)
			{
				return ButtonState::None;
			}
		}
		glm::vec2 scenePos = MySceneManager.mainCamera->get_game_ndc();
		Transform& transform{ btn.gameObj.transform };

		if (pHoveredBtn == nullptr)
		{
			//PRINT("GETTING BUTTON STATE");
			//PRINT(scenePos.x << " , " << scenePos.y);
			if (static_collision_pointrect(scenePos, btn.bounds.GetRelativeBounds(transform.GetWorldPosition(), transform.GetWorldScale())))
			{
				//PRINT("COLLIDED");
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
			//PRINT("UI: Clicked " << btn.gameObj.name);
			if (btn.targetGraphic)
			{
				btn.targetGraphic->layeredColor = Linear(btn.previousColor, btn.clickedColor, btn.timer / btn.fadeDuration);
			}
			break;
		}
		case ButtonState::OnHover:
		{
			PRINT("UI: Hover " << btn.gameObj.name);
			if (btn.targetGraphic)
			{
				btn.targetGraphic->layeredColor = Linear(btn.previousColor, btn.hoverColor, btn.timer / btn.fadeDuration);
				PRINT("R " << btn.targetGraphic->layeredColor.r);
				PRINT("G " << btn.targetGraphic->layeredColor.g);
				PRINT("B " << btn.targetGraphic->layeredColor.b);
				PRINT("A " << btn.targetGraphic->layeredColor.a);
			}
			break;
		}
		case ButtonState::OnHeld:
		{
			//PRINT("UI: Held " << btn.gameObj.name);
			if (btn.targetGraphic)
			{
				btn.targetGraphic->layeredColor = Linear(btn.previousColor, btn.clickedColor, btn.timer / btn.fadeDuration);
			}
			break;
		}
		case ButtonState::OnRelease:
		{
			//PRINT("UI: Released " << btn.gameObj.name);
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
		glm::vec2 mousePosition = gameCamera.get_game_ndc();
		for (GameObject& gameObject : pScene->gameObjects)
		{
			if (!gameObject.IsActive())
				continue;
			Transform& t = gameObject.transform;
			Math::Vec3 worldPos{ t.position };
			Math::Vec3 worldScale{ t.scale };

			if (t.HasParent())
			{
				Transform* tempObj = t.parent;
				while (tempObj)
				{
					glm::vec3 tempPos = tempObj->position.glmVec3;
					glm::mat4 pTranslate = glm::translate(glm::mat4(1.f), tempPos);

					float rot = glm::radians(tempObj->rotation.z);
					glm::mat4 pRotate = {
					glm::vec4(cos(rot), sin(rot), 0.f, 0.f),
					glm::vec4(-sin(rot), cos(rot), 0.f, 0.f),
					glm::vec4(0.f, 0.f, 1.f, 0.f),
					glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					glm::vec3 size = tempObj->scale.glmVec3;
					glm::mat4 pScale = {
						glm::vec4(size.x, 0.f, 0.f, 0.f),
						glm::vec4(0.f, size.y, 0.f, 0.f),
						glm::vec4(0.f, 0.f, 1.f, 0.f),
						glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					glm::mat4 pTransform = pTranslate * pRotate * pScale;

					worldPos.glmVec3 = glm::vec3(pTransform * glm::vec4(worldPos.glmVec3, 1.f));

					worldScale.glmVec3 *= tempObj->scale.glmVec3;

					tempObj = tempObj->parent;
				}
			}

			glm::vec2 objPosition = { worldPos.x, worldPos.y };

			// Not Within bounds // NEED A BETTER CHECK THAT INCLUDES THE BOUNDS
			//if (glm::distance(objPosition, mousePosition)
			//	> glm::length(gameCamera.get_dimension()))
			//	continue;
			AABB bounds{};
			Button* button = gameObject.GetComponent<Button>();
			Image* image = gameObject.GetComponent<Image>();

			if (button)
			{
				if (!button->enabled)
					continue;
				bounds = button->bounds;
				AABB bound = bounds.GetRelativeBounds(worldPos, worldScale);
				if (static_collision_pointrect(mousePosition, bound))
				{
					pGameObjs.push_back(&gameObject);
					continue;
				}
			}

			if (image)
			{
				if (!image->enabled)
					continue;
				Texture* texture = image->sprite.refTexture;
				if (texture != nullptr)
				{
					//tempX = tempScale.x * texture->get_pixel_width();
					//tempY = tempScale.y * texture->get_pixel_height();
					bounds.max = { texture->get_pixel_width() / 2.f,texture->get_pixel_height() / 2.f};
					bounds.min = { -texture->get_pixel_width() / 2.f , -texture->get_pixel_height() / 2.f};
				}
				AABB bound = bounds.GetRelativeBounds(worldPos, worldScale);
				if (static_collision_pointrect(mousePosition, bound))
					pGameObjs.push_back(&gameObject);
			}
			
			// Check AABB
		}


		if (pGameObjs.empty())
		{
			return nullptr;
		}
		// Ensure that the container is not empty
		// Sort base on depth value
		// If there is no selected gameobject
		// Update button behaviour for layered game objects
		// Update from back to front within layer
		std::vector<Layer>& sortingLayers{ MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers() };
		//Iterate through list of possible gameObjects

		for (GameObject* pGameObject : pGameObjs)
		{
			if (!selectedGameObject)
			{
				selectedGameObject = pGameObject;
				continue;
			}
			SortingGroup* sg = pGameObject->GetComponent<SortingGroup>();
			SortingGroup * selectedSg{ selectedGameObject->GetComponent<SortingGroup>() };
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
				else if (sg->sortingLayer == selectedSg->sortingLayer && sg->orderInLayer > selectedSg->orderInLayer)
				{
					selectedGameObject = pGameObject;
					continue;
				}
			}
			//Object is in same layer
			//No conditions met, skip this gameObject
		}
		return selectedGameObject;
	}


	void LogicSystem::init()
	{
		messageSystem.subscribe(MESSAGE_TYPE::MT_START_PREVIEW,this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
		MyEventSystem->subscribe(this, &LogicSystem::CallbackSceneLinked);
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
				//	return
		}

		GameObject* selected = GetSelectedGameObject();

		//if (selected)
		//	PRINT(selected->name);
		for (Button& button : pScene->componentArrays.GetArray<Button>())
		{
			if (!button.enabled || !button.gameObj.IsActive())
			{
				button.state = ButtonState::None;
				if (&button == pHoveredBtn)
					pHoveredBtn = nullptr;
				continue;
			}
			button.state = GetButtonState(button,selected);
			ButtonBehavior(button);
		}
	}

	void LogicSystem::exit()
	{
	}

	void LogicSystem::CallbackSceneLinked(SceneLinkedEvent* pEvent)
	{
		pHoveredBtn = nullptr;
		//MT_START_PREVIEW
		if (!inPlayMode)
			return;
		//PRINT("CALLED START AGAIN!");
		Scene& scene = pEvent->scene;
		for (Script& script : scene.componentArrays.GetArray<Script>())
		{
			MyEventSystem->publish(new ScriptInvokeMethodEvent(script, "Awake"));
		}

		for (Script& script : scene.componentArrays.GetArray<Script>())
		{
			MyEventSystem->publish(new ScriptInvokeMethodEvent(script, "Start"));
		}
		timeElasped = MyFrameRateController.getDt();
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