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

namespace Copium
{
	Animator::Animator(GameObject& _gameObj) : Component(_gameObj, ComponentType::Animator)
	{

	}

	void Animator::inspector_view()
	{
		std::string toggleAnimation = "Play";
		bool loop = false;

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
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::Checkbox("", &loop);
			ImGui::PopItemFlag();

			// Play / Pause the animation
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Play Animation");
			ImGui::TableNextColumn();
			ImGui::Button(toggleAnimation.c_str(), ImVec2(ImGui::GetColumnWidth() * 0.2f, 0.f)); // Change toggleAnimation

			ImGui::Unindent();
			ImGui::EndTable();
		}
	}

	void AnimationSystem::Update()
	{
		for (Animator* anim : animators)
		{

		}
	}
}