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

namespace Copium::Graphics
{
	Spritesheet::Spritesheet(const std::string& _filePath, const unsigned int _xDivisor, const unsigned int _yDivisor) 
		: Texture(_filePath), xColumns(_xDivisor), yRows(_yDivisor)
	{
		
	}

	Spritesheet::~Spritesheet()
	{

	}
}