/*!***************************************************************************************
****
\file			scriptable-object.h
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file contains the declarations of the ScriptableObject class.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#pragma once

#include <map>
#include <string>
#include <vector>

namespace Copium
{
	struct ScriptableObject
	{
		std::string className;
		//Serializing will create an instance
	};
}