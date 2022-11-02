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
#include "Graphics/ui-components.h"
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
	const UIButtonComponent* UIButtonComponent::hoveredBtn{nullptr};

	UIButtonComponent::UIButtonComponent(GameObject& _gameObj,Math::Vec2 _min, Math::Vec2 _max) 
		: Component(_gameObj, ComponentType::UIButton)
	{
		min = _min;
		max = _max;
		state = UIButtonState::None;
		for (int i = 0; i <= int(UIButtonState::None); ++i)
		{
			mapStateCallbacks.insert({UIButtonState(i),nullptr});
		}
	}


	void UIButtonComponent::update()
	{
		state = getInternalState();

		UIButtonCallback callback = mapStateCallbacks[state];
		if (callback != nullptr)
		{
			callback();
		}
		switch (state)
		{
		case UIButtonState::OnClick:
		{
			PRINT("UI: Clicking on " << gameObj.get_name());
			break;
		}
		case UIButtonState::OnHover:
		{
			PRINT("UI: Hovering on " << gameObj.get_name());
			break;
		}
		case UIButtonState::OnRelease:
		{
			PRINT("UI: Released on " << gameObj.get_name());
			break;
		}
		}
	}

	UIButtonState UIButtonComponent::getInternalState() const
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
					return UIButtonState::OnClick;
				}
				return UIButtonState::OnHover;
			}
		}
		else if (hoveredBtn == this)
		{
			if (!inputSystem.is_mousebutton_pressed(0))
			{
				hoveredBtn = nullptr;
				return UIButtonState::OnRelease;
			}
			return UIButtonState::OnClick;
		}
		return UIButtonState::None;
	}

	UIButtonComponent& UIButtonComponent::operator=(const UIButtonComponent& rhs)
	{
		min = rhs.min;
		max = rhs.max;
		state = rhs.state;
		mapStateCallbacks = rhs.mapStateCallbacks;
		return *this;
	}

	UITextComponent::UITextComponent(GameObject& _gameObj)
		: Component(_gameObj, ComponentType::UIText), font{Font::getFont("corbel")}
	{
	}

	void UITextComponent::render()
	{
		if (!font)
			return;
		font->draw_text("Button!", gameObj.Transform().position.to_glm(), { 1.f, 1.f, 1.f, 1.f }, 0.2f, 0);
	}

	UITextComponent& UITextComponent::operator=(const UITextComponent& rhs)
	{
		offset = rhs.offset;
		percentage = rhs.percentage;
		content = rhs.content;
		font = rhs.font;
		return *this;
	}

	UIImageComponent::UIImageComponent(GameObject& _gameObj)
		: Component(_gameObj, ComponentType::UIImage)
	{

	}

	void UIImageComponent::render()
	{

	}

	void UIImageComponent::inspector_view() 
	{
		float sameLinePadding = 16.f;
		bool openPopup = false;

		glm::vec4 clrGLM = spriteRenderer.get_color();
		ImVec4 color = { clrGLM.r, clrGLM.g, clrGLM.b, clrGLM.a };

		int spriteID = (int)spriteRenderer.get_sprite_id();

		std::string spriteName = spriteRenderer.get_name();
		static ImVec4 backupColor;

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("UIImageComponent", 2, windowFlags))
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
		spriteRenderer.set_name(spriteName);
	};

	UIImageComponent& UIImageComponent::operator=(const UIImageComponent& rhs)
	{
		offset = rhs.offset;
		percentage = rhs.percentage;
		return *this;
	}
}