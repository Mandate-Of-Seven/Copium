#pragma once

#include <map>
#include <string>
#include <vector>

namespace Copium
{
	struct ScriptableObject
	{
		enum class FieldType
		{
			Float, Double,
			Bool, Char, Short, Int, Long,
			UShort, UInt, ULong,
			Vector2, Vector3, GameObject, Component, None
		};
		std::string className;
		std::map<std::string, FieldType> fieldTypeMap;

		//Serializing will create an instance
	};

	struct ScriptableObjectInstance
	{
		std::string className;
		std::map<std::string, std::vector<char>> fieldDataMap;
		
		//Serializing will create an instance
	};

}