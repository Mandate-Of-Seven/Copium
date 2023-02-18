#pragma once

#ifndef JSONUTILITIES_H
#define JSONUTILITIES_H

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <string>

namespace Copium
{
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

	template <typename T>
	void Serialize(const T& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key)
	{
		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);
		_value.AddMember(key, _data, _doc.GetAllocator());
		//PRINT("You have reached the default serialize function, it will do nothing so here's something for your troubles...\nBleep Bloop!");
	}

	template <>
	void Serialize<bool>(const bool& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key);

	template <>
	void Serialize<std::string>(const std::string& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key);

	template <typename T>
	bool Deserialize(T& _data, rapidjson::Value& _value, const std::string& _key)
	{
		return false;
	}

	template <>
	bool Deserialize<bool>(bool& _data, rapidjson::Value& _value, const std::string& _key);

	template <>
	bool Deserialize<int>(int& _data, rapidjson::Value& _value, const std::string& _key);
	template <>
	bool Deserialize<int64_t>(int64_t& _data, rapidjson::Value& _value, const std::string& _key);

	template <>
	bool Deserialize<unsigned int>(unsigned int& _data, rapidjson::Value& _value, const std::string& _key);	
	template <>
	bool Deserialize<uint64_t>(uint64_t& _data, rapidjson::Value& _value, const std::string& _key);

	template <>
	bool Deserialize<float>(float& _data, rapidjson::Value& _value, const std::string& _key);
	template <>
	bool Deserialize<double>(double& _data, rapidjson::Value& _value, const std::string& _key);

	template <>
	bool Deserialize<std::string>(std::string& _data, rapidjson::Value& _value, const std::string& _key);

	
}


#endif // JSONUTILITIES_H
