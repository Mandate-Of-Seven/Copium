﻿/*!***************************************************************************************
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
#include "GameObject/Components/ui-components.h"

namespace Copium
{
	namespace
	{
		NewSceneManager& sceneManager {*NewSceneManager::Instance()};
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
		std::vector<GameObject*>* gameObjects;
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
		gameObjects = &pScene->get_gameobjectvector();
		for (GameObject* pGameObj : *gameObjects)
		{
			const std::vector<ButtonComponent*>& buttonComponents{ pGameObj->getComponents<ButtonComponent>() };
			for (ButtonComponent* pButtonComponent : buttonComponents)
			{
				if (!pButtonComponent)
					continue;
				pButtonComponent->update();
			}
			const std::vector<ScriptComponent*>& pScriptComponents{ pGameObj->getComponents<ScriptComponent>() };
			for (ScriptComponent* pScriptComponent : pScriptComponents)
			{
				if (!pScriptComponent)
					continue;
				pScriptComponent->Update();
				pScriptComponent->LateUpdate();
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
		gameObjects = &pScene->get_gameobjectvector();
		for (GameObject* pGameObj : *gameObjects)
		{
			const std::vector<ScriptComponent*>& pScriptComponents{ pGameObj->getComponents<ScriptComponent>() };
			for (ScriptComponent* pScriptComponent : pScriptComponents)
			{
				pScriptComponent->Awake();
				pScriptComponent->Start();
			}
		}
	}
}