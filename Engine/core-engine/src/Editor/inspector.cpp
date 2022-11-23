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
#include "GameObject/Components/script-component.h"
#include "Editor/editor-hierarchy-list.h"

// Bean: Remove once we can auto select gameobjects
#include "SceneManager/sm.h"
#include "Files/file-system.h"


#define BUTTON_HEIGHT .1 //Percent
#define BUTTON_WIDTH .6 //Percent
#define INPUT_BUFFER_SIZE 128

namespace Window
{

	namespace Inspector
	{
        bool isAddingComponent;
        char nameBuffer[INPUT_BUFFER_SIZE];
        Copium::ScriptingSystem& scriptingSystem{ *Copium::ScriptingSystem::Instance() };
        Copium::NewSceneManager& sceneManager{ *Copium::NewSceneManager::Instance() };
        Copium::FileSystem& fileSystem{ *Copium::FileSystem::Instance() };

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
            isAddingComponent = false;
            isInspectorOpen = true;

            for (size_t i{ 0 }; i < 128; ++i)
            {
                nameBuffer[i] = '\0';
            }
        }

        void update()
        {
            if (!isInspectorOpen)
                return;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
            ImGui::SetNextWindowSizeConstraints(ImVec2(320, 180), ImVec2(FLT_MAX, FLT_MAX));

            if (!ImGui::Begin("Inspector", &isInspectorOpen))
            {
                ImGui::End();
                ImGui::PopStyleVar();
                return;
            }

            Copium::GameObject* selectedGameObject = sceneManager.selectedGameObject;
            Copium::File* selectedFile = fileSystem.get_selected_file();
            if (selectedGameObject && !selectedFile)
            {
                // Set flags for tables
                selectedGameObject->inspectorView();

                //AlignForWidth(buttonSize.x);
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= (float)BUTTON_HEIGHT;
                if (ImGui::Button("Add Component", buttonSize)) {
                    isAddingComponent = true;
                }
            }
            else if (selectedFile)
            {
                selectedFile->inspector_view();
            }
            ImGui::End();
            ImGui::PopStyleVar();
            if (isAddingComponent)
            {
                ImGui::Begin("Add Component", &isAddingComponent);
                AlignForWidth(ImGui::GetWindowSize().x);
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= (float)BUTTON_HEIGHT;
                static ImGuiTextFilter filter;
                ImGui::PushItemWidth(-1);
                filter.Draw("##ComponentName");
                ImGui::PopItemWidth();
                std::map<Copium::ComponentType, std::string>::iterator it;
                for (it = Copium::MAP_COMPONENT_TYPE_NAME.begin();
                    it != Copium::MAP_COMPONENT_TYPE_NAME.end(); ++it)
                {
                    if (it->first == Copium::ComponentType::Script)
                        continue;
                    const std::string& componentName{ it->second };
                    if (filter.PassFilter(componentName.c_str()) && ImGui::Button(componentName.c_str(), buttonSize))
                    {
                        selectedGameObject->addComponent(it->first);
                        isAddingComponent = false;
                        break;
                    }
                }
                for (auto& nameToScriptClass : scriptingSystem.getScriptFiles())
                {
                    const std::string& name{ nameToScriptClass.filename().stem().string() };
                    if (filter.PassFilter(name.c_str()) && ImGui::Button(name.c_str(), buttonSize)) {
                        selectedGameObject->addComponent<Copium::Script>().Name(name);
                        isAddingComponent = false;
                    }
                }
                static std::string newScriptPrompt;
                newScriptPrompt.clear();
                newScriptPrompt += "[New Script] ";
                newScriptPrompt += filter.InputBuf;
                if(ImGui::Button(newScriptPrompt.c_str(), buttonSize)) 
                {
                    //Ask scripting system query if file exists
                    scriptingSystem.addEmptyScript(filter.InputBuf);
                    selectedGameObject->addComponent<Copium::Script>().Name(filter.InputBuf);
                    isAddingComponent = false;
                }
                ImGui::End();
            }
        }

        void exit()
        {
        }
	}
}