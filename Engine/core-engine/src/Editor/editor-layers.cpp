/*!***************************************************************************************
\file			editor-layers.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			13/01/2023

\brief
	This file holds the definition of the functions in the editor layers gui

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Editor/editor-layers.h"

namespace Copium
{
	void EditorLayers::init()
	{

	}

	void EditorLayers::update()
	{
        
        ImGui::Begin("Tags and Layers");
			
        if (ImGui::CollapsingHeader("Tags"))
		{

		}

		if (ImGui::CollapsingHeader("Sorting Layers"))
		{
            ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
                | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp
                | ImGuiTableFlags_PadOuterX;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 2));

            // Bean: Add a add and remove layer button

			if (ImGui::BeginTable("Table: Sorting Layers", 2, tableFlags))
			{
                ImGui::Indent();

                ImGui::TableSetupColumn("Text", 0, 0.7f);
                ImGui::TableSetupColumn("Input", 0, 0.3f);

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 0));

                for (int i = 0; i < sortingLayers.GetLayerCount(); i++)
                {
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Layer");
                    ImGui::TableNextColumn();

                    char* name = sortingLayers.GetSortingLayers()[i].name.data();
                    size_t size = sortingLayers.GetSortingLayers()[i].name.size();
                        
                    ImGuiInputTextFlags textFlag = 0;
                    if (!sortingLayers.GetSortingLayers()[i].name.compare("Default"))
                        textFlag |= ImGuiInputTextFlags_ReadOnly;
                        
                    ImGui::InputText("", name, size, textFlag);
                        
                }

                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::Unindent();
				ImGui::EndTable();
			}

            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
		}

		if (ImGui::CollapsingHeader("Layers"))
		{

		}

		ImGui::End();
	}

	void EditorLayers::exit()
	{

	}
}