/*!***************************************************************************************
\file			animation-struct.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			05/01/2023

\brief
	This file holds the declaration of the Animation structures.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef ANIMATION_STRUCT_H
#define ANIMATION_STRUCT_H

#include <vector>
#include "Math/math-library.h"

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
		std::vector<Frame> frames;	// Container which stores all the frames of the animation
		int currentFrameIndex;		// Current frame that is playing
	};
}

#endif // !ANIMATION_STRUCT_H
