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


#define BUTTON_HEIGHT .1 //Percent
#define BUTTON_WIDTH .6 //Percent
#define MAX_NAME_LENGTH 128;

using Vector3 = glm::dvec3;

namespace Window
{

	namespace Inspector
	{
        

        bool isOpen;
        GameObject* selectedGameObject;
        bool isAddingComponent = false;
        char nameBuffer[128];

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
            selectedGameObject = nullptr;
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("assets\\fonts\\bahnschrift.ttf", 32.f);
            isOpen = true;

            for (size_t i{ 0 }; i < 128; ++i)
            {
                nameBuffer[i] = '\0';
            }
        }

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		void update()
		{
            if (!isOpen)
                return;

            if (Copium::NewSceneManager::Instance() != nullptr)
                if (selectedGameObject != Copium::NewSceneManager::Instance()->get_selected_gameobject())
                    selectedGameObject = Copium::NewSceneManager::Instance()->selectedGameObject;

            if (!ImGui::Begin("Inspector", &isOpen)) 
            {

                ImGui::End();
                return;
            }
            if (selectedGameObject)
            {

                // Print Name
                ImGui::Text("Name: %s", selectedGameObject->get_name().c_str());

                if (selectedGameObject->Trans())
                {
                    Transform trans{ selectedGameObject->Trans()->get_transform()};
                    Vector3 position = trans.get_position().to_glm();
                    Vector3 rotation = trans.get_rotation().to_glm();
                    Vector3 scale = trans.get_scale().to_glm();
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
                }

                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.x *= (float) BUTTON_WIDTH;
                buttonSize.y *= (float) BUTTON_HEIGHT;

                //for (Component *component : selectedGameObject->Components())
                //{
                //    if (ImGui::CollapsingHeader(component->Name().c_str()))
                //    {
                //        
                //    }
                //}

                AlignForWidth(buttonSize.x);
                if (ImGui::Button("Add Component", buttonSize)) {
                    isAddingComponent = true;
                }
            }
            ImGui::End();

            //if (isAddingComponent)
            //{
            //    ImGui::Begin("Add Component",&isAddingComponent);
            //    AlignForWidth(ImGui::GetWindowSize().x);
            //    char buff[7]{' '};
            //    ImGui::PushItemWidth(-1);
            //    ImGui::InputText("Search", buff,7);
            //    ImGui::PopItemWidth();
            //    ImVec2 buttonSize = ImGui::GetWindowSize();
            //    buttonSize.y *= (float) BUTTON_HEIGHT;
            //    std::map<Component::Type, const std::string>::iterator it;
            //    for (it = Component::componentMap.begin(); it != Component::componentMap.end(); ++it)
            //    {
            //        if (ImGui::Button(it->second.c_str(), buttonSize)) {
            //            selectedGameObject->addComponent(it->first);
            //        }
            //    }
            //    ImGui::End();
            //}
		}
	}
}