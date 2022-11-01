/*!***************************************************************************************
\file			renderer-component.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	This file holds the declaration of the Renderer component class.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "GameObject/renderer-component.h"

namespace Copium
{

	RendererComponent::RendererComponent(GameObject& _gameObj) :Component(_gameObj, ComponentType::Renderer)
	{
		PRINT("Renderer Component constructed");
	}

	void RendererComponent::deserialize(rapidjson::Value& _value)
	{
		spriteRenderer.deserialize(_value);
	}

	void RendererComponent::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{

	}


	void RendererComponent::inspector_view()
	{
		float sameLinePadding = 16.f;
		bool openPopup = false;

		glm::vec4 clrGLM = spriteRenderer.get_color();
		ImVec4 color = { clrGLM.r, clrGLM.g, clrGLM.b, clrGLM.a };

		int spriteID = (int)spriteRenderer.get_sprite_id();

		static ImVec4 backupColor;

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Component Sprite Renderer", 2, windowFlags))
		{
			ImGui::Indent();
			// Sprite
			// Extern source file

			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Sprite");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-FLT_MIN);
			ImGui::InputInt("", &spriteID, 0);

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem");
				PRINT("Filename: " << (char*)payload->Data);
				ImGui::EndDragDropTarget();
			}

			// Color
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Color");
			ImGui::TableNextColumn();
			openPopup = ImGui::ColorButton("Color", color, miscFlags, ImVec2(FLT_MAX, 0));

			// Flip
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Flip");
			ImGui::TableNextColumn();
			ImGui::Checkbox("X", spriteRenderer.access_flip_x());
			ImGui::SameLine(0.f, sameLinePadding);
			ImGui::Checkbox("Y", spriteRenderer.access_flip_y());

			ImGui::Unindent();
			ImGui::EndTable();
		}

		if (openPopup)
		{
			ImGui::OpenPopup("Color");
			backupColor = color;
			windowFlags = ImGuiTableFlags_NoBordersInBody;
		}
		if (ImGui::BeginPopup("Color", windowFlags))
		{
			ImGui::Text("Color");
			ImGui::Separator();
			miscFlags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview
				| ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoLabel;
			ImGui::ColorPicker4("Picker", spriteRenderer.access_color(), miscFlags);

			ImGui::EndPopup();
		}

		if (spriteID >= 0)
			spriteRenderer.set_sprite_id(spriteID);
	}

	RendererComponent& RendererComponent::operator=(const RendererComponent& rhs)
	{
		spriteRenderer = rhs.spriteRenderer;
		return *this;
	}
}