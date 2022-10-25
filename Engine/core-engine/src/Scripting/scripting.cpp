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
#include "Scripting/scripting.h"
#include "GameObject/game-object.h"

#define DEFAULT_SCRIPT_NAME "NewScript"
#include <mono/jit/jit.h>
namespace Copium
{
	using namespace Scripting;
	ScriptingSystem& ScriptComponent::sS{ *ScriptingSystem::Instance() };

	ScriptComponent::ScriptComponent(GameObject& _gameObj) :
		mObject{ nullptr }, pScriptClass{ nullptr }, Component(_gameObj, Component::Type::Script), name{ DEFAULT_SCRIPT_NAME }
	{
		Message::MessageSystem::Instance()->subscribe(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		pScriptClass = sS.getScriptClass(name);
		instantiate();
	}

	ScriptComponent::~ScriptComponent()
	{
	}

	void ScriptComponent::instantiate()
	{
		if (pScriptClass != nullptr)
		{
			mObject = sS.instantiateClass(pScriptClass->mClass);
			GameObjectID id = gameObj.get_id();
			void* param = &id;
			if (!pScriptClass->mOnCreate)
			{
				PRINT("ON CREATE DOES NOT EXIST");
			}
			sS.invoke(mObject, pScriptClass->mOnCreate, &param);
		}
	}

	void ScriptComponent::handleMessage(Message::MESSAGE_TYPE mType)
	{
		//MT_SCRIPTING_UPDATED
		pScriptClass = sS.getScriptClass(name.c_str());
		instantiate();
	}

	const std::string& ScriptComponent::Name() const
	{
		return name;
	}

	void ScriptComponent::Name(const std::string& _name)
	{
		name = _name;		
		pScriptClass = sS.getScriptClass(name);
		instantiate();
	}

	void ScriptComponent::Awake()
	{
		if (pScriptClass && pScriptClass->mAwake)
			sS.invoke(mObject, pScriptClass->mAwake);
	}

	void ScriptComponent::Start()
	{
		if (pScriptClass && pScriptClass->mStart)
			sS.invoke(mObject, pScriptClass->mStart);
	}

	void ScriptComponent::Update()
	{
		if (pScriptClass && pScriptClass->mUpdate)
			sS.invoke(mObject, pScriptClass->mUpdate);
	}

	void ScriptComponent::LateUpdate()
	{
		if (pScriptClass && pScriptClass->mLateUpdate)
			sS.invoke(mObject, pScriptClass->mLateUpdate);
	}

	void ScriptComponent::OnCollisionEnter()
	{
		if (pScriptClass && pScriptClass->mOnCollisionEnter)
			sS.invoke(mObject, pScriptClass->mOnCollisionEnter);
	}

	//Use for serialization
	bool ScriptComponent::getFieldValue(const std::string& name, void* buffer)
	{
		const auto& it = pScriptClass->mFields.find(name);
		if (it == pScriptClass->mFields.end())
			return false;
		const Field& field = it->second;
		mono_field_get_value(mObject, field.classField, buffer);
		return true;
	}

	bool ScriptComponent::setFieldValue(const std::string& name, const void* value)
	{
		const auto& it = pScriptClass->mFields.find(name);
		if (it == pScriptClass->mFields.end())
			return false;
		const Field& field = it->second;
		mono_field_set_value(mObject, field.classField, (void*)value);
		return true;
	}

	void ScriptComponent::inspector_view()
	{
		if (!pScriptClass)
			return;
		float Padding = 16.f;
		float sameLinePadding = 16.f;
		bool openPopup = false;

		static ImVec4 backupColor;

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable(name.c_str(), 2, windowFlags))
		{
			ImGui::Indent();
			// Sprite
			// Extern source file
			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			const auto& fieldMap = pScriptClass->mFields;
			auto it = fieldMap.begin();
			while (it != fieldMap.end())
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				const std::string& name{ it->first };
				ImGui::Text(name.c_str());
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(-FLT_MIN);
				using namespace Scripting;
				static char buffer[32];
				getFieldValue(name, buffer);
				switch (it->second.type)
				{
				case FieldType::Float:
					ImGui::InputFloat(name.c_str(), reinterpret_cast<float*>(buffer));
					break;
				case FieldType::Double:
					ImGui::InputDouble(name.c_str(), reinterpret_cast<double*>(buffer));
					break;
				case FieldType::Bool:
					ImGui::Checkbox(name.c_str(), reinterpret_cast<bool*>(buffer));
					break;
				case FieldType::Char:
					ImGui::InputInt(name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Byte:
					ImGui::InputInt(name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Short:
					ImGui::InputInt(name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Int:
					ImGui::InputInt(name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::Long:
					ImGui::InputInt2(name.c_str(), reinterpret_cast<int*>(buffer));
					break;
				case FieldType::UByte:				
					break;
				case FieldType::UShort:				
					break;
				case FieldType::UInt:				
					break;
				case FieldType::ULong:				
					break;
				case FieldType::Vector2:
					ImGui::InputFloat2(name.c_str(), reinterpret_cast<float*>(buffer));
					break;
				case FieldType::Vector3:
					ImGui::InputFloat3(name.c_str(), reinterpret_cast<float*>(buffer));
					break;
				}
				setFieldValue(name, buffer);
				++it;
			}
			ImGui::Unindent();
			ImGui::EndTable();
		}
	}
}

