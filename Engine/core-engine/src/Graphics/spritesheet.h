/*!***************************************************************************************
\file			spritesheet.h
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			27/10/2022

\brief
	This file holds the declaration of the spritesheet.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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
		uint64_t spriteID;
		std::string name;
		glm::bvec2 flip{ 0 };
		int columns, rows;
	};
}

#endif // !SPRITESHEET_H

