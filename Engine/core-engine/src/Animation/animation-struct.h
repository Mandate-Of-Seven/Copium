/*!***************************************************************************************
\file			animation-struct.h
\project
\author			Sean Ngo
\co-author		Matthew Lau

\par			Course: GAM250
\par			Section:
\date			05/01/2023

\brief
	This file holds the declaration of the Animation structures.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef ANIMATION_STRUCT_H
#define ANIMATION_STRUCT_H

#include <vector>
#include "Math/math-library.h"
#include "Graphics/spritesheet.h"

#define MAX_ANIMATION_COUNT 5

namespace Copium
{
	struct Frame
	{
		Math::Vec2 top;		// Top-Left UV of the sprite
		Math::Vec2 bottom;	// Bottom-Right UV of the sprite
		float timeDelay;	// How long the frame is played
	};

	struct Animation
	{
		enum class AnimationStatus : char
		{
			idle,
			playing,
			paused,
			completed
		};

		Spritesheet spriteSheet;
		double timer, timeDelay;
		int frameCount, currentFrameIndex;
		int rowIndex, columnIndex;
		int columns, rows;
		Math::Vec4 color;
		AnimationStatus status;
		bool loop;

		/***************************************************************************/
		/*!
		\brief
			Default constructor for an Animation

		\return
			void
		*/
		/**************************************************************************/
		Animation();

		/***************************************************************************/
		/*!
		\brief
			Updates the timer using dt

		\param _dt
			delta time

		\return
			true if timer has hit 0
			false if timer has not hit 0
		*/
		/**************************************************************************/
		bool UpdateFrame(double _dt);
		/***************************************************************************/
		/*!
		\brief
			Move to the next frame. Wraparounds if appropriate

		\return
			void
		*/
		/**************************************************************************/
		void IncrementFrame();
		/***************************************************************************/
		/*!
		\brief
			Move to next frame but in reverse order. Wraparounds if appropriate

		\return
			void
		*/
		/**************************************************************************/
		void DecrementFrame();
		/***************************************************************************/
		/*!
		\brief
			Reset the frame offsets to the 1st frame

		\return
			void
		*/
		/**************************************************************************/
		void ResetFrame();

		/***************************************************************************/
		/*!
		\brief
			Set the current animation's frame to the specified frame
			Note: if the index specified is negative or higher than the total no. of frames, nothing happens

		\param _frames
			the frame index to set the current frame to


		\return
			void
		*/
		/**************************************************************************/
		void SetFrame(int _frame);

	};
}

#endif // !ANIMATION_STRUCT_H
