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
#include <Files/assets-system.h> 
#include <stdlib.h>  
#include <Utilities/easing.h>
#include <Debugging/frame-rate-controller.h>
#include <SceneManager/scene-manager.h>
#include <GameObject/Components/script-component.h>
#include <GameObject/Components/camera-component.h>
#include <Events/events-system.h>
namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
}

namespace Copium
{
	//Button------------/
	Button* Button::hoveredBtn{nullptr};
	Button::Button(GameObject& _gameObj,Math::Vec2 _min, Math::Vec2 _max) 
		: Component(_gameObj, ComponentType::Button), bounds{_min,_max},
		normalColor{1.f,1.f,1.f,0.5f}, hoverColor{0.5f,1.f,1.f,0.5f}, clickedColor{0.5f},
		targetGraphic{nullptr}
	{
		previousColor = normalColor;
		state = ButtonState::None;
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
			if (targetGraphic)
				previousColor = targetGraphic->layeredColor;
		}
		switch (state)
		{
		case ButtonState::OnClick:
		{
			//PRINT("UI: Clicked " << gameObj.get_name());
			if (targetGraphic)
				targetGraphic->layeredColor = Linear(previousColor, clickedColor, timer / fadeDuration);
			Script* script = gameObj.getComponent<Script>();
			if (script)
			{
				MyEventSystem->publish(new ScriptInvokeMethodEvent(*script,callbackName,nullptr,0));
				if (!gameObj.isActive())
				{
					hoveredBtn = nullptr;
				}
			}
			break;
		}
		case ButtonState::OnHover:
		{
			//PRINT("UI: Hover " << gameObj.get_name());
			if (targetGraphic)
				targetGraphic->layeredColor = Linear(previousColor, hoverColor, timer / fadeDuration);
			break;
		}
		case ButtonState::OnHeld:
		{
			//PRINT("UI: Held " << gameObj.get_name());
			break;
		}
		case ButtonState::OnRelease:
		{
			//PRINT("UI: Released " << gameObj.get_name());
			break;
		}
		default:
		{
			if (targetGraphic)
				targetGraphic->layeredColor = Linear(previousColor, normalColor, timer / fadeDuration);
			break;
		}
		}
		if (targetGraphic == nullptr)
			return;
		if (timer < fadeDuration)
			timer += (float)MyFrameRateController.getDt();
		else if (timer > fadeDuration)
			timer = fadeDuration;
	}


	void Button::deserializeLink(rapidjson::Value& _value) 
	{
		if (_value.HasMember("Graphic ID"))
		{
			ComponentID targetGraphicID = _value["Graphic ID"].GetUint64();
			targetGraphic = reinterpret_cast<Text*>(MySceneManager.findComponentByID(targetGraphicID));
		}
	}

	void Button::previewLink(Component* rhs) 
	{
		if (reinterpret_cast<Button*>(rhs)->targetGraphic)
		{
			ComponentID _ID = reinterpret_cast<Button*>(rhs)->targetGraphic->id;

			Component* foundText = MySceneManager.findComponentByID(_ID);
			if (foundText)
				targetGraphic = reinterpret_cast<Text*>(foundText);
		}
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
							targetGraphic->layeredColor = {0,0,0,0};
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
			Script* script = gameObj.getComponent<Script>();
			if (script)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("On Click Callback: ");
				ImGui::TableNextColumn();
				const char** namesArr{};
				size_t arrSize;
				MyEventSystem->publish(new ScriptGetMethodNamesEvent(*script, namesArr, arrSize));
				ImGui::PushItemWidth(-1);
				if (ImGui::BeginCombo("##functions", callbackName.c_str())) // The second parameter is the label previewed before opening the combo.
				{
					for (size_t i = 0; i < arrSize; ++i)
					{
						bool is_selected = (callbackName.c_str() == namesArr[i]); // You can store your selection however you want, outside or inside your objects
						if (ImGui::Selectable(namesArr[i], is_selected))
							callbackName = namesArr[i];
							if (is_selected)
								ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Bounds");
			ImGui::TableNextColumn();
			if (ImGui::BeginTable("Component AABB", 4, windowFlags))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::PushID(0);
				ImGui::Text("minX"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputFloat("", &bounds.min.x);
				ImGui::PopID();

				ImGui::TableNextColumn();
				ImGui::PushID(1);
				ImGui::Text("minY"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputFloat("", &bounds.min.y);
				ImGui::PopID();

				ImGui::TableNextColumn();
				ImGui::PushID(2);
				ImGui::Text("maxX"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputFloat("", &bounds.max.x);
				ImGui::PopID();

				ImGui::TableNextColumn();
				ImGui::PushID(3);
				ImGui::Text("maxY"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputFloat("", &bounds.max.y);
				ImGui::PopID();

				ImGui::EndTable();
			}

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

	ButtonState Button::getInternalState()
	{
		glm::vec2 scenePos = MySceneManager.mainCamera->get_game_ndc();
		//PRINT("x: " << scenePos.x << " , y: " << scenePos.y);
		if (hoveredBtn == nullptr)
		{
			if (static_collision_pointrect(scenePos, getBounds()))
			{
				if (inputSystem.is_mousebutton_pressed(0))
				{
					hoveredBtn = this;
					if (state == ButtonState::OnClick || state == ButtonState::OnHeld)
						return ButtonState::OnHeld;
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
			if (state == ButtonState::OnClick || state == ButtonState::OnHeld)
				return ButtonState::OnHeld;
			return ButtonState::OnClick;
		}
		return ButtonState::None;
	}

	Component* Button::clone(GameObject& _gameObj) const
	{
		auto* component = new Button(_gameObj);
		component->bounds = bounds;
		component->state = state;
		component->callbackName = callbackName;
		return component;
	}

	void Button::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		_value.AddMember("ID", id, _doc.GetAllocator());

		if (targetGraphic)
			_value.AddMember("Graphic ID", targetGraphic->id, _doc.GetAllocator());
		else
			_value.AddMember("Graphic ID", 0, _doc.GetAllocator());

		rapidjson::Value bb(rapidjson::kObjectType);
		bounds.serialize(bb, _doc);
		_value.AddMember("BoundingBox", bb, _doc.GetAllocator());

		rapidjson::Value rjName;
		rjName.SetString(callbackName.c_str(), _doc.GetAllocator());
		_value.AddMember("Callback", rjName, _doc.GetAllocator());
	}
	void Button::deserialize(rapidjson::Value& _value)
	{

		if (_value.HasMember("ID"))
		{
			id = _value["ID"].GetUint64();
		}

		if (_value.HasMember("Callback"))
		{
			callbackName = _value["Callback"].GetString();
		}

		if (_value.HasMember("BoundingBox"))
		{
			rapidjson::Value& _v = _value["BoundingBox"].GetObj();
			bounds.deserialize(_v);
		}
	}

	AABB Button::getBounds() const
	{
		Math::Vec3& size{ gameObj.transform.scale };
		Math::Vec3& pos{ gameObj.transform.position };
		float x = (bounds.max.x - bounds.min.x) * size.x;
		float y = (bounds.max.y - bounds.min.y) * size.y;
		AABB tmp{ bounds };
		tmp.max.x *= x;
		tmp.min.x *= x;
		tmp.max.y *= y;
		tmp.min.y *= y;
		tmp.max.x += pos.x;
		tmp.min.x += pos.x;
		tmp.max.y += pos.y;
		tmp.min.y += pos.y;
		//PRINT("X: " << tmp.min.x << ", " << tmp.max.x);
		//PRINT("Y: " << tmp.min.y << ", " << tmp.max.y);
		return tmp;
	}

	void Button::set_targetgraphic(Text* _txt)
	{
		targetGraphic = _txt;
	}
	Text* Button::get_targetgraphic() { return targetGraphic; }
	
	//----------------/

	//Text------------/
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
		glm::fvec4 mixedColor{0};
		mixedColor.a = 1 - (1 - layeredColor.a) * (1 - color.a); // 0.75
		if (mixedColor.a < 0.01f)
			return;
		mixedColor.r = layeredColor.r * layeredColor.a / mixedColor.a + color.r * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.67
		mixedColor.g = layeredColor.g * layeredColor.a / mixedColor.a + color.g * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.33
		mixedColor.b = layeredColor.b * layeredColor.a / mixedColor.a + color.b * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.00
	
		/*PRINT("Color: " << color.r << " " << color.g << " " << color.b << " " << color.a);
		PRINT("Mixed Color: " << mixedColor.r << " " << mixedColor.g << " " << mixedColor.b << " " << mixedColor.a);
		*/

		if (gameObj.transform.hasParent())
		{
			Transform& t1 = *gameObj.transform.parent;
			Copium::Math::Matrix3x3 rot;
			Copium::Math::matrix3x3_rotdeg(rot, t1.rotation.z);
			Copium::Math::Vec3 intermediate = (rot * pos);

			font->draw_text(content, intermediate + t1.position, mixedColor, scale, 0, _camera);
		}
		else
		{
			font->draw_text(content, pos, mixedColor, scale, 0, _camera);
		}

		
	}

	Component* Text::clone(GameObject& _gameObj) const
	{
		Text* component = new Text(_gameObj);
		memcpy(component->content, content, TEXT_BUFFER_SIZE);
		component->vAlignment = vAlignment;
		component->hAlignment = hAlignment;
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
			ImGui::InputTextMultiline("##Text", content, TEXT_BUFFER_SIZE, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
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

			//PRINT(color.r << ' ' << color.g << ' ' << color.b << ' ' << color.a);
			ImGui::EndPopup();
		}
	}

	void Text::deserialize(rapidjson::Value& _value)
	{
		if (_value.HasMember("ID"))
		{
			id = _value["ID"].GetUint64();
		}
		if (_value.HasMember("FontName"))
		{
			fontName = _value["FontName"].GetString();
			font = Font::getFont(fontName);
		}
		if (_value.HasMember("H_Align"))
		{
			hAlignment = (HorizontalAlignment)_value["H_Align"].GetInt();
		}
		if (_value.HasMember("V_Align"))
		{
			vAlignment = (VerticalAlignment)_value["V_Align"].GetInt();
		}
		if (_value.HasMember("Content"))
		{
			strcpy(content, _value["Content"].GetString());
		}
		if (_value.HasMember("Font Size"))
		{
			fSize = _value["Font Size"].GetFloat();
		}
		if (_value.HasMember("r"))
		{
			color.r = _value["r"].GetFloat();
		}
		if (_value.HasMember("g"))
		{
			color.g = _value["g"].GetFloat();
		}
		if (_value.HasMember("b"))
		{
			color.b = _value["b"].GetFloat();

		}
		if (_value.HasMember("a"))
		{
			color.a = _value["a"].GetFloat();
		}
	}
	void Text::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		_value.AddMember("ID", id, _doc.GetAllocator());

		type.SetString(fontName.c_str(), rapidjson::SizeType(fontName.length()), _doc.GetAllocator());
		_value.AddMember("FontName", type, _doc.GetAllocator());
		_value.AddMember("H_Align", (int)hAlignment, _doc.GetAllocator());
		_value.AddMember("V_Align", (int)vAlignment, _doc.GetAllocator());

		type.SetString(content, rapidjson::SizeType(strlen(content)), _doc.GetAllocator());
		_value.AddMember("Content", type, _doc.GetAllocator());

		_value.AddMember("Font Size", fSize, _doc.GetAllocator());
		_value.AddMember("r", color.r, _doc.GetAllocator());
		_value.AddMember("g", color.g, _doc.GetAllocator());
		_value.AddMember("b", color.b, _doc.GetAllocator());
		_value.AddMember("a", color.a, _doc.GetAllocator());
	}
	//-----------------/

	//Image------------/
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
		ImVec4 clrIM = { clrGLM.r, clrGLM.g, clrGLM.b, clrGLM.a };

		uint64_t spriteID = sprite.get_sprite_id();

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
							uint64_t pathID = std::hash<std::string>{}(assets->get_texture(i)->get_file_path());
							MetaID metaID = assets->GetMetaID(pathID);
							spriteID = metaID.uuid;

							// Attach Reference
							sprite.set_texture(assets->get_texture(i));
						}
					}
					size_t pos = str.find_last_of('\\');
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
			openPopup = ImGui::ColorButton("Color", clrIM, miscFlags, ImVec2(FLT_MAX, 0));

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
			backupColor = clrIM;
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
		component->vAlignment = vAlignment;
		component->hAlignment = hAlignment;
		component->offset = offset;
		component->sprite = sprite;
		return component;
	}
	//-----------------/

}