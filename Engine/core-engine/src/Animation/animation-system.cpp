/*!***************************************************************************************
\file			animation-system.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			05/01/2023

\brief
	This file holds the definition of the Animation system. The Animation system animates
	the spritesheets in the engine, where the user has to create the animation in the
	animator in the editor.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Animation/animation-system.h"
#include "SceneManager/scene-manager.h"
#include "Debugging/frame-rate-controller.h"
#include "Files/assets-system.h"
#define MAX_ANIMATION_COUNT 5

namespace
{
	Copium::SceneManager* sm= Copium::SceneManager::Instance();
}

namespace Copium
{

	Animator::Animator(GameObject& _gameObj) 
		: Component(_gameObj, ComponentType::Animator), currentAnimationIndex{0},startingAnimationIndex{0}, animationCount{0}, loop{true}, status{AnimatorStatus::idle}
	{

	}

	void Animator::inspector_view()
	{
		std::string toggleAnimation = "Play";
		if (status == AnimatorStatus::playing)
			toggleAnimation = "Stop";

		ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
			| ImGuiColorEditFlags_NoLabel;

		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Component Animator", 2, tableFlags))
		{
			ImGui::Indent();

			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			// Animation Looping
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Loop");

			ImGui::TableNextColumn();
			//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::Checkbox("", &loop);
			//ImGui::PopItemFlag();

			// Play / Pause the animation
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Play Animation");
			ImGui::TableNextColumn();
			if (ImGui::Button(toggleAnimation.c_str(), ImVec2(ImGui::GetColumnWidth() * 0.2f, 0.f)))
			{
				Animation* anim{ nullptr };
				if (GetCurrentAnimation())
					anim = GetCurrentAnimation();

				if (status == AnimatorStatus::idle)
					status = AnimatorStatus::playing;
				else
				{
					status = AnimatorStatus::idle;
					anim->ResetFrame();
				}

			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Number of Animations:");
			ImGui::TableNextColumn();
			ImGui::Text("%d", animationCount);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Button("Add Animation"))
			{
				AddAnimation();
			}

			// For each animation display appropriate things
			for (int i{ 0 }; i < animations.size(); ++i)
			{
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Animation %d", i + 1);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Number of Frames:");
				ImGui::TableNextColumn();
				if (ImGui::DragInt("", &animations[i].frameCount, 1))
				{
					animations[i].frameCount = animations[i].frameCount < 0 ? 0 : animations[i].frameCount;
				}
				ImGui::PopID();

				ImGui::PushID(i + 1);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Time Delay:");
				ImGui::TableNextColumn();
				if (ImGui::DragFloat("", &animations[i].timeDelay, 0.1f))
				{
					animations[i].timeDelay = animations[i].timeDelay < 0.f ? 0.f : animations[i].timeDelay;

				}
				ImGui::PopID();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Sprite");
				ImGui::TableNextColumn();
				ImGui::Button(animations[i].spriteSheet.name.c_str(), ImVec2(-FLT_MIN, 0.f));
				if (ImGui::BeginDragDropTarget())
				{
					unsigned int spriteID{0};
					std::string spriteName;
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
					{
						std::string str = (const char*)(payload->Data);
						AssetsSystem* assets = AssetsSystem::Instance();
						for (int j = 0; j < assets->get_textures().size(); j++)
						{
							if (!assets->get_texture(j)->get_file_path().compare(str))
							{
								spriteID = j + 1;
							}
						}
						size_t pos = str.find_last_of('/');
						spriteName = str.substr(pos + 1, str.length() - pos);
					}
					ImGui::EndDragDropTarget();

					if (spriteID >= 0)
					{
						animations[i].spriteSheet.spriteID = spriteID;
						animations[i].spriteSheet.name = spriteName;
					}
				}

			}
		

			ImGui::Unindent();
			ImGui::EndTable();
		}
	}
	void Animator::AddAnimation()
	{
		if (animationCount == MAX_ANIMATION_COUNT)
		{
			PRINT("Maximum number of animations for this Animator has been reached!");
			return;
		}

		animations.push_back(Animation());
		++animationCount;

		currentAnimationIndex = animations.size() - 1;

	}
	void Animator::PlayAnimation()
	{

	}
	void Animator::Update(float _dt)
	{
		//for (Animation& anim : animations)
		//{
		//	if (anim.UpdateFrame(_dt))
		//		anim.IncrementFrame();

		//}

		if (!animationCount)
			return;

		if (status != AnimatorStatus::playing)
			return;
		//PRINT("Animator update\n");
		if (animations[currentAnimationIndex].UpdateFrame(_dt))
			animations[currentAnimationIndex].IncrementFrame();


	}
	void Animator::deserialize(rapidjson::Value& _value)
	{
		Component::deserialize(_value);
		if (_value.HasMember("Loop"))
			loop = _value["Loop"].GetBool();
		else
			loop = true;

		if (_value.HasMember("Count"))
			animationCount = _value["Count"].GetUint();

		if (_value.HasMember("Animations"))
		{
			rapidjson::Value& animArray = _value["Animations"].GetArray();
			int i{ 0 };
			for (rapidjson::Value::ValueIterator iter = animArray.Begin(); iter != animArray.End(); ++iter)
			{
				
				rapidjson::Value& val = *iter;
				std::string animName;
				if (val.HasMember("Name"))
					animName = val["Name"].GetString();

				unsigned int sid{ 0 };
				if (val.HasMember("Sprite ID"))
					sid = val["Sprite ID"].GetUint();

				float td{ 0.f };
				if (val.HasMember("Time Delay"))
					td = val["Time Delay"].GetFloat();

				int fc{ 0 };
				if (val.HasMember("FrameCount"))
					fc = val["FrameCount"].GetInt();

				animations.push_back(Animation());
				animations[i].timeDelay = td;
				animations[i].frameCount = fc;
				animations[i].spriteSheet.spriteID = sid;
				animations[i].spriteSheet.name = animName;

				++i;
			}

		}

		currentAnimationIndex = 0;
		status = AnimatorStatus::idle;


	}
	void Animator::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Animator Serialization Start\n");
		Component::serialize(_value, _doc);
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), (rapidjson::SizeType)tc.length(), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		_value.AddMember("Loop", loop, _doc.GetAllocator());
		_value.AddMember("Count", animationCount, _doc.GetAllocator());


		rapidjson::Value anims(rapidjson::kArrayType);
		for (Animation& a : animations)
		{
			rapidjson::Value anim(rapidjson::kObjectType);
			rapidjson::Value name;
			name.SetString(a.spriteSheet.name.c_str(), 
							(rapidjson::SizeType)a.spriteSheet.name.length(), _doc.GetAllocator());
			anim.AddMember("Name", name, _doc.GetAllocator());
			anim.AddMember("Sprite ID", a.spriteSheet.spriteID, _doc.GetAllocator());
			anim.AddMember("Time Delay", a.timeDelay, _doc.GetAllocator());
			anim.AddMember("FrameCount", a.frameCount, _doc.GetAllocator());
			

			anims.PushBack(anim, _doc.GetAllocator());
		}
		_value.AddMember("Animations", anims, _doc.GetAllocator());

		PRINT("Animator Serialization End\n");

	}

	void AnimationSystem::init()
	{
		PRINT("anim sys init");
		systemFlags |= FLAG_RUN_ON_PLAY | FLAG_RUN_ON_EDITOR;
	}
	void AnimationSystem::update()
	{

		if (!sm->get_current_scene())
			return;

		//PRINT("Animation system update");


		for (Copium::GameObject* go : sm->get_current_scene()->gameObjects)
		{
			for (Component* component : go->getComponents<Animator>()) {

				Animator* anim = reinterpret_cast<Animator*>(component);
				anim->Update(MyFrameRateController.getDt());

			}

		}
	}
	void AnimationSystem::exit()
	{
		for (Animator* anim : animators)
		{
			if (anim->get_animation_vector().empty())
				continue;



		}
	}
}