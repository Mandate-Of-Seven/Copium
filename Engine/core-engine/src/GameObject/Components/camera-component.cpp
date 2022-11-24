/*!***************************************************************************************
\file			camera-component.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/11/2022

\brief
	This file holds the definitions of functions for the camera component, which is used
	when the game plays.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "GameObject/Components/camera-component.h"
#include "Graphics/graphics-system.h"
#include "../game-object.h"

namespace Copium
{
	namespace
	{
		GraphicsSystem* graphics = GraphicsSystem::Instance();
	}

	Camera::Camera(GameObject& _gameObj) : Component(_gameObj, ComponentType::Camera)
	{
		PRINT("  Camera Component constructed");

		// Bean: Checking for archetypes
		BaseCamera::init(1280.f, 720.f, CameraType::GAME, true);
		graphics->get_cameras().push_back(this);
		gameObject = &_gameObj;
	}

	Camera::~Camera()
	{
		PRINT("Camera Component deconstructed");

		BaseCamera::exit();
		graphics->get_cameras().remove(this);
	}

	void Camera::deserialize(rapidjson::Value& _value)
	{
		BaseCamera::deserialize(_value);
	}

	void Camera::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), (rapidjson::SizeType) tc.length(), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		BaseCamera::serialize(_value, _doc);
	}

	void Camera::inspector_view()
	{
		bool openPopup = false;

		glm::vec4 clrGLM = backgroundColor;

		ImVec4 color = { clrGLM.r, clrGLM.g, clrGLM.b, clrGLM.a };

		static ImVec4 backupColor;

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Component Camera", 2, tableFlags))
		{
			ImGui::Indent();
			// Sprite
			// Extern source file

			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			// Background Color
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Background Color");
			ImGui::TableNextColumn();
			openPopup = ImGui::ColorButton("Color", color, miscFlags, ImVec2(FLT_MAX, 0));

			// Projection
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Projection");
			ImGui::TableNextColumn();

			ImGui::Text("Orthographic"); ImGui::SameLine();
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::Checkbox("", &orthographic);
			ImGui::PopItemFlag();

			// Clipping Planes
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Clipping Planes");
			ImGui::TableNextColumn();

			ImGui::PushID(0);
			ImGui::Text("Near"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::InputFloat("", &nearClip);
			if (ImGui::IsItemEdited())
				update_ortho_projection();
			ImGui::PopID();

			ImGui::PushID(1);
			ImGui::Text("Far"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::InputFloat("", &farClip);
			if (ImGui::IsItemEdited())
				update_ortho_projection();
			ImGui::PopID();

			ImGui::Unindent();
			ImGui::EndTable();
		}

		if (openPopup)
		{
			ImGui::OpenPopup("Color");
			backupColor = color;
			tableFlags = ImGuiTableFlags_NoBordersInBody;
		}
		if (ImGui::BeginPopup("Color", tableFlags))
		{
			ImGui::Text("Color");
			ImGui::Separator();
			miscFlags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview
				| ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoLabel;
			ImGui::ColorPicker4("Picker", &(backgroundColor.x), miscFlags);

			ImGui::EndPopup();
		}

		focalPoint = gameObject->transform.position;
	}
} 