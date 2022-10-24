/*!***************************************************************************************
\file			inspector.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Defines Window::Inspector namespace with ImGui functions for displaying and
    interacting with the window.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Editor/inspector.h"

// Bean: Remove once we can auto select gameobjects
#include "SceneManager/sm.h"


#define BUTTON_HEIGHT 0.05 //Percent
#define BUTTON_WIDTH .6 //Percent

namespace Window
{

	namespace Inspector
	{
        bool isOpen;
        GameObject* selectedGameObject;
        bool isAddingComponent = false;

        void AlignForWidth(float width, float alignment = 0.5f)
        {
            //ImGuiStyle& style = ImGui::GetStyle();
            float avail = ImGui::GetContentRegionAvail().x;
            float off = (avail - width) * alignment;
            if (off > 0.0f)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        }

        void init()
        {
            Copium::NewSceneManager* sm = Copium::NewSceneManager::Instance();
            selectedGameObject = sm->get_current_scene()->get_gameobjectvector()[0];
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("assets\\fonts\\bahnschrift.ttf", 32.f);
            isOpen = true;
        }


        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		void update()
		{
            if (!isOpen)
                return;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
            ImGui::SetNextWindowSizeConstraints(ImVec2(320, 180), ImVec2(FLT_MAX, FLT_MAX));
            if (!ImGui::Begin("Inspector", &isOpen)) 
            {
                ImGui::End();
                return;
            }
            if (selectedGameObject)
            {
                // Set flags for tables
                ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH 
                    | ImGuiTableFlags_ScrollY;
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);

                // A row represent a component
                if (ImGui::BeginTable("Components", 1, tableFlags, ImVec2(0.f, 450.f)))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
                    for (Component *component : selectedGameObject->Components())
                    {
                        if (ImGui::CollapsingHeader(component->Name().c_str(), nodeFlags))
                        {
                            component->inspector_view(*selectedGameObject);
                        }
                        ImGui::TableNextColumn();
                    }
                    ImGui::EndTable();
                }
                ImGui::PopStyleVar();

                //AlignForWidth(buttonSize.x);
                //if (ImGui::Button("Add Component", buttonSize)) {
                //    isAddingComponent = true;
                //}
            }
            ImGui::End();
            ImGui::PopStyleVar();

            if (isAddingComponent)
            {
                ImGui::Begin("Add Component",&isAddingComponent);
                AlignForWidth(ImGui::GetWindowSize().x);
                char buff[7]{' '};
                ImGui::PushItemWidth(-1);
                ImGui::InputText("Search", buff,7);
                ImGui::PopItemWidth();
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= (float) BUTTON_HEIGHT;
                std::map<Component::Type, const std::string>::iterator it;
                for (it = Component::componentMap.begin(); it != Component::componentMap.end(); ++it)
                {
                    if (ImGui::Button(it->second.c_str(), buttonSize)) {
                        selectedGameObject->addComponent(it->first);
                    }
                }
                ImGui::End();
            }

            // For each component in gameobject, show in inspector

		}

        void exit()
        {
            selectedGameObject = nullptr;
        }
	}
}