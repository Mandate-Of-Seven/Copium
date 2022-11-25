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
#include "SceneManager/sm.h"
#include "Messaging/message-system.h"
#include <Windows/windows-system.h>
#include <Debugging/frame-rate-controller.h>
#include "GameObject/Components/ui-components.h"

namespace Copium
{
	namespace
	{
		NewSceneManager& sceneManager {*NewSceneManager::Instance()};
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
		std::vector<GameObject*>* gameObjects;
		double timeElasped;
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
		gameObjects = &pScene->gameObjects;
		for (GameObject* pGameObj : *gameObjects)
		{
			//for (Component* pComponent : pGameObj->components)
			//{
			//	PRINT(pComponent->Name() << ":  " << pComponent->id);
			//}
			const std::vector<Button*>& Buttons{ pGameObj->getComponents<Button>() };
			for (Button* pButton : Buttons)
			{
				if (!pButton)
					continue;
				pButton->update();
			}
			const std::vector<Script*>& pScripts{ pGameObj->getComponents<Script>() };
			for (Script* pScript : pScripts)
			{
				//PRINT("HELLO!");
				if (!pScript)
					continue;
				pScript->invoke("Update");
				if (pScene != sceneManager.get_current_scene())
					return;

				for (size_t i = 0; i < MyFrameRateController.getSteps(); ++i)
				{
					pScript->invoke("FixedUpdate");
				}
			}
		}

	}

	void LogicSystem::exit()
	{

	}

	void LogicSystem::handleMessage(MESSAGE_TYPE mType)
	{
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
	}
}