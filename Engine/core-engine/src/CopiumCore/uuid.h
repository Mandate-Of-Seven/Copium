/*!***************************************************************************************
\file			uuid.h
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			06/01/2023

\brief
	The declaration of UUID struct is contained in this file.
	The UUID struct's purpose is to generate unique universal ids for general use across Copium Engine.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
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
		// Constructors generate the UUID
		UUID();
		UUID(const UUID& _rhs) = default;
		UUID(uint64_t _rhs) : uuid{ _rhs } {}
		operator uint64_t() const { return uuid; }
		const uint64_t uuid;
	};
}

namespace std
{
	// Hash template specialization so that UUID can be used with std maps and sets
	template<>
	struct hash<Copium::UUID>
	{
		/*******************************************************************************
		/*!
		*
		\brief
			Function overload which will allow Copium::UUID to be used with std maps and sets

		\param	_uuid
			read-only reference to a UUID struct 

		\return
			the resulting hash value
		*/
		/*******************************************************************************/
		size_t operator()(const Copium::UUID& _uuid) const
		{
			return hash<uint64_t>()((uint64_t)_uuid);
		}
	};
}

#endif // !UUID_H

