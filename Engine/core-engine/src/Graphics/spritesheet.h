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
		\param _texture
			The texture to reference and set
		\param _xDivisor
			The number of columns
		\param _yDivisor
			The number of rows
		*/
		/***************************************************************************/
		Spritesheet(const Texture& _texture, const unsigned int _xDivisor, const unsigned int _yDivisor);
		
		const unsigned int& get_size() const { return size;}
		const Texture& get_texture() const { return refTexture; }
		const glm::vec2& get_steps() const { return steps; }
		const std::vector<glm::vec2>& get_offsets() const { return textureOffsets; }

	private:
		const Texture& refTexture;

		unsigned int xColumns; // Minimum 1 column
		unsigned int yRows; // Minimum 1 row
		unsigned int size;

		glm::vec2 steps;
		std::vector<glm::vec2> textureOffsets; // Offsets starts from 0,0 to 1,1
		// E.g. if column = 4, and row = 1, textureOffsets[0] = {0,0}, textureOffsets[1] = {0.25,0}
		// textureOffsets[2] = {0.5,0}, textureOffsets[3] = {0.75,0}
	};
}

#endif // !SPRITESHEET_H

