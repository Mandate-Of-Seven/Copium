#include <pch.h>
#include <Graphics/animation-system.h>
#include <Events/events-system.h>

namespace Copium
{
	void AnimationSystem::Init()
	{
		systemFlags |= FLAG_RUN_ON_PLAY | FLAG_RUN_ON_EDITOR;
	}

	void AnimationSystem::Update()
	{
		ComponentsArray<Animator>* pAnimatorsArray{};
		MyEventSystem.publish(new GetComponentsArrayEvent{ pAnimatorsArray });
		float dt{};
		MyEventSystem.publish(new GetDeltaTimeEvent{ dt });
		for (size_t i = 0; i < pAnimatorsArray->GetSize(); ++i) {

			Animator& anim = (*pAnimatorsArray)[i];
			if (!anim.animationCount)
				return;

			if (anim.status != Animator::AnimatorStatus::playing)
				return;
			//PRINT("Animator update\n");
			if (anim.animations[anim.currentAnimationIndex].UpdateFrame(dt))
				anim.animations[anim.currentAnimationIndex].IncrementFrame();
		}
	}

	void AnimationSystem::Exit()
	{
	}
}