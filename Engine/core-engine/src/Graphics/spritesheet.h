/*!***************************************************************************************
\file			spritesheet.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			27/10/2022

\brief
	This file holds the declaration of the spritesheet.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Graphics/textures.h"

namespace Copium
{
	class Spritesheet
	{
	public:
		const Texture* refTexture;
		unsigned int xColumns{1}; // Minimum 1 column
		unsigned int yRows{1}; // Minimum 1 row
		unsigned int size;
		// E.g. if column = 4, and row = 1, textureOffsets[0] = {0,0}, textureOffsets[1] = {0.25,0}
		// textureOffsets[2] = {0.5,0}, textureOffsets[3] = {0.75,0}
	};
}

#endif // !SPRITESHEET_H

