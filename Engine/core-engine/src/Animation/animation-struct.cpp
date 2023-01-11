#include "pch.h"
#include "Animation/animation-struct.h"

namespace Copium
{
	Animation::Animation() : currentFrameIndex{ 0 }, timer{ 0 }, frameCount{ 0 }, timeDelay{ 0.0f }
	{

	}
	bool Animation::UpdateFrame(float _dt)
	{
		timer -= _dt;

		if (timer <= 0.f)
		{
			timer = 0.f;
			return true;
		}
		else
		{
			return false;
		}
	}
	void Animation::IncrementFrame()
	{
		if (frameCount <= 0)
			return;

		// Perform wrap-around if incrementing from the last frame
		if (currentFrameIndex == frameCount - 1)
			currentFrameIndex = 0;
		else
			++currentFrameIndex;

		timer = timeDelay;
	}
	void Animation::ResetFrame()
	{
		currentFrameIndex = 0;
	}
}
