
#include "pch.h"
#include "GameObject/game-object.h"
#include "Scripting/logic-system.h"
#include "Scripting/script-component.h"
#include "SceneManager/sm.h"

namespace Copium
{
	namespace
	{
		NewSceneManager& sceneManager {*NewSceneManager::Instance()};
		bool play;
		std::vector<GameObject*>* gameObjects;
	}

	void LogicSystem::init()
	{
		gameObjects = &sceneManager.get_current_scene()->get_gameobjectvector();
		this->onlyUpdateOnPlay = true;
	}

	void LogicSystem::update()
	{
		gameObjects = &sceneManager.get_current_scene()->get_gameobjectvector();
		for (GameObject* pGameObj : *gameObjects)
		{
			const std::vector<ScriptComponent*>& pScriptComponents{ pGameObj->getComponents<ScriptComponent>() };
			PRINT("LOGIC!" << pGameObj->get_name() << " " << pScriptComponents.size());
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
		//for (GameObject* pGameObj : *gameObjects)
		//{
		//	const std::vector<ScriptComponent*>& pScriptComponents{ pGameObj->getComponents<ScriptComponent>() };
		//	for (ScriptComponent* pScriptComponent : pScriptComponents)
		//	{
		//		pScriptComponent->Awake();
		//		pScriptComponent->Start();
		//	}
		//}
	}
}