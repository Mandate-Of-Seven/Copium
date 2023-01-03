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

	SpriteRenderer::SpriteRenderer(ComponentID _entityID) :Component(_entityID, ComponentType::SpriteRenderer), isAddingSprite{false}
	{
	}

	void SpriteRenderer::deserialize(rapidjson::Value& _value)
	{
		Component::deserialize(_value);
		sprite.deserialize(_value);
	}

	void SpriteRenderer::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Component::serialize(_value, _doc);
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), (rapidjson::SizeType)tc.length(), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		// Serialize Sprite
		sprite.serialize(_value, _doc);

	}

	void SpriteRenderer::inspector_view()
	{
		float sameLinePadding = 16.f;
		bool openPopup = false;

		glm::vec4 clrGLM = sprite.get_color();
		ImVec4 color = { clrGLM.r, clrGLM.g, clrGLM.b, clrGLM.a };

		int spriteID = (int)sprite.get_sprite_id();
		
		std::string spriteName = sprite.get_name();
		static ImVec4 backupColor;

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Component Sprite Renderer", 2, tableFlags))
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

			// Pop up window for sprite selection
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				isAddingSprite = true;
			}
			if (isAddingSprite)
			{
				// Open pop-up window
				ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
				ImGui::Begin("Add Sprite", &isAddingSprite);
				ImVec2 buttonSize = ImGui::GetWindowSize();
				buttonSize.y *= (float)0.1;
				static ImGuiTextFilter filter;
				ImGui::PushItemWidth(-1);
				filter.Draw("##SpriteName");
				ImGui::PopItemWidth();
				AssetsSystem& assets = AssetsSystem::Instance();
				for (int i = 0; i < assets.get_textures().size(); i++)
				{
					size_t startPos = assets.get_texture(i)->get_file_path().find_last_of('/');
					std::string name = assets.get_texture(i)->get_file_path().substr(startPos + 1, assets.get_texture(i)->get_file_path().length() - startPos);
					if (ImGui::Button(name.c_str(), buttonSize))
					{
						if (filter.PassFilter(name.c_str()))
						{
							spriteName = name;
							isAddingSprite = false;
							spriteID = i + 1;
						}
					}

				}
				ImGui::End();


			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
				{
					std::string str = (const char*)(payload->Data);
					AssetsSystem& assets = AssetsSystem::Instance();
					for (int i = 0; i < assets.get_textures().size(); i++)
					{
;						if (!assets.get_texture(i)->get_file_path().compare(str))
						{
							spriteID = i + 1;
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
			ImGui::Checkbox("X", sprite.access_flip_x());
			ImGui::SameLine(0.f, sameLinePadding);
			ImGui::Checkbox("Y", sprite.access_flip_y());

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
			ImGui::ColorPicker4("Picker", sprite.access_color(), miscFlags);

			ImGui::EndPopup();
		}

		if (spriteID >= 0)
			sprite.set_sprite_id(spriteID);
		sprite.set_name(spriteName);
	}


	Component* SpriteRenderer::clone(ComponentID _entityID) const
	{
		SpriteRenderer* component = new SpriteRenderer(_entityID);
		component->sprite = sprite;
		return component;
	}
}