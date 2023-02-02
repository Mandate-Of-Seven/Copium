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
#include "SceneManager/scene-manager.h"
#define DEFAULT_SCRIPT_NAME "NewScript"
#include <Events/events-system.h>

#define BUFFER_SIZE 128

namespace Copium
{
	char Script::buffer[BUFFER_SIZE];
	std::pair<const std::string, Field>* Script::editedField;
	bool Script::isAddingReference{nullptr};

	Script::Script(GameObject& _gameObj) :
		Component(_gameObj, ComponentType::Script), name{ "" }, reference{nullptr}
	{
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_SCENE_DESERIALIZED, this);
	}

	Script::~Script()
	{
		MessageSystem::Instance()->unsubscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		MessageSystem::Instance()->unsubscribe(MESSAGE_TYPE::MT_SCENE_DESERIALIZED, this);
	}

	void Script::instantiate()
	{
		MyEventSystem->publish(new ReflectComponentEvent(*this));
	}

	void Script::handleMessage(MESSAGE_TYPE mType)
	{
		switch (mType)
		{
		case MESSAGE_TYPE::MT_SCRIPTING_UPDATED:
		{


			//CreateFields
		}
		case MESSAGE_TYPE::MT_SCENE_DESERIALIZED:
			for (auto pair : fieldComponentReferences)
			{
				MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, pair.first.c_str(), pair.second));
			}

			for (auto pair : fieldGameObjReferences)
			{
				MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, pair.first.c_str(), pair.second));
			}
			break;

			//CreateFields
		}

	}

	const std::string& Script::Name() const
	{
		return name;
	}

	void Script::Name(const std::string& _name)
	{
		name = _name;
		instantiate();
	}

	//Use for serialization
	void Script::GetFieldValue(const std::string& _name, char* outBuffer)
	{
		MyEventSystem->publish(new ScriptGetFieldEvent(*this, _name.c_str(), (void*)outBuffer));
	}

	void Script::SetFieldValue(const std::string& _name, const char* value)
	{
		MyEventSystem->publish(new ScriptSetFieldEvent(*this, _name.c_str(), (void*)value));
	}

	void Script::inspector_view()
	{
		static ImVec4 backupColor;

		int counter{ 0 };

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable(name.c_str(), 2, windowFlags))
		{
			ImGui::Indent();
			// Extern source file
			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);
			auto it = fieldDataReferences.begin();
			while (it != fieldDataReferences.end())
			{
				const std::string& fieldName{ it->first };
				Field& field{ it->second };

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(fieldName.c_str());
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(-FLT_MIN);

				if (field.fType == FieldType::Component)
				{
					auto componentRef = fieldComponentReferences.find(it->first);
					if (componentRef == fieldComponentReferences.end())
					{
						std::string displayName = "None(" + it->second.typeName + ")";
						ImGui::Button(displayName.c_str(), ImVec2(-FLT_MIN, 0.f));
						//PRINT(it->first);
					}
					else
					{
						std::string displayName = (*componentRef).second->gameObj.get_name() + "(" + it->second.typeName + ")";
						ImGui::Button(displayName.c_str(), ImVec2(-FLT_MIN, 0.f));
					}
					
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						std::cout << "double clicked on game object reference field\n";
						isAddingReference = true;
						editedField = &(*it);

					}

					if (ImGui::BeginDragDropTarget())
					{
						//GameObject ID, Component ID
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(it->second.typeName.c_str());
						if (payload)
						{
							Component* pComponent = (Component*)(*reinterpret_cast<void**>(payload->Data));
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, fieldName.c_str(), pComponent));
							fieldComponentReferences[fieldName] = pComponent;
						}
						ImGui::EndDragDropTarget();
					}
					++it;
					continue;
				}
				else if (field.fType == FieldType::GameObject)
				{
					ImGui::PushID(counter++);
					auto gameObjRef = fieldGameObjReferences.find(it->first);
					if (gameObjRef == fieldGameObjReferences.end())
					{
						ImGui::Button("None(GameObject)", ImVec2(-FLT_MIN, 0.f));
						///////////////////////
					}
					else
					{
						std::string displayName = (*gameObjRef).second->get_name() + "(GameObject)";
						ImGui::Button(displayName.c_str(), ImVec2(-FLT_MIN, 0.f));
					}


					// Matthew's stuff/////
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						std::cout << "double clicked on game object reference field\n";
						isAddingReference = true;
						editedField = &(*it);
					}

					ImGui::PopID();


					if (ImGui::BeginDragDropTarget())
					{
						//GameObject ID, Component ID
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject");
						if (payload)
						{
							GameObject* pGameObj = (GameObject*)(*reinterpret_cast<void**>(payload->Data));
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, fieldName.c_str(), pGameObj));
							fieldGameObjReferences[fieldName] = pGameObj;
						}
						ImGui::EndDragDropTarget();
					}
					++it;
					continue;
				}

				GetFieldValue(fieldName, buffer);
				switch (field.fType)
				{
				case FieldType::Float:
					ImGui::InputFloat(fieldName.c_str(), reinterpret_cast<float*>(buffer));
					break;
				case FieldType::Double:
					ImGui::InputDouble(fieldName.c_str(), reinterpret_cast<double*>(buffer));
					break;
				case FieldType::Bool:
					ImGui::Checkbox(fieldName.c_str(), reinterpret_cast<bool*>(buffer));
					break;
				case FieldType::Char:
					ImGui::InputInt(fieldName.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Short:
					ImGui::InputInt(fieldName.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Int:
					ImGui::InputInt(fieldName.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Long:
					ImGui::InputInt2(fieldName.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::UShort:
					break;
				case FieldType::UInt:
					break;
				case FieldType::ULong:
					break;
				case FieldType::String:
				{
					ImGui::InputTextMultiline(fieldName.c_str(), buffer, BUFFER_SIZE);
					break;
				}
				case FieldType::Vector2:
				{
					if (ImGui::BeginTable(fieldName.c_str(), 3, windowFlags))
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
					if (ImGui::BeginTable(fieldName.c_str(), 3, windowFlags))
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
				SetFieldValue(fieldName, buffer);
				++it;
			}

			ImGui::Unindent();
			ImGui::EndTable();

		}


		if (editedField && isAddingReference)
		{
			// Open pop-up window
			ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
			ImGui::PushID(counter++);
			ImGui::Begin("Add Reference", &isAddingReference);
			ImVec2 buttonSize = ImGui::GetWindowSize();
			buttonSize.y *= (float)0.1;
			static ImGuiTextFilter filter;
			ImGui::PushItemWidth(-1);
			filter.Draw("##References");
			ImGui::PopItemWidth();
			Scene* scene = Copium::SceneManager::Instance()->get_current_scene();

			const std::string& fieldName{ editedField->first };
			Field& field{ editedField->second };

			switch (field.fType)
			{
			case FieldType::Component:
			{
				// Iterate through game object list
				for (Copium::GameObject* go : scene->gameObjects)
				{
					Component* pComponent = {};
					for (Component* component : go->components)
					{
						//
						if (component->Name() == field.typeName)
						{
							pComponent = component;
							break;
						}
					}
					if (!pComponent)
						continue;
					if (filter.PassFilter(go->get_name().c_str()) && ImGui::Button(go->get_name().c_str(), buttonSize))
					{
						isAddingReference = false;
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, fieldName.c_str(), pComponent));

						//Reset Panel
						fieldComponentReferences[fieldName] = pComponent;
						editedField = nullptr;
						break;
					}
				}
				break;
			}
			case FieldType::GameObject:
			{
				// Iterate through game object list
				for (Copium::GameObject* go : scene->gameObjects)
				{
					if (filter.PassFilter(go->get_name().c_str()) && ImGui::Button(go->get_name().c_str(), buttonSize))
					{
						isAddingReference = false;
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, fieldName.c_str(), go));

						//Reset Panel
						fieldGameObjReferences[fieldName] = go;
						editedField = nullptr;
						break;
					}
				}
				break;
			}
			}
			ImGui::End();
			ImGui::PopID();
		}
	}

	Component* Script::clone(GameObject& _gameObj) const
	{
		//mono_field_get_value(mObject, pScriptClass->mFields["gameObj"].classField, buffer);
		Script* component = new Script(_gameObj);
		component->Name(name);
		return component;
	}


	void Script::previewLink(Component* rhs)
	{
		Script* scriptRhs = reinterpret_cast<Script*>(rhs);
		for (auto pair : scriptRhs->fieldGameObjReferences)
		{
			fieldGameObjReferences[pair.first] = MySceneManager.findGameObjByID(pair.second->id);
		}

		for (auto pair : scriptRhs->fieldComponentReferences)
		{
			fieldComponentReferences[pair.first] = MySceneManager.findComponentByID(pair.second->id);
		}

		for (auto pair : scriptRhs->fieldDataReferences)
		{
			Field field{ scriptRhs->fieldDataReferences[pair.first] };
			fieldDataReferences.emplace(pair.first,std::move(field));
		}
		
	}

	void Script::deserialize(rapidjson::Value& _value)
	{
		Component::deserialize(_value);
		if (_value.HasMember("Name"))
		{
			Name(_value["Name"].GetString());
		}
		for (auto it = fieldDataReferences.begin(); it != fieldDataReferences.end(); ++it)
		{
			const std::string& _name{ it->first };
			if (!_value.HasMember(_name.c_str()))
				continue;
			FieldType fType = it->second.fType;
			Field& field{ fieldDataReferences[_name]};

			switch (fType)
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
			case FieldType::Component:
			case FieldType::GameObject:
			{
				uint64_t buf = _value[_name.c_str()].GetUint64();
				memcpy(buffer, &buf, sizeof(uint64_t));
				break;
			}
			case FieldType::ULong:
			{
				int64_t buf = _value[_name.c_str()].GetUint();
				memcpy(buffer, &buf, sizeof(int64_t));
				break;
			}
			case FieldType::String:
			{
				const char* buf = _value[_name.c_str()].GetString();
				memcpy(buffer, &buf, sizeof(char));
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
			SetFieldValue(_name, buffer);
		}
	}

	void Script::deserializeLink(rapidjson::Value& _value)
	{
		for (auto it = fieldDataReferences.begin(); it != fieldDataReferences.end(); ++it)
		{
			const std::string& _name{ it->first };
			Field& field = it->second;
			FieldType fType = field.fType;

			switch (fType)
			{
				case FieldType::Component:
				{
					uint64_t id{ field.Get<uint64_t>()};
					PRINT(_name << " COMPONENT: " << id);
					Component* pComponent = MySceneManager.findComponentByID(id);
					if (pComponent)
						fieldComponentReferences[_name] = pComponent;
					PRINT("");
					break;
				}
				case FieldType::GameObject:
				{
					uint64_t id{ field.Get<uint64_t>() };
					PRINT(_name << " GAMEOBJECT: " << id);
					GameObject* pGameObject = MySceneManager.findGameObjByID(id);
					if (pGameObject)
						fieldGameObjReferences[_name] = pGameObject;
					break;
				}
			}
		}
	}

	void Script::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Component::serialize(_value, _doc);
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());
		rapidjson::Value rjName;
		rjName.SetString(name.c_str(), rapidjson::SizeType(name.length()), _doc.GetAllocator());
		_value.AddMember("Name", rjName, _doc.GetAllocator());

		auto it = fieldDataReferences.begin();
		while (it != fieldDataReferences.end())
		{
			const std::string& fieldName{ it->first };
			GetFieldValue(fieldName, buffer);
			switch (it->second.fType)
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
			case FieldType::String:
			{
				rapidjson::Value type;
				type.SetString(buffer, rapidjson::SizeType(BUFFER_SIZE), _doc.GetAllocator());
				_value.AddMember(rapidjson::StringRef(fieldName.c_str()), type, _doc.GetAllocator());
				//_value.AddMember(rapidjson::StringRef(fieldName.c_str()), std::string(buffer), _doc.GetAllocator());
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

		auto gameObjsIt = fieldGameObjReferences.begin();
		while (gameObjsIt != fieldGameObjReferences.end())
		{
			const std::string& fieldName{ gameObjsIt->first };
			_value.AddMember(rapidjson::StringRef(fieldName.c_str()), (*gameObjsIt).second->id, _doc.GetAllocator());
			++gameObjsIt;
		}

		auto componentsIt = fieldComponentReferences.begin();
		while (componentsIt != fieldComponentReferences.end())
		{
			const std::string& fieldName{ componentsIt->first };
			_value.AddMember(rapidjson::StringRef(fieldName.c_str()), (*componentsIt).second->id, _doc.GetAllocator());
			++componentsIt;
		}
	}
}

