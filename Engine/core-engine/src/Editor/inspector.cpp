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

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Editor/inspector.h"
#include "Scripting/scripting.h"

// Bean: Remove once we can auto select gameobjects
#include "SceneManager/sm.h"


#define BUTTON_HEIGHT .1 //Percent
#define BUTTON_WIDTH .6 //Percent
#define MAX_NAME_LENGTH 128;

namespace Window
{

	namespace Inspector
	{
        bool isOpen;
        GameObject* selectedGameObject;
        bool isAddingScript;
        bool isAddingComponent;
        char nameBuffer[128];
        Copium::ScriptingSystem& scriptingSystem{ *Copium::ScriptingSystem::Instance() };

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
            isAddingScript = false;
            isAddingComponent = false;
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("assets\\fonts\\bahnschrift.ttf", 32.f);
            isOpen = true;

            for (size_t i{ 0 }; i < 128; ++i)
            {
                nameBuffer[i] = '\0';
            }
        }

		void update()
		{
            if (!isOpen)
                return;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
            ImGui::SetNextWindowSizeConstraints(ImVec2(320, 180), ImVec2(FLT_MAX, FLT_MAX));
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
                // Set flags for tables
                selectedGameObject->inspectorView();

                //AlignForWidth(buttonSize.x);
                if (ImGui::Button("Add Component", {100.f,100.f})) {
                    isAddingComponent = true;
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();

            if (isAddingComponent)
            {
                ImGui::Begin("Add Component",&isAddingComponent);
                AlignForWidth(ImGui::GetWindowSize().x);
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= (float) BUTTON_HEIGHT;
                std::map<ComponentType, const std::string>::iterator it;
                for (it = Component::componentMap.begin(); it != Component::componentMap.end(); ++it)
                {
                    if (ImGui::Button(it->second.c_str(), buttonSize)) {
                        if (it->first == ComponentType::Script)
                        {
                            isAddingScript = true;
                        }
                        else
                        {
                            selectedGameObject->addComponent(it->first);
                        }
                        isAddingComponent = false;
                        break;
                    }
                }
                ImGui::End();
            }

            if (isAddingScript)
            {
                ImGui::Begin("Add Script", &isAddingComponent);
                AlignForWidth(ImGui::GetWindowSize().x);
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= (float)BUTTON_HEIGHT;
                for (auto& nameToScriptClass : scriptingSystem.getScriptClassMap())
                {
                    const std::string& name { nameToScriptClass.first };
                    if (ImGui::Button(name.c_str(), buttonSize)) {
                        selectedGameObject->addComponent<Copium::ScriptComponent>().Name(name);
                    }
                }
                ImGui::End();
            }

		}

        void exit()
        {
            selectedGameObject = nullptr;
        }
	}
}