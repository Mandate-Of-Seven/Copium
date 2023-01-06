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

	UUID::UUID() : uuid{uniformDistribution(randomiser)}
	{
		
	}

	UUID::UUID(uint64_t _uuid) : uuid{_uuid}
	{

	}

	void UUID::Deserialize(rapidjson::Value& _val)
	{
		uuid = _val.GetUint64();
	}
	void UUID::Serialize(rapidjson::Value& _val, rapidjson::Document& _doc)
	{
		_val.AddMember("ID", uuid, _doc.GetAllocator());
	}


}
