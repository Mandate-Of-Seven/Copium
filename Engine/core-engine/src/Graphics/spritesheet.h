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
		unsigned int GetSize() const { return size; }


		Texture* texture;
		unsigned int size, spriteID;
		std::string name;
	};
}

#endif // !SPRITESHEET_H

