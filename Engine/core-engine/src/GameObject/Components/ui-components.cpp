﻿/*!***************************************************************************************
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
#include <Files/assets-system.h> 
#include <stdlib.h>  
#include <Utilities/easing.h>
#include <Debugging/frame-rate-controller.h>
namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
}

namespace Copium
{
	const Button* Button::hoveredBtn{nullptr};

	Button::Button(GameObject& _gameObj,Math::Vec2 _min, Math::Vec2 _max) 
		: Component(_gameObj, ComponentType::Button), bounds{_min,_max},
		normalColor{1.f}, hoverColor{0.5f,1.f,1.f,1.f}, clickedColor{0.5f,0.5f,0.5f,1.f},
		targetGraphic{nullptr}
	{
		previousColor = normalColor;
		state = ButtonState::None;
		for (int i = 0; i <= int(ButtonState::None); ++i)
		{
			mapStateCallbacks.insert({ButtonState(i),nullptr});
		}
	}


	void Button::updateBounds()
	{
		Math::Vec3 pos{ gameObj.transform.position };
		Math::Vec3 scale{ gameObj.transform.scale };
		//Get ratio first
		// Positive ratio, negative ratio for x and y
		Math::Vec2 _min = bounds.min;
		Math::Vec2 _max = bounds.max;
		_min.x *= scale.x;
		_max.x *= scale.x;
		_min.y *= scale.y;
		_max.y *= scale.y;
		relativeBounds = { _min + pos, _max + pos };
	}

	const AABB& Button::getRelativeBounds() const
	{
		return relativeBounds;
	}


	void Button::update()
	{
		updateBounds();
		state = getInternalState();
		if (previousState != state)
		{
			timer = 0;
			previousState = state;
		}
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
		if (targetGraphic == nullptr)
		{
			return;
		}
		switch (state)
		{
		case ButtonState::OnClick:
		{
			targetGraphic->color = Linear(previousColor, clickedColor, timer / fadeDuration);
			break;
		}
		case ButtonState::OnHover:
		{
			targetGraphic->color = Linear(previousColor, hoverColor, timer / fadeDuration);
			break;
		}
		default:
		{
			targetGraphic->color = Linear(previousColor, normalColor, timer / fadeDuration);
			break;
		}
		}
		if (timer < fadeDuration)
			timer += (float)MyFrameRateController.getDt();
		else if (timer > fadeDuration)
			timer = fadeDuration;
	}


	void Button::inspector_view()
	{
		bool openPopup = false;

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;

		static glm::fvec4* pColor{nullptr};

		if (ImGui::BeginTable("Component UI Text", 2, windowFlags))
		{
			ImGui::Indent();
			// Sprite
			// Extern source file
			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Target Graphic");
			ImGui::TableNextColumn();
			if (targetGraphic == nullptr)
				ImGui::Button("Empty", ImVec2(-FLT_MIN, 0.f));
			else
			{

				ImGui::Button((targetGraphic->Name() + " (" + targetGraphic->gameObj.get_name() + ")").c_str(), ImVec2(-FLT_MIN, 0.f));
			}
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Text");
				if (payload)
				{
					Text* pText = (Text*)(*reinterpret_cast<void**>(payload->Data));
					if (pText != targetGraphic)
					{
						if (targetGraphic)
							targetGraphic->color = { 1.f ,1.f,1.f,1.f};
						targetGraphic = pText;
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Normal Color");
			ImGui::TableNextColumn();
			if (ImGui::ColorButton("Normal Color", *reinterpret_cast<ImVec4*>(&normalColor), miscFlags, ImVec2(FLT_MAX, 0)))
			{
				openPopup = true;
				pColor = &normalColor;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Hovered Color");
			ImGui::TableNextColumn();
			if (ImGui::ColorButton("Hovered Color", *reinterpret_cast<ImVec4*>(&hoverColor), miscFlags, ImVec2(FLT_MAX, 0)))
			{
				openPopup = true;
				pColor = &hoverColor;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Clicked Color");
			ImGui::TableNextColumn();
			if (ImGui::ColorButton("Clicked Color", *reinterpret_cast<ImVec4*>(&clickedColor), miscFlags, ImVec2(FLT_MAX, 0)))
			{
				openPopup = true;
				pColor = &clickedColor;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Fade Duration");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::SliderFloat("##Fade Duration", &fadeDuration, 0.01f, 1.f);
			ImGui::PopItemWidth();

			ImGui::Unindent();
			ImGui::EndTable();
		}

		if (openPopup)
		{
			ImGui::OpenPopup("Color");
			windowFlags = ImGuiTableFlags_NoBordersInBody;
		}
		if (ImGui::BeginPopup("Color", windowFlags))
		{
			ImGui::Text("Color");
			ImGui::Separator();
			miscFlags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview
				| ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoLabel;
			ImGui::ColorPicker4("Picker", reinterpret_cast<float*>(pColor), miscFlags);

			ImGui::EndPopup();
		}
	}

	ButtonState Button::getInternalState() const
	{
		glm::vec2 scenePos = EditorSystem::Instance()->get_camera()->get_ndc();
		if (hoveredBtn == nullptr)
		{
			if (static_collision_pointrect(scenePos, relativeBounds))
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

	Component* Button::clone(GameObject& _gameObj) const
	{
		auto* component = new Button(_gameObj);
		component->bounds = bounds;
		component->state = state;
		component->mapStateCallbacks = mapStateCallbacks;
		return component;
	}

	Text::Text(GameObject& _gameObj)
		: Component(_gameObj, ComponentType::Text), font{ Font::getFont("corbel") },fontName{"corbel"}, fSize{1.f}, content{"New Text"}
	{
	}

	void Text::render(BaseCamera* _camera)
	{
		if (!font)
			return;
		Transform& trans{ gameObj.transform };
		Math::Vec3 pos{ trans.position };
		float scale = trans.scale.x;
		if (scale > trans.scale.y)
			scale = trans.scale.y;
		scale *= fSize;
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
		glm::fvec4 mixedColor;
		mixedColor.a = 1 - (1 - layeredColor.a) * (1 - color.a); // 0.75
		if (mixedColor.a < 0.01f)
			return;
		mixedColor.r = layeredColor.r * layeredColor.a / mixedColor.a + color.r * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.67
		mixedColor.g = layeredColor.g * layeredColor.a / mixedColor.a + color.g * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.33
		mixedColor.b = layeredColor.b * layeredColor.a / mixedColor.a + color.b * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.00
		

		/*PRINT("Color: " << color.r << " " << color.g << " " << color.b << " " << color.a);
		PRINT("Layered Color: " << layeredColor.r << " " << layeredColor.g << " " << layeredColor.b << " " << layeredColor.a);
		PRINT("Mixed Color: " << mixedColor.r << " " << mixedColor.g << " " << mixedColor.b << " " << mixedColor.a);
		*/
		font->draw_text(content, pos, mixedColor, scale, 0, _camera);
	}

	Component* Text::clone(GameObject& _gameObj) const
	{
		Text* component = new Text(_gameObj);
		memcpy(component->content, content, TEXT_BUFFER_SIZE);
		component->color = color;
		component->fSize = fSize;
		component->font = font;
		return component;
	}

	void Text::inspector_view()
	{
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
			ImGui::Text("Font Size");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat("##Font Size", &fSize);
			ImGui::PopItemWidth();

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

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Color");
			ImGui::TableNextColumn();
			openPopup = ImGui::ColorButton("Color", *reinterpret_cast<ImVec4*>(&color), miscFlags, ImVec2(FLT_MAX, 0));

			ImGui::Unindent();
			ImGui::EndTable();
		}

		if (openPopup)
		{
			ImGui::OpenPopup("Color");
			windowFlags = ImGuiTableFlags_NoBordersInBody;
		}
		if (ImGui::BeginPopup("Color", windowFlags))
		{
			ImGui::Text("Color");
			ImGui::Separator();
			miscFlags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview
				| ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoLabel;
			ImGui::ColorPicker4("Picker", reinterpret_cast<float*>(&color), miscFlags);

			PRINT(color.r << ' ' << color.g << ' ' << color.b << ' ' << color.a);
			ImGui::EndPopup();
		}
	}

	ImageComponent::ImageComponent(GameObject& _gameObj)
		: Component(_gameObj, ComponentType::Image)
	{

	}

	glm::fvec2 ImageComponent::Offset()
	{
		Transform& trans{ gameObj.transform };
		Math::Vec2 pos{ trans.position };
		glm::vec2 dimensions{ sprite.get_size() };
		if (dimensions.x == 0)
			dimensions.x = 1.f;
		if (dimensions.y == 0)
			dimensions.y = 1.f;
		dimensions.x *= trans.scale.x;
		dimensions.y *= trans.scale.y;

		switch (hAlignment)
		{
		case HorizontalAlignment::Left:
		{
			pos.x += dimensions.x / 2.f;
			break;
		}
		case HorizontalAlignment::Right:
		{
			pos.x -= dimensions.x / 2.f;
			break;
		}
		}
		switch (vAlignment)
		{
		case VerticalAlignment::Top:
		{
			pos.y -= dimensions.y / 2.f;
			break;
		}
		case VerticalAlignment::Bottom:
		{
			pos.y += dimensions.y / 2.f;
			break;
		}
		}
		return pos;
	}

	void ImageComponent::inspector_view()
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

		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Component Image Renderer", 2, windowFlags))
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
						if (!assets->get_texture(i)->get_file_path().compare(str))
						{
							spriteID = i + 1;
						}
					}
					size_t pos = str.find_last_of('/');
					spriteName = str.substr(pos + 1, str.length() - pos);
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Horizontal Alignment: ");
			ImGui::TableNextColumn();
			static const char* const horizontal[] = { "Left", "Center", "Right" };
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
			windowFlags = ImGuiTableFlags_NoBordersInBody;
		}
		if (ImGui::BeginPopup("Color", windowFlags))
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

	Component* ImageComponent::clone(GameObject& _gameObj) const
	{
		ImageComponent* component = new ImageComponent(_gameObj);
		component->offset = offset;
		component->sprite = sprite;
		return component;
	}
}