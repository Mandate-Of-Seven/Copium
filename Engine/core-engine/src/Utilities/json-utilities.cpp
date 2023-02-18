#include "pch.h"
#include "Utilities/json-utilities.h"

namespace Copium
{
	void CreateJsonString(const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::SizeType length = (rapidjson::SizeType)_key.length();
		_value.SetString(_key.c_str(), length, _doc.GetAllocator());

	}
	void CreateJsonString(const char* _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::SizeType length = (rapidjson::SizeType)strlen(_key);
		_value.SetString(_key, length, _doc.GetAllocator());
	}

	template <>
	void Serialize<bool>(const bool& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key)
	{
		PRINT("Serializing a Bool");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);
		_value.AddMember(key, _data, _doc.GetAllocator());
	}

	template <>
	void Serialize<std::string>(const std::string& _data, rapidjson::Value& _value, rapidjson::Document& _doc, const std::string& _key)
	{
		PRINT("Serializing a String");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);
		rapidjson::Value data;
		CreateJsonString(_data, data, _doc);

		_value.AddMember(key, data, _doc.GetAllocator());
	}




	template <>
	bool Deserialize<bool>(bool& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetBool();
		return true;
	}

	template <>
	bool Deserialize<int>(int& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetInt();
		return true;
	}
	template <>
	bool Deserialize<int64_t>(int64_t& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetInt64();
		return true;
	}
	template <>
	bool Deserialize<unsigned int>(unsigned int& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetUint();
		return true;
	}
	template <>
	bool Deserialize<uint64_t>(uint64_t& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetUint64();
		return true;
	}

	template <>
	bool Deserialize<float>(float& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetFloat();

		return true;
	}
	template <>
	bool Deserialize<double>(double& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetDouble();
		return true;
	}

	template <>
	bool Deserialize<std::string>(std::string& _data, rapidjson::Value& _value, const std::string& _key)
	{
		if (!_value.HasMember(_key.c_str()))
			return false;

		_data = _value[_key.c_str()].GetString();
		return true;
	}
}

