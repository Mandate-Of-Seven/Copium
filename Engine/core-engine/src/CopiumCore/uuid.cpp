/*!***************************************************************************************
\file			uuid.cpp
\project
\author			Matthew Lau

\par			Course: GAM250
\par			Section:
\date			06/01/2023

\brief
	Listed below are the definitions of functions pertaining to UUID struct.
	The UUID struct's purpose is to generate unique universal ids for general use across Copium Engine.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "uuid.h"
#include <random>
#include <unordered_set>
#include <unordered_map>

namespace Copium
{
	static std::random_device randomDevice;
	static std::mt19937_64 randomiser(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	#define PID_KEY "PID"

	UUID::UUID() : uuid{uniformDistribution(randomiser)}
	{
		
	}

	bool operator==(UUID& lhs, UUID& rhs)
	{
		if (lhs.ConstGetUUID() == rhs.ConstGetUUID())
			return true;

		return false;
	}

}
