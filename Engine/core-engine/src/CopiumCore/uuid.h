#ifndef UUID_H
#define UUID_H


#pragma once
#include <xhash>
#include <rapidjson/document.h>


namespace Copium
{
	class UUID
	{
	public:
		// Constructor generates the UUID
		UUID();
		UUID(uint64_t _uuid);
		UUID(const UUID& _rhs) = default;

		operator uint64_t() const { return uuid; }

		void Deserialize(rapidjson::Value& _val);
		void Serialize(rapidjson::Value& _val, rapidjson::Document& _doc);

	private:
		uint64_t uuid;
	};
}

namespace std
{
	// Hash template specialization so that UUID can be used with std maps and sets
	template<>
	struct hash<Copium::UUID>
	{
		size_t operator()(const Copium::UUID& _uuid) const
		{
			return hash<uint64_t>()((uint64_t)_uuid);
		}
	};
}

#endif // !UUID_H

