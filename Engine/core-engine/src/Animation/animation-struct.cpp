/*!***************************************************************************************
\file			animation-struct.cpp
\project
\author			Matthew Lau

\par			Course: GAM250
\par			Section:
\date			11/01/2023

\brief
	This file holds the definitons of the Animation structures.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Animation/animation-struct.h"

namespace Copium
{
	Animation::Animation() : currentFrameIndex{ 0 }, timer{ 0 }, frameCount{ 0 }, timeDelay{ 0.0f },
							columnIndex{0}, rowIndex{0}, columns{1}, rows{1}, status{AnimationStatus::idle}, loop{true}
	{
		spriteSheet.columns = columns;
		spriteSheet.rows = rows;
		color = Math::Vec4(1.f, 1.f, 1.f, 1.f);

	}
	bool Animation::UpdateFrame(double _dt)
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

		//PRINT("Index:" << rowIndex << '|' << columnIndex);

		if (frameCount <= 0)
			return;


		if (status == AnimationStatus::completed)
			return;

		// Perform wrap-around if incrementing from the last frame
		if (currentFrameIndex == frameCount-1)
		{
			ResetFrame();
		}
		else
		{
			//PRINT("IncrementFrame");

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

		}

		timer = timeDelay;
	}
	void Animation::DecrementFrame()
	{
		if (frameCount <= 0)
			return;


		if (status == AnimationStatus::completed)
			return;

		// Perform wrap-around if incrementing from the last frame
		if (currentFrameIndex <= 0)
		{
			currentFrameIndex = frameCount - 1;
			columnIndex = columns - 1;
			rowIndex = rows - 1;
			if (!loop)
			{
				status = AnimationStatus::completed;
			}
		}
		else
		{
			--currentFrameIndex;
			--columnIndex;

			if (columnIndex <= 0)
			{
				--rowIndex;
				columnIndex = columns - 1;
			}

			if (rowIndex <= 0)
			{
				rowIndex = rows - 1;
			}

		}

		timer = timeDelay;
	}
	void Animation::ResetFrame()
	{
		//PRINT("Reset Frame");
		if (!loop)
		{
			//PRINT("Animation Completed");
			status = AnimationStatus::completed;
		}

		currentFrameIndex = 0;
		rowIndex = columnIndex = 0;
	}

	void Animation::SetFrame(int _frame)
	{
		if (_frame < 0 || _frame >= frameCount)
			return;

		//PRINT("set frame");
		currentFrameIndex = _frame;
		rowIndex = _frame / spriteSheet.columns;
		columnIndex = _frame % spriteSheet.columns;

		//PRINT("Row:" << rowIndex << ' ' << "Cols:" << columnIndex);

	}
}
