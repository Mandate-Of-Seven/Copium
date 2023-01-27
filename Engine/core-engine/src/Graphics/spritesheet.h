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
		/***************************************************************************/
		/*!
		\brief
			Constructs the spritesheet to be used for animations
		*/
		/***************************************************************************/
		Spritesheet();

		Spritesheet(const Texture& _texture);
		
		Texture* GetTexture() { return texture; }


		Texture* texture;
		unsigned int spriteID;
		std::string name;
		int columns, rows;
	};
}

#endif // !SPRITESHEET_H

