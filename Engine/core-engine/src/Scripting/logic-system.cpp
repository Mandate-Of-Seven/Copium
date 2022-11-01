
#include "pch.h"
#include "GameObject/game-object.h"
#include "Scripting/logic-system.h"
#include "Scripting/script-component.h"
#include "SceneManager/sm.h"
#include "Messaging/message-system.h"

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
			const std::vector<ScriptComponent*>& pScriptComponents{ pGameObj->getComponents<ScriptComponent>() };
			for (ScriptComponent* pScriptComponent : pScriptComponents)
			{
				PRINT("NAME: " << pScriptComponent->Name());
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