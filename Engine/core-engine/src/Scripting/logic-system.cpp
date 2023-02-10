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
#include "GameObject/Components/script-component.h"
#include "SceneManager/scene-manager.h"
#include "Messaging/message-system.h"
#include <Windows/windows-system.h>
#include <Debugging/frame-rate-controller.h>
#include "GameObject/Components/ui-components.h"
#include <Events/events-system.h>

namespace Copium
{
	namespace
	{
		SceneManager& sceneManager {*SceneManager::Instance()};
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
		std::vector<GameObject*>* gameObjects;
		double timeElasped;
		std::list<Script*> scriptComponents;
		bool inPlayMode{ false };
	}

	void LogicSystem::init()
	{
		messageSystem.subscribe(MESSAGE_TYPE::MT_START_PREVIEW,this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
		systemFlags |= FLAG_RUN_ON_PLAY;
		MyEventSystem->subscribe(this, &LogicSystem::CallbackScriptCreated);
		MyEventSystem->subscribe(this, &LogicSystem::CallbackScriptDestroyed);
	}

	void LogicSystem::update()
	{
		Scene* pScene = sceneManager.get_current_scene();
		if (pScene == nullptr)
			return;

		for (Script* script : scriptComponents)
		{
			if (!script->gameObj.isActive())
				continue;
			if (!script->Enabled())
				continue;
			MyEventSystem->publish(new ScriptInvokeMethodEvent(*script, "Update"));
			if (pScene != sceneManager.get_current_scene())
				return;

				//for (size_t j = 0; j < MyFrameRateController.getSteps(); ++j)
				//{
				//	MyEventSystem->publish(new ScriptInvokeMethodEvent(*pScript,"FixedUpdate"));
				//}
				//if (pScene != sceneManager.get_current_scene())
				//	return;
		}

		if (Button::hoveredBtn && (!Button::hoveredBtn->gameObj.isActive() || !Button::hoveredBtn->Enabled()))
		{
			Button::hoveredBtn->state = ButtonState::None;
			Button::hoveredBtn = nullptr;
		}
		for (GameObject* pGameObj : pScene->gameObjects)
		{
			if (!pGameObj->isActive())
				continue;
			const std::vector<Button*>& Buttons{ pGameObj->GetComponents<Button>() };
			for (Button* pButton : Buttons)
			{
				if (!pButton)
					continue;
				if (!pButton->Enabled())
					continue;
				pButton->update();
				if (pScene != sceneManager.get_current_scene())
					return;
			}
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
			gameObjects = &pScene->gameObjects;
			for (GameObject* pGameObj : *gameObjects)
			{
				const std::vector<Script*>& pScripts{ pGameObj->GetComponents<Script>() };
				for (Script* pScript : pScripts)
				{
					scriptComponents.push_back(pScript);
					MyEventSystem->publish(new ScriptInvokeMethodEvent(*pScript, "Awake"));
				}
			}


			for (Script* script : scriptComponents)
			{
				MyEventSystem->publish(new ScriptInvokeMethodEvent(*script, "Start"));
			}
			timeElasped = MyFrameRateController.getDt();
		}
		else
		{
			inPlayMode = false;
		}
	}


	void LogicSystem::CallbackScriptCreated(ScriptCreatedEvent* pEvent)
	{
		if (inPlayMode)
			scriptComponents.push_back(&pEvent->script);
	}

	void LogicSystem::CallbackScriptDestroyed(ScriptDestroyedEvent* pEvent)
	{
		if (inPlayMode)
			scriptComponents.remove(&pEvent->script);
	}
}