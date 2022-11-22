/*!***************************************************************************************
\file			scripting.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definition of functions for scripting

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "GameObject/Components/script-component.h"
#include "GameObject/game-object.h"

#define DEFAULT_SCRIPT_NAME "NewScript"
#include <mono/jit/jit.h>
namespace Copium
{
	ScriptingSystem& Script::sS{ *ScriptingSystem::Instance() };

	Script::Script(GameObject& _gameObj) :
		mObject{ nullptr }, pScriptClass{ nullptr }, Component(_gameObj, ComponentType::Script), name{ DEFAULT_SCRIPT_NAME }, reference{nullptr}
	{
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		pScriptClass = sS.getScriptClass(name);
		instantiate();
	}

	Script::~Script()
	{
		MessageSystem::Instance()->unsubscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
	}

	void Script::instantiate()
	{
		if (pScriptClass != nullptr)
		{
			if (reference == nullptr)
			{
				mObject = sS.instantiateClass(pScriptClass->mClass);
			}
			else
			{
				mObject = sS.cloneInstance(reference->mObject);
				reference = nullptr;
			}
			GameObjectID _id = gameObj.id;
			void* param = &_id;
			sS.invoke(mObject, pScriptClass->mMethods["OnCreate"], &param);
		}
	}

	void Script::handleMessage(MESSAGE_TYPE mType)
	{
		//MT_SCRIPTING_UPDATED
		pScriptClass = sS.getScriptClass(name.c_str());
		instantiate();
	}

	const std::string& Script::Name() const
	{
		return name;
	}

	void Script::Name(const std::string& _name)
	{
		name = _name;
		pScriptClass = sS.getScriptClass(name);
		instantiate();
	}

	void Script::invoke(const std::string& methodName)
	{
		if (!pScriptClass)
			return;
		std::unordered_map<std::string,MonoMethod*>::iterator method = pScriptClass->mMethods.find(methodName);
		if (method != pScriptClass->mMethods.end())
		{
			sS.invoke(mObject, (*method).second);
		}
	}

	//Use for serialization
	bool Script::getFieldValue(const std::string& _name, void* outBuffer)
	{
		const auto& it = pScriptClass->mFields.find(_name);
		if (it == pScriptClass->mFields.end())
			return false;
		const Field& field = it->second;
		mono_field_get_value(mObject, field.classField, buffer);
		return true;
	}

	bool Script::setFieldValue(const std::string& _name, const void* value)
	{
		const auto& it = pScriptClass->mFields.find(_name);
		if (it == pScriptClass->mFields.end())
			return false;
		const Field& field = it->second;
		mono_field_set_value(mObject, field.classField, (void*)value);
		return true;
	}

	void Script::inspector_view()
	{
		if (!pScriptClass)
			return;

		static ImVec4 backupColor;

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable(name.c_str(), 2, windowFlags))
		{
			ImGui::Indent();
			// Extern source file
			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);
			const auto& fieldMap = pScriptClass->mFields;
			auto it = fieldMap.begin();
			while (it != fieldMap.end())
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				const std::string& _name{ it->first };
				ImGui::Text(_name.c_str());
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(-FLT_MIN);
				getFieldValue(_name, buffer);
				switch (it->second.type)
				{
				case FieldType::Float:
					ImGui::InputFloat(_name.c_str(), reinterpret_cast<float*>(buffer));
					break;
				case FieldType::Double:
					ImGui::InputDouble(_name.c_str(), reinterpret_cast<double*>(buffer));
					break;
				case FieldType::Bool:
					ImGui::Checkbox(_name.c_str(), reinterpret_cast<bool*>(buffer));
					break;
				case FieldType::Char:
					ImGui::InputInt(_name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Short:
					ImGui::InputInt(_name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Int:
					ImGui::InputInt(_name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Long:
					ImGui::InputInt2(_name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::UShort:
					break;
				case FieldType::UInt:
					break;
				case FieldType::ULong:
					break;
				case FieldType::Vector2:
				{
					if (ImGui::BeginTable(_name.c_str(), 3, windowFlags))
					{
						float* fBuffer = reinterpret_cast<float*>(buffer);
						ImGui::TableNextColumn();
						ImGui::PushID(0);
						ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
						ImGui::InputFloat("", fBuffer++);
						ImGui::PopID();

						ImGui::TableNextColumn();
						ImGui::PushID(1);
						ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
						ImGui::InputFloat("", fBuffer++);
						ImGui::PopID();
						ImGui::EndTable();
					}
					break;
				}
				case FieldType::Vector3:
				{
					if (ImGui::BeginTable(_name.c_str(), 3, windowFlags))
					{
						float* fBuffer = reinterpret_cast<float*>(buffer);
						ImGui::TableNextColumn();
						ImGui::PushID(0);
						ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
						ImGui::InputFloat("", fBuffer++);
						ImGui::PopID();

						ImGui::TableNextColumn();
						ImGui::PushID(1);
						ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
						ImGui::InputFloat("", fBuffer++);
						ImGui::PopID();

						ImGui::TableNextColumn();
						ImGui::PushID(2);
						ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
						ImGui::InputFloat("", fBuffer);
						ImGui::PopID();
						ImGui::EndTable();
					}
					break;
				}
			}
				setFieldValue(_name, buffer);
				++it;
			}

			ImGui::Unindent();
			ImGui::EndTable();
		}
	}

	Component* Script::clone(GameObject& _gameObj) const
	{
		Script* component = new Script(_gameObj);
		component->pScriptClass = pScriptClass;
		component->name = name;
		component->reference = this;
		component->instantiate();
		return component;
	}

	void Script::deserialize(rapidjson::Value& _value)
	{
		if (_value.HasMember("Name"))
		{
			Name(_value["Name"].GetString());
		}

		if (pScriptClass == nullptr)
			return;

		const auto& fieldMap = pScriptClass->mFields;
		auto it = fieldMap.begin();
		while (it != fieldMap.end())
		{
			const std::string& _name{ it->first };
			if (!_value.HasMember(_name.c_str()))
			{
				++it;
				continue;
			}
			switch (it->second.type)
			{
			case FieldType::Float:
			{
				float buf = _value[_name.c_str()].GetFloat();
				memcpy(buffer, &buf, sizeof(float));
				break;
			}
			case FieldType::Double:
			{
				double buf = _value[_name.c_str()].GetDouble();
				memcpy(buffer, &buf, sizeof(double));
				break;
			}
			case FieldType::Bool:
			{
				bool buf = _value[_name.c_str()].GetBool();
				memcpy(buffer, &buf, sizeof(bool));
				break;
			}
			case FieldType::Char:
			{
				const char* buf = _value[_name.c_str()].GetString();
				memcpy(buffer, &buf, sizeof(char));
				break;
			}
			case FieldType::Short:
			{
				int buf = _value[_name.c_str()].GetInt();
				short max = std::numeric_limits<short>().max();
				if (buf > max)
				{
					memcpy(buffer, &max, sizeof(short));
				}
				memcpy(buffer, &buf, sizeof(int));
				break;
			}
			case FieldType::Int:
			{
				int buf = _value[_name.c_str()].GetInt();
				memcpy(buffer, &buf, sizeof(int));
				break;
			}
			case FieldType::Long:
			{
				int64_t buf = _value[_name.c_str()].GetInt64();
				memcpy(buffer, &buf, sizeof(int64_t));
				break;
			}
			case FieldType::UShort:
			{
				unsigned buf = _value[_name.c_str()].GetUint();
				unsigned max = std::numeric_limits<unsigned short>().max();
				if (buf > max)
				{
					memcpy(buffer, &max, sizeof(short));
				}
				memcpy(buffer, &buf, sizeof(int));
				break;
			}
			case FieldType::UInt:
			{
				uint32_t buf = _value[_name.c_str()].GetUint();
				memcpy(buffer, &buf, sizeof(int));
				break;
			}
			case FieldType::ULong:
			{
				int64_t buf = _value[_name.c_str()].GetUint();
				memcpy(buffer, &buf, sizeof(int64_t));
				break;
			}
			case FieldType::Vector2:
			{
				rapidjson::Value& _v = _value[_name.c_str()].GetObj();
				Math::Vec2 tmp;
				tmp.deserialize(_v);
				memcpy(buffer, &tmp, sizeof(Math::Vec2));
				break;
			}
			case FieldType::Vector3:
			{
				rapidjson::Value& _v = _value[_name.c_str()].GetObj();
				Math::Vec3 tmp;
				tmp.deserialize(_v);
				memcpy(buffer, &tmp, sizeof(Math::Vec3));
				break;
			}
			}
			setFieldValue(_name, buffer);
			++it;
		}
	}

	void Script::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		if (pScriptClass == nullptr)
			return;
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());
		rapidjson::Value rjName;
		rjName.SetString(name.c_str(), rapidjson::SizeType(name.length()), _doc.GetAllocator());
		_value.AddMember("Name", rjName, _doc.GetAllocator());

		const auto& fieldMap = pScriptClass->mFields;
		auto it = fieldMap.begin();
		while (it != fieldMap.end())
		{
			const std::string& fieldName{ it->first };
			getFieldValue(fieldName, buffer);
			switch (it->second.type)
			{
			case FieldType::Float:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<float*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::Double:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<double*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::Bool:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<bool*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::Char:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<char*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::Short:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<short*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::Int:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<int*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::Long:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<int64_t*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::UShort:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<uint16_t*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::UInt:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<uint32_t*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::ULong:
			{
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), *reinterpret_cast<uint64_t*>(buffer), _doc.GetAllocator());
				break;
			}
			case FieldType::Vector2:
			{
				float* fBuf = reinterpret_cast<float*> (buffer);
				Math::Vec2 tmp{ *(fBuf++),*fBuf };
				rapidjson::Value vec2(rapidjson::kObjectType);
				tmp.serialize(vec2, _doc);
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), vec2, _doc.GetAllocator());
				break;
			}
			case FieldType::Vector3:
			{
				float* fBuf = reinterpret_cast<float*> (buffer);
				Math::Vec3 tmp{ *(fBuf++),*(fBuf++),*fBuf };
				rapidjson::Value vec3(rapidjson::kObjectType);
				tmp.serialize(vec3, _doc);
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), vec3, _doc.GetAllocator());
				break;
			}
			}
			++it;
		}
	}
}

