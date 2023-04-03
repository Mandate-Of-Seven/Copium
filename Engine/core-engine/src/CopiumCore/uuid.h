/*!***************************************************************************************
\file			uuid.h
\project
\author			Matthew Lau

\par			Course: GAM250
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
		/*******************************************************************************
		/*!
		*
		\brief
			Default constructor for UUID
			Note: the uuid is generated on construction

		\return
			void
		*/
		/*******************************************************************************/
		UUID();
		/*******************************************************************************
		/*!
		*
		\brief
			Default copy constructor 

		\return
			void
		*/
		/*******************************************************************************/
		UUID(const UUID& _rhs) = default;
		/*******************************************************************************
		/*!
		*
		\brief
			Single argument constructor

		\param _rhs
			a uint64_t value which this UUID will take as its value

		\return
			void
		*/
		/*******************************************************************************/
		UUID(uint64_t _rhs) : uuid{ _rhs } {}
		/*******************************************************************************
		/*!
		*
		\brief
			uint64_t conversion

		\return
			this UUID as a uint64_t value
		*/
		/*******************************************************************************/
		operator uint64_t() const { return uuid; }

		/*******************************************************************************
		/*!
		*
		\brief
			Deserialize UUID from a rapidjson Value

		\param	_val
			reference to a rapidjson::Value which should contain the UUID

		\return
			void
		*/
		/*******************************************************************************/
		void Deserialize(rapidjson::Value& _val);
		/*******************************************************************************
		/*!
		*
		\brief
			Serialize this UUID to a rapidjson Value

		\param	_val
			reference to the rapidjson::Value which this UUID's data will be written to

		\param _doc
			reference to the rapidjson::Document which this UUID's data is being written to

		\return
			void
		*/
		/*******************************************************************************/
		void Serialize(rapidjson::Value& _val, rapidjson::Document& _doc, const std::string& _name = "UID");

		/*******************************************************************************
		/*!
		*
		\brief
			Get reference to this UUID

		\return
			reference to this UUID
		*/
		/*******************************************************************************/
		uint64_t& GetUUID() { return uuid; }
		
		/*******************************************************************************
		/*!
		*
		\brief
			Get this UUID value

		\return
			this UUID's value
		*/
		/*******************************************************************************/
		uint64_t ConstGetUUID() { return uuid; }

	private:
		uint64_t uuid;
	};


	bool operator==(UUID& lhs, UUID& rhs);

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

