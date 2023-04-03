/*!***************************************************************************************
\file			spritesheet.cpp
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			27/10/2022

\brief
	This file holds the definition of the spritesheet.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"

#include "Graphics/spritesheet.h"

namespace Copium
{
	Spritesheet::Spritesheet() : texture{nullptr}, spriteID{0}, name{std::string()}
	{
	
	}
}