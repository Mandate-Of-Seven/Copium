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

#include "inspector.h"


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
            ImGuiStyle& style = ImGui::GetStyle();
            float avail = ImGui::GetContentRegionAvail().x;
            float off = (avail - width) * alignment;
            if (off > 0.0f)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        }

        void init()
        {
            selectedGameObject = nullptr;
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("assets\\fonts\\bahnschrift.ttf", 32.f);
            isOpen = true;
        }


        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		void update()
		{
            if (!isOpen)
                return;

            

            if (!ImGui::Begin("Inspector", &isOpen)) 
            {

                ImGui::End();
                return;
            }
            if (selectedGameObject)
            {
                Transform trans{ selectedGameObject->Trans() };
                Vector3 position = trans.Position();
                Vector3 rotation = trans.Rotation();
                Vector3 scale = trans.Scale();
                if (ImGui::CollapsingHeader("Transform"))
                {
                    if (ImGui::BeginTable("split", 4))
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("Position");

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("X"); ImGui::SameLine();
                        ImGui::InputDouble("posx", &position.x);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::InputDouble("posy", &position.y);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("Z"); ImGui::SameLine();
                        ImGui::InputDouble("posz", &position.z);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::Text("Rotation");

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("X"); ImGui::SameLine();
                        ImGui::InputDouble("rotx", &rotation.x);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::InputDouble("roty", &rotation.y);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("Z"); ImGui::SameLine();
                        ImGui::InputDouble("rotz", &rotation.z);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::Text("Scale");

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("X"); ImGui::SameLine();
                        ImGui::InputDouble("scalex", &scale.x);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::InputDouble("scaley", &scale.y);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("Z"); ImGui::SameLine();
                        ImGui::InputDouble("scalez", &scale.z);
                        ImGui::PopItemWidth();
                        ImGui::EndTable();
                    }
                }
                selectedGameObject->Trans({position, rotation, scale});
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.x *= BUTTON_WIDTH;
                buttonSize.y *= BUTTON_HEIGHT;

                for (Component *component : selectedGameObject->Components())
                {
                    if (ImGui::CollapsingHeader(component->Name().c_str()))
                    {
                        
                    }
                }

                AlignForWidth(buttonSize.x);
                if (ImGui::Button("Add Component", buttonSize)) {
                    isAddingComponent = true;
                }
            }
            ImGui::End();

            if (isAddingComponent)
            {
                ImGui::Begin("Add Component",&isAddingComponent);
                AlignForWidth(ImGui::GetWindowSize().x);
                char buff[7]{' '};
                ImGui::PushItemWidth(-1);
                ImGui::InputText("Search", buff,7);
                ImGui::PopItemWidth();
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= BUTTON_HEIGHT;
                std::map<Component::Type, const std::string>::iterator it;
                for (it = Component::componentMap.begin(); it != Component::componentMap.end(); it++)
                {
                    if (ImGui::Button(it->second.c_str(), buttonSize)) {
                        selectedGameObject->addComponent(it->first);
                    }
                }

                ImGui::End();
            }
		}
	}
}