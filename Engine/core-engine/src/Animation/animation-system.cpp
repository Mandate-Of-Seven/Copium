/*!***************************************************************************************
\file			animation-system.cpp
\project
\author			Sean Ngo
\co-authors		Matthew Lau
				Shawn Tanary

\par			Course: GAM250
\par			Section:
\date			05/01/2023

\brief
	This file holds the definition of the Animation system. The Animation system animates
	the spritesheets in the engine, where the user has to create the animation in the
	animator in the editor.

	The definitions of functions from the Animator Component are here as well.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Animation/animation-system.h"
#include "SceneManager/scene-manager.h"
#include "Files/assets-system.h"
#include "Debugging/frame-rate-controller.h"
#include "Utilities/json-utilities.h"

#define MAX_ANIMATION_COUNT 5

namespace Copium
{
	void AnimationSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_PLAY | FLAG_RUN_ON_EDITOR;
		
	}
	void AnimationSystem::update()
	{
		Scene* pScene{ MySceneManager.get_current_scene() };
		if (!pScene)
			return;

		for (Animator& animator : pScene->componentArrays.GetArray<Animator>())
		{
			if (!animator.enabled || !animator.gameObj.IsActive())
				continue;
			animator.Update(MyFrameRateController.getDt());
			if (MySceneManager.GetSceneState() == Scene::SceneState::play && animator.GetStatus() != Animator::AnimatorStatus::paused)
			{
				animator.SetStatus(Animator::AnimatorStatus::playing);
			}
			else if (MySceneManager.GetSceneState() == Scene::SceneState::paused)
			{
				animator.SetStatus(Animator::AnimatorStatus::idle);

			}
		}
	}
	void AnimationSystem::exit()
	{
	
	}

	void AnimationSystem::PauseAllAnimation()
	{
		Scene* pScene{ MySceneManager.get_current_scene() };
		if (!pScene)
			return;

		for (Animator& animator : pScene->componentArrays.GetArray<Animator>())
		{
			animator.PauseAnimation();
		}
	}

	void AnimationSystem::PlayAllAnimation()
	{
		Scene* pScene{ MySceneManager.get_current_scene() };
		if (!pScene)
			return;

		for (Animator& animator : pScene->componentArrays.GetArray<Animator>())
		{
			animator.PlayAnimation();
		}
	}

}