/*!***************************************************************************************
\file			json-utilities.h
\project
\author			Matthew Lau
\co-authors		Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			16/02/2023

\brief
	This file holds the declarations of functions that abstract some Json operations.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#pragma once

#ifndef JSONUTILITIES_H
#define JSONUTILITIES_H

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <string>

namespace Copium
{
	using C_String = const char*;

	/***************************************************************************/
	/*!
	\brief
		Set a rapidjson::Value as a String. To be used for deserialization purposes!

	\param _key
		read-only reference to a stl string that will be the key for the value

	\param _value
		reference to the rapidjson::Value that will be set as a String

	\param _doc
		reference to the rapidjson::Document that data is being deserialized to

	\return
		void
	*/
	/***************************************************************************/
	void CreateJsonString(const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Set a rapidjson::Value as a String. To be used for deserialization purposes!

	\param _key
		read-only pointer to a string that will be the key for the value

	\param _value
		reference to the rapidjson::Value that will be set as a String

	\param _doc
		reference to the rapidjson::Document that data is being deserialized to

	\return
		void
	*/
	/***************************************************************************/
	void CreateJsonString(const char* _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	/***************************************************************************/
	/*!
	\brief
		Template function for serializing a basic data type to the specified rapidjson::Document with specified key.

	\param _data
		read-only reference to data that is to be serialized.

	\param _value
		reference to the rapidjson::Value that the data is serialized to

	\param _doc
		reference to the rapidjson::Document that data is being serialized to

	\param _key
		string that will be used as the key for the data value being serialized.

	\return
		void
	*/
	/***************************************************************************/
	template <typename T>
	void SerializeBasic(const T& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key)
	{
		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);
		_value.AddMember(key, _data, _doc.GetAllocator());
	}
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a bool to the specified rapidjson::Document with specified key.

	\param _data
		read-only reference to bool that is to be serialized.

	\param _value
		reference to the rapidjson::Value that the bool is going to be serialized to

	\param _doc
		reference to the rapidjson::Document that bool is being serialized to

	\param _key
		string that will be used as the key for the bool value being serialized.

	\return
		void
	*/
	/***************************************************************************/
	template <>
	void SerializeBasic<bool>(const bool& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a std::string to the specified rapidjson::Document with specified key.

	\param _data
		read-only reference to std::string that is to be serialized.

	\param _value
		reference to the rapidjson::Value that the std::string is going to be serialized to

	\param _doc
		reference to the rapidjson::Document that std::string is being serialized to

	\param _key
		string that will be used as the key for the std::string value being serialized.

	\return
		void
	*/
	/***************************************************************************/
	template <>
	void SerializeBasic<std::string>(const std::string& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a C string to the specified rapidjson::Document with specified key.

	\param _data
		read-only reference to C string that is to be serialized.

	\param _value
		reference to the rapidjson::Value that the C string is going to be serialized to

	\param _doc
		reference to the rapidjson::Document that C string is being serialized to

	\param _key
		string that will be used as the key for the C string value being serialized.

	\return
		void
	*/
	/***************************************************************************/
	template <>
	void SerializeBasic<C_String>(const C_String& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key);

	/***************************************************************************/
	/*!
	\brief
		Template function for deserializing a basic data type from the specified rapidjson::Value with specified key.
		Note: this base template function should not be called, if it is, nothing should happen.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		false
	*/
	/***************************************************************************/
	template <typename T>
	bool Deserialize(T& _data, rapidjson::Value& _value, const std::string& _key)
	{
		static_assert(true);
		return false;
	}

	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a bool from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<bool>(bool& _data, rapidjson::Value& _value, const std::string& _key);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing an int from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<int>(int& _data, rapidjson::Value& _value, const std::string& _key);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a int64 from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<int64_t>(int64_t& _data, rapidjson::Value& _value, const std::string& _key);

	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a uint from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<unsigned int>(unsigned int& _data, rapidjson::Value& _value, const std::string& _key);	
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a uint64 from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<uint64_t>(uint64_t& _data, rapidjson::Value& _value, const std::string& _key);

	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a float from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<float>(float& _data, rapidjson::Value& _value, const std::string& _key);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a double from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<double>(double& _data, rapidjson::Value& _value, const std::string& _key);

	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a std::string from the specified rapidjson::Value with specified key.

	\param _data
		read-only reference to the variable that the data is to be deserialized to.

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\param _key
		the key of the rapidjson::Value that holds the desired data

	\return
		if successful, return true
		if failed, return false
	*/
	/***************************************************************************/
	template <>
	bool Deserialize<std::string>(std::string& _data, rapidjson::Value& _value, const std::string& _key);

	
}


#endif // JSONUTILITIES_H
