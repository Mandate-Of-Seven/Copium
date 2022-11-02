/*!***************************************************************************************
\file			spritesheet.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			27/10/2022

\brief
	This file holds the definition of the spritesheet.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"

#include "Graphics/spritesheet.h"

namespace Copium
{
	Spritesheet::Spritesheet(const Texture& _texture, const unsigned int _xDivisor, const unsigned int _yDivisor)
		: refTexture(_texture), xColumns(_xDivisor), yRows(_yDivisor), size(xColumns * yRows)
	{
		// Divide the texture coordinates by x and y
		// Give us how many rows and columns
		// They will be indexed from the bottom left being 0,0 to 1,1
		// Users will be able to retrieve the texture from the spritesheet using an index to the textureOffsets

		float xStep = (xColumns == 1) ? 0.f : 1.f / (float) xColumns;
		float yStep = (yRows == 1) ? 0.f : 1.f / (float) yRows;
		steps = glm::vec2(xStep, yStep);
		glm::vec2 textureCoord = { 0.f, 0.f };
		for (unsigned int i = 0; i < yRows; i++)
		{
			for (unsigned int j = 0; j < xColumns; j++)
			{
				textureOffsets.push_back(textureCoord);
				//PRINT("texture offset: " << textureCoord.x << " " << textureCoord.y);
				textureCoord += glm::vec2(xStep, yStep);
			}
		}

		glm::vec2 textCord = { textureOffsets.back().x, textureOffsets.back().y };
		//PRINT("Last texture offset: " << textCord.x << " " << textCord.y);
		//PRINT("Steps: " << xStep << " " << yStep);
	}
}