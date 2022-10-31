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

namespace Copium::Graphics
{
	class Spritesheet : public Texture
	{
	public:
		// Constructors
		/***************************************************************************/
		/*!
		\brief
			Generates the texture based on the file path using stbi_load
		*/
		/***************************************************************************/
		Spritesheet(const std::string& _filePath, const unsigned int _xDivisor, const unsigned int _yDivisor);

		/***************************************************************************/
		/*!
		\brief
			Delete texture object id
		*/
		/***************************************************************************/
		~Spritesheet();
	private:
		unsigned int xColumns; // Minimum 1 column
		unsigned int yRows; // Minimum 1 row

		std::vector<glm::vec2> textureOffsets; // Offsets starts from 0,0 to 1,1
		// E.g. if column = 4, and row = 1, textureOffsets[0] = {0,0}, textureOffsets[1] = {0.25,0}
		// textureOffsets[2] = {0.5,0}, textureOffsets[3] = {0.75,0}
	};
}

#endif // !SPRITESHEET_H

