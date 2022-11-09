/*!***************************************************************************************
\file			scripting-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	This file holds the definition of UI components

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Windows/windows-input.h"
#include "GameObject/Components/ui-components.h"
#include "GameObject/game-object.h"
#include "Physics/collision.h"
#include "Editor/editor-system.h"
#include "Files/assets-system.h"

namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
}

namespace Copium
{
	const ButtonComponent* ButtonComponent::hoveredBtn{nullptr};

	ButtonComponent::ButtonComponent(GameObject& _gameObj,Math::Vec2 _min, Math::Vec2 _max) 
		: Component(_gameObj, ComponentType::Button)
	{
		min = _min;
		max = _max;
		state = ButtonState::None;
		for (int i = 0; i <= int(ButtonState::None); ++i)
		{
			mapStateCallbacks.insert({ButtonState(i),nullptr});
		}
	}


	void ButtonComponent::update()
	{
		state = getInternalState();

		ButtonCallback callback = mapStateCallbacks[state];
		if (callback != nullptr)
		{
			callback();
		}
		switch (state)
		{
		case ButtonState::OnClick:
		{
			PRINT("UI: Clicking on " << gameObj.get_name());
			break;
		}
		case ButtonState::OnHover:
		{
			PRINT("UI: Hovering on " << gameObj.get_name());
			break;
		}
		case ButtonState::OnRelease:
		{
			PRINT("UI: Released on " << gameObj.get_name());
			break;
		}
		}
	}

	ButtonState ButtonComponent::getInternalState() const
	{
		static const Math::Vec3& pos{ gameObj.Transform().position };
		AABB newBounds{ pos + min, pos + max };
		glm::vec2 scenePos = EditorSystem::Instance()->get_camera()->get_ndc();
		if (hoveredBtn == nullptr)
		{
			if (static_collision_pointrect(scenePos, newBounds))
			{
				if (inputSystem.is_mousebutton_pressed(0))
				{
					hoveredBtn = this;
					return ButtonState::OnClick;
				}
				return ButtonState::OnHover;
			}
		}
		else if (hoveredBtn == this)
		{
			if (!inputSystem.is_mousebutton_pressed(0))
			{
				hoveredBtn = nullptr;
				return ButtonState::OnRelease;
			}
			return ButtonState::OnClick;
		}
		return ButtonState::None;
	}

	ButtonComponent& ButtonComponent::operator=(const ButtonComponent& rhs)
	{
		min = rhs.min;
		max = rhs.max;
		state = rhs.state;
		mapStateCallbacks = rhs.mapStateCallbacks;
		return *this;
	}

	TextComponent::TextComponent(GameObject& _gameObj)
		: Component(_gameObj, ComponentType::Text), font{ Font::getFont("corbel") },fontName{"corbel"}
	{
	}

	void TextComponent::render()
	{
		if (!font)
			return;
		Transform& trans{ gameObj.Transform() };
		glm::vec2 pos{ trans.position.to_glm() };
		float scale = trans.scale.x;
		if (scale > trans.scale.y)
			scale = trans.scale.y;
		glm::vec2 dimensions{ font->getDimensions(content, scale) };


		switch (hAlignment)
		{
			case HorizontalAlignment::Center:
			{
				pos.x -= dimensions.x / 2.f;
				break;
			}
			case HorizontalAlignment::Right:
			{
				pos.x -= dimensions.x;
				break;
			}
		}
		switch (vAlignment)
		{
			case VerticalAlignment::Top:
			{
				pos.y -= dimensions.y;
				break;
			}
			case VerticalAlignment::Center:
			{
				pos.y -= dimensions.y / 2.f;
				break;
			}
		}

		font->draw_text(content, pos, { 1.f, 1.f, 1.f, 1.f }, scale, 0);
	}

	TextComponent& TextComponent::operator=(const TextComponent& rhs)
	{
		offset = rhs.offset;
		memcpy(content, rhs.content, TEXT_BUFFER_SIZE);
		font = rhs.font;
		return *this;
	}


	void TextComponent::inspector_view()
	{
		float sameLinePadding = 16.f;
		bool openPopup = false;

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Component UI Text", 2, windowFlags))
		{
			ImGui::Indent();
			// Sprite
			// Extern source file

			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Content:");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##Text", content, TEXT_BUFFER_SIZE);
			ImGui::PopItemWidth();


			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Horizontal Alignment: ");
			ImGui::TableNextColumn();
			static const char* const horizontal[] = { "Left", "Center", "Right"};
			ImGui::PushItemWidth(-1);
			ImGui::Combo("hAlign", reinterpret_cast<int*>(&hAlignment), horizontal, 3);
			ImGui::PopItemWidth();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Vertical Alignment: ");
			ImGui::TableNextColumn();
			static const char* const vertical[] = { "Top", "Center", "Bottom" };
			ImGui::PushItemWidth(-1);
			ImGui::Combo("vAlign", reinterpret_cast<int*>(&vAlignment), vertical, 3);
			ImGui::PopItemWidth();

			ImGui::Unindent();
			ImGui::EndTable();
		}
	}

	ImageComponent::ImageComponent(GameObject& _gameObj)
		: Component(_gameObj, ComponentType::Image)
	{

	}

	void ImageComponent::render()
	{

	}

	void ImageComponent::inspector_view() 
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
		if (ImGui::BeginTable("ImageComponent", 2, windowFlags))
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
	};

	ImageComponent& ImageComponent::operator=(const ImageComponent& rhs)
	{
		offset = rhs.offset;
		return *this;
	}
}