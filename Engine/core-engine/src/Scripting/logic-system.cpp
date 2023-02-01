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
#include "../Game/game-nonscript.h"

namespace Copium
{
	namespace
	{
		SceneManager& sceneManager {*SceneManager::Instance()};
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
		std::vector<GameObject*>* gameObjects;
		double timeElasped;
		Game game;
	}

	void LogicSystem::init()
	{
		messageSystem.subscribe(MESSAGE_TYPE::MT_START_PREVIEW,this);
		systemFlags |= FLAG_RUN_ON_PLAY;
	}

	void LogicSystem::update()
	{
		Scene* pScene = sceneManager.get_current_scene();
		if (pScene == nullptr)
			return;

		for (size_t i = 0; i < pScene->gameObjects.size(); ++i)
		{
			GameObject* pGameObj = pScene->gameObjects[i];
			if (!pGameObj->isActive())
				continue;
			const std::vector<Script*>& pScripts{ pGameObj->getComponents<Script>() };
			for (Script* pScript : pScripts)
			{
				if (!pScript)
					continue;
				if (!pScript->Enabled())
					continue;
				pScript->invoke("Update");
				if (pScene != sceneManager.get_current_scene())
					return;

				for (size_t j = 0; j < MyFrameRateController.getSteps(); ++j)
				{
					pScript->invoke("FixedUpdate");
				}
				if (pScene != sceneManager.get_current_scene())
					return;
			}
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
			const std::vector<Button*>& Buttons{ pGameObj->getComponents<Button>() };
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

		// Bean: Temporary for hardcoded scripts
		game.update();
	}

	void LogicSystem::exit()
	{
		game.exit();
	}

	void LogicSystem::handleMessage(MESSAGE_TYPE mType)
	{
		PRINT("LOGIC STARTING");
		//MT_START_PREVIEW
		Scene* pScene = sceneManager.get_current_scene();
		if (pScene == nullptr)
			return;
		gameObjects = &pScene->gameObjects;
		for (GameObject* pGameObj : *gameObjects)
		{
			const std::vector<Script*>& pScripts{ pGameObj->getComponents<Script>() };
			for (Script* pScript : pScripts)
			{
				pScript->invoke("Awake");
				pScript->invoke("Start");
			}
		}
		timeElasped = MyFrameRateController.getDt();

		PRINT("LOGIC END");
		// Bean: Temporary for hardcoded scripts
		game.init();
	}
}