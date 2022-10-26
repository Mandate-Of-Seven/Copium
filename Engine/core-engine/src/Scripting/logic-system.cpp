
#include "pch.h"
#include "GameObject/game-object.h"
#include "Scripting/logic-system.h"
#include "Scripting/scripting.h"
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
		play = false;
		gameObjects = nullptr;
	}

	void LogicSystem::update()
	{
		if (play)
		{
			for (GameObject* pGameObj : *gameObjects)
			{
				const std::vector<ScriptComponent*>& pScriptComponents{ pGameObj->getComponents<ScriptComponent>() };
				for (ScriptComponent* pScriptComponent : pScriptComponents)
				{
					pScriptComponent->Update();
					pScriptComponent->LateUpdate();
				}
			}
		}
	}

	void LogicSystem::exit()
	{

	}

	bool LogicSystem::Play() const
	{
		return play;
	}

	void LogicSystem::Play(bool _play)
	{
		//Awake, Init
		if (_play == true)
		{
			gameObjects = &sceneManager.get_current_scene()->get_gameobjectvector();
			for (GameObject* pGameObj : *gameObjects) 
			{
				const std::vector<ScriptComponent*>& pScriptComponents{pGameObj->getComponents<ScriptComponent>()};
				for (ScriptComponent* pScriptComponent : pScriptComponents)
				{
					pScriptComponent->Awake();
					pScriptComponent->Start();
				}
			}
		}
		play = _play;
	}
}