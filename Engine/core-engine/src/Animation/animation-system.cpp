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

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Animation/animation-system.h"
#include "Files/assets-system.h"
#define MAX_ANIMATION_COUNT 5

namespace Copium
{

	Animator::Animator(GameObject& _gameObj) 
		: Component(_gameObj, ComponentType::Animator), currentAnimationIndex{0},startingAnimationIndex{0}, animationCount{0}, loop{false}, status{AnimatorStatus::idle}
	{

	}

	void Animator::inspector_view()
	{
		std::string toggleAnimation = "Play";

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
			ImGui::Button(toggleAnimation.c_str(), ImVec2(ImGui::GetColumnWidth() * 0.2f, 0.f)); // Change toggleAnimation

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
				ImGui::DragInt("", &animations[i].frameCount, 1);
				ImGui::PopID();

				ImGui::PushID(i + 1);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Time Delay:");
				ImGui::TableNextColumn();
				ImGui::DragFloat("", &animations[i].timeDelay, 0.1f);
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

	void Animator::update(float _dt)
	{
		for (Animation& anim : animations)
		{
			if (anim.UpdateFrame(_dt))
				anim.IncrementFrame();

		}
	}

	void AnimationSystem::Update()
	{
		for (Animator* anim : animators)
		{
			if (anim->get_animation_vector().empty())
				continue;



		}
	}
}