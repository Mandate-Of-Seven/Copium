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
#include "Editor/editor-system.h"

namespace Copium
{
    namespace
    {
        EditorSystem* editor = EditorSystem::Instance();
    }

	SortingGroup::SortingGroup(GameObject& _gameObj) :Component(_gameObj, ComponentType::SortingGroup), sortingLayer{0}, orderInLayer{0}
	{
        PRINT("Added to default layer");
        editor->getLayers()->SortLayers()->AddGameObject(0, _gameObj);
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
            SortingLayers& editorSortingLayer = *editor->getLayers()->SortLayers();
            std::vector<Layer>& sortingLayers = editorSortingLayer.GetSortingLayers();


            const char* previewItem{nullptr};
            for (Layer& lay : sortingLayers)
            {
                if (lay.layerID == sortingLayer)
                {
                    previewItem = lay.name.c_str();
                    break;
                }
            }


            if (!previewItem)
                previewItem = "NULL";

            if (ImGui::BeginCombo("##LayerSelection", previewItem))
            {
                for (int i = 0; i < editorSortingLayer.GetSortingLayers().size(); i++)
                {
                    const bool isSelected = (sortingLayer == i);

                    ImGuiSelectableFlags flags = ImGuiSelectableFlags_AllowItemOverlap;
                    char* name = sortingLayers[i].name.data();
                    int id = sortingLayers[i].layerID + 1;
                    std::string label = "##" + std::to_string(id);
                    if (ImGui::Selectable(label.c_str(), isSelected))
                    {
                        if (i != sortingLayer)
                        {
                            editor->getLayers()->SortLayers()->RemoveGameObject(sortingLayer, gameObj);
                            editor->getLayers()->SortLayers()->AddGameObject(i, gameObj);
                        }

                        sortingLayer = i;

                    }
                    ImGui::SameLine();
                    ImGui::Text(sortingLayers[i].name.c_str());

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
            if (ImGui::DragInt("", &orderInLayer, 1.f, 0, 100))
            {
                // Sort the layer based on all order ids
                // Michael Buble sort here
                Layer* layer = &editor->getLayers()->SortLayers()->GetSortingLayers()[sortingLayer];
                if (layer)
                {
                    bool swapped{ false };
                    for (size_t i{ 0 }; i < layer->gameObjects.size() - 1; ++i)
                    {
                        for (size_t j{ 0 }; j < layer->gameObjects.size() - 1 - i; ++j)
                        {
                            SortingGroup* sg1{ nullptr }, * sg2{ nullptr };

                            if (!layer->gameObjects[j] && layer->gameObjects[j + 1])
                            {
                                std::swap(layer->gameObjects[j], layer->gameObjects[j + 1]);
                                swapped = true;
                                continue;
                            }

                            if (layer->gameObjects[j] && layer->gameObjects[j + 1])
                            {
                                Component* co1 = layer->gameObjects[j]->getComponent<SortingGroup>();
                                Component* co2 = layer->gameObjects[j + 1]->getComponent<SortingGroup>();

                                if (co1 && co2)
                                {
                                    sg1 = reinterpret_cast<SortingGroup*>(co1);
                                    sg2 = reinterpret_cast<SortingGroup*>(co2);

                                    if (sg1->GetOrderInLayer() > sg2->GetOrderInLayer())
                                    {
                                        std::swap(layer->gameObjects[j], layer->gameObjects[j + 1]);
                                        swapped = true;
                                    }
                                }
                            }

                        }

                        if (!swapped)
                            break;
                    }
                }
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
}