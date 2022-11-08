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
#include "GameObject/Components/renderer-component.h"
#include "Files/assets-system.h"

namespace Copium
{

	SpriteRenderer::SpriteRenderer(GameObject& _gameObj) :Component(_gameObj, ComponentType::SpriteRenderer)
	{
		PRINT("Renderer Component constructed");
	}

	void SpriteRenderer::deserialize(rapidjson::Value& _value)
	{
		Sprite.deserialize(_value);
	}

	void SpriteRenderer::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), (rapidjson::SizeType)tc.length(), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		// Serialize Sprite
		Sprite.serialize(_value, _doc);

	}


	void SpriteRenderer::inspector_view()
	{
		float sameLinePadding = 16.f;
		bool openPopup = false;

		glm::vec4 clrGLM = Sprite.get_color();
		ImVec4 color = { clrGLM.r, clrGLM.g, clrGLM.b, clrGLM.a };

		int spriteID = (int)Sprite.get_sprite_id();
		
		std::string spriteName = Sprite.get_name();
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
			ImGui::Button(spriteName.c_str(), ImVec2(-FLT_MIN, 0.f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
				{
					std::string str = (const char*)(payload->Data);
					Copium::AssetsSystem* assets = Copium::AssetsSystem::Instance();
					for (int i = 0; i < assets->get_textures().size(); i++)
					{
						if (!assets->get_textures()[i].get_file_path().compare(str))
						{
							spriteID = i + 1;
							PRINT("Sprite ID: " << spriteID);
						}
					}
					size_t pos = str.find_last_of('/');
					spriteName = str.substr(pos + 1, str.length() - pos);
				}
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
			ImGui::Checkbox("X", Sprite.access_flip_x());
			ImGui::SameLine(0.f, sameLinePadding);
			ImGui::Checkbox("Y", Sprite.access_flip_y());

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
			ImGui::ColorPicker4("Picker", Sprite.access_color(), miscFlags);

			ImGui::EndPopup();
		}

		if (spriteID >= 0)
			Sprite.set_sprite_id(spriteID);
		Sprite.set_name(spriteName);
	}

	SpriteRenderer& SpriteRenderer::operator=(const SpriteRenderer& rhs)
	{
		Sprite = rhs.Sprite;
		return *this;
	}
}