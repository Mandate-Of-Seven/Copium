/*!***************************************************************************************
\file			sorting-group-component.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			13/01/2023

\brief
	This file holds the definition of the sorting group component.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "GameObject/Components/sorting-group-component.h"

namespace Copium
{
	SortingGroup::SortingGroup(GameObject& _gameObj) :Component(_gameObj, ComponentType::SortingGroup), sortingLayer{0}, orderInLayer{0}
	{
	}

	void SortingGroup::inspector_view()
	{
        ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp
            | ImGuiTableFlags_PadOuterX;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 2));
        if (ImGui::BeginTable("Component Sorting Group", 2, windowFlags))
        {
            windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
                | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;

            ImGui::Indent();

            ImGui::TableSetupColumn("Text", 0, 0.4f);
            ImGui::TableSetupColumn("Input", 0, 0.6f);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 0));

            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Sorting Layer");
            ImGui::TableNextColumn();

            // Bean: This is temporary and should be referenced from the sorting layer editor
            std::string items[] = { "Default", "Player", "UI", "Background" };
            const char* previewItem = items[sortingLayer].c_str();
            if (ImGui::BeginCombo(" ", previewItem))
            {
                for (int i = 0; i < IM_ARRAYSIZE(items); i++)
                {
                    const bool isSelected = (sortingLayer == i);
                    if (ImGui::Selectable(items[i].c_str(), isSelected))
                        sortingLayer = i;

                    // Bean: Change the gameobjects current layer
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Order In Layer");
            ImGui::TableNextColumn();
            ImGui::DragInt("", &orderInLayer, 1.f, 0, 100);

            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            ImGui::Unindent();
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();

	}
}