#include "pch.h"
#include "Animation/animation-struct.h"

namespace Copium
{
	Animation::Animation() : currentFrameIndex{ 0 }, timer{ 0 }, frameCount{ 0 }, timeDelay{ 0.0f },
							columnIndex{0}, rowIndex{0}, columns{1}, rows{1}
	{
		spriteSheet.columns = columns;
		spriteSheet.rows = rows;

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
		if (currentFrameIndex == frameCount-1)
		{
			ResetFrame();

		}
		else
		{
			++currentFrameIndex;
			++columnIndex;

			if (columnIndex >= spriteSheet.columns)
			{
				++rowIndex;
				columnIndex = 0;
			}

			if (rowIndex >= spriteSheet.rows)
			{
				rowIndex = 0;
			}
			//PRINT("Frame Index:" << currentFrameIndex);
			//PRINT("Row Index:" << rowIndex);
			//PRINT("Column Index:" << columnIndex);

		}

		timer = timeDelay;
	}
	void Animation::ResetFrame()
	{
		currentFrameIndex = 0;
		rowIndex = columnIndex = 0;
	}
}
