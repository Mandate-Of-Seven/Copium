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
#include <Editor/editor-system.h>
#include <Events/events-system.h>
#include <imgui/include/imgui.h>

// Bean: Remove once we can auto select gameobjects

#include "Files/file-system.h"


#define BUTTON_HEIGHT .1 //Percent
#define BUTTON_WIDTH .6 //Percent
#define INPUT_BUFFER_SIZE 128

namespace Window
{
    //Editor
    namespace
    {
        using namespace Copium;

        template <typename T>
        void Display(const char* name, T& val);

        template <typename T>
        void DisplayPointer(T& container)
        {
            PRINT(typeid(T).name());
            static std::string buttonName = std::string("(") + (typeid(T).name()+ strlen("class Copium::")) +")";
            ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
        }

        template <>
        void DisplayPointer(const Texture& container)
        {
            static std::string buttonName{};
            const std::string& filePath{ container.get_file_path() };
            size_t offset = filePath.find_last_of("/");
            if (offset == std::string::npos)
            {
                buttonName = (filePath.c_str());
            }
            else
            {
                buttonName = (filePath.c_str() + offset + 1);
            }
            buttonName += '(';
            buttonName += typeid(Texture).name() + strlen("class Copium::");
            buttonName += ')';
            ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
        }

        template <typename T>
        void DisplayType(const char* name, T*& container)
        {
            static std::string buttonName{};
            if (container == nullptr)
            {
                buttonName = "Empty";
                buttonName += '(';
                buttonName += typeid(T).name() + strlen("class Copium::");
                buttonName += ')';
                ImGui::Button("Empty", ImVec2(-FLT_MIN, 0.f));
            }
            else
            {
                DisplayPointer(*container);
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(typeid(T).name());
                if (payload)
                {
                    PRINT("SETTED");
                    container = (T*)(*reinterpret_cast<void**>(payload->Data));
                }
                ImGui::EndDragDropTarget();
            }
        }

        void DisplayType(const char* name,bool& val)
        {
            static std::string idName{};
            idName = "##";
            idName += name;
            ImGui::Checkbox(idName.c_str(), &val);
        }

        void DisplayType(const char* name,float& val)
        {
            static float temp{};
            static std::string idName{};
            const char* cIdName{};
            if (name[0] == '#' && name[1] == '#')
            {
                cIdName = name;
            }
            else
            {
                idName = "##";
                idName += name;
                cIdName = idName.c_str();
            }
            ImGui::DragFloat(cIdName, &val, 0.15f);
            if (ImGui::IsItemActivated()) { temp = val; }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != val)
                {
                    //UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&position.x, temp);
                    //EditorSystem::Instance().get_commandmanager()->undoStack.push(tempUndo);
                    temp = val;
                }
            }
        }

        void DisplayType(const char* name, Math::Vec3& val)
        {
            static ImGuiTableFlags windowFlags =
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_NoBordersInBody |
                ImGuiTableFlags_NoSavedSettings |
                ImGuiTableFlags_SizingStretchProp;


            static float temp{};
            static std::string idName{};
            idName = "##";
            idName += name;
            if (ImGui::BeginTable("Vector3", 3, windowFlags))
            {
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                idName += 'X';
                ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                DisplayType(idName.c_str(), val.x);

                ImGui::TableNextColumn();
                idName.back() = 'Y';
                ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                DisplayType(idName.c_str(), val.y);

                ImGui::TableNextColumn();
                idName.back() = 'Z';
                ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                DisplayType(idName.c_str(), val.z);
                ImGui::EndTable();
            }
        }

        void DisplayType(const char* name,Math::Vec2& val)
        {
            static ImGuiTableFlags windowFlags =
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_NoBordersInBody |
                ImGuiTableFlags_NoSavedSettings |
                ImGuiTableFlags_SizingStretchProp;


            static float temp{};
            static std::string idName{};
            idName = "##";
            idName += name;
            if (ImGui::BeginTable("Vector2", 2, windowFlags))
            {
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                idName += 'X';
                ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                DisplayType(idName.c_str(), val.x);

                ImGui::TableNextColumn();
                idName.back() = 'Y';
                ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                DisplayType(idName.c_str(), val.y);

                ImGui::EndTable();
            }
        }

        void DisplayType(const char* name, AABB& val)
        {
            static ImGuiTableFlags windowFlags =
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_NoBordersInBody |
                ImGuiTableFlags_NoSavedSettings |
                ImGuiTableFlags_SizingStretchProp;

            static std::string idName{};
            idName = "##";
            idName += name;

            if (ImGui::BeginTable("AABB", 2, windowFlags))
            {
                ImGui::AlignTextToFramePadding();
                Display("Max", val.max);
                Display("Min", val.min);
                ImGui::EndTable();
            }
        }


        template <typename T>
        void Display(const char* name, T& val)
        {
            //static std::string displayName{};
            //displayName = "##";
            //displayName += name;
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(name);
            ImGui::TableNextColumn();
            DisplayType(name,val);
        }


    }

    namespace
    {
        using namespace Copium;
        
        bool AddComponent(ImGuiTextFilter& filter, EntityID entityID) { (void)filter; (void)entityID; return false; }

        template <typename Component, typename... Components>
        bool AddComponent(ImGuiTextFilter& filter, EntityID entityID,Pack<Component, Components...> components)
        {
            static ImVec2 buttonSize = ImGui::GetWindowSize();
            buttonSize.y *= (float)BUTTON_HEIGHT;
            static const char* name = typeid(Component).name() + strlen("struct Copium::");
            if (filter.PassFilter(name) && ImGui::Button(name, buttonSize))
            {
                Component* component;
                MyEventSystem.publish(new AddComponentEvent{entityID,component});
                return true;
            }
            if constexpr (sizeof...(Components) == 0)
            {
                return false;
            }
            else
            {
                return AddComponent(filter,entityID,Pack<Components...>());
            }
        }

        void AddComponentPanel(EntityID entityID, bool& open)
        {
            if (open)
            {
                ImGui::Begin("Add Component", &open);
                //AlignForWidth(ImGui::GetWindowSize().x);
                static ImGuiTextFilter filter;
                ImGui::PushItemWidth(-1);
                filter.Draw("##ComponentName");
                ImGui::PopItemWidth();
                open = !AddComponent(filter,entityID,AllComponents::Types());

                /*for (auto& nameToScriptClass : scriptingSystem.getScriptFiles())
                {
                    const std::string& name{ nameToScriptClass.filename().stem().string() };
                    if (filter.PassFilter(name.c_str()) && ImGui::Button(name.c_str(), buttonSize)) {
                        selectedGameObject->addComponent<Copium::Script>().Name(name);
                        isAddingComponent = false;
                    }
                }*/
                //static std::string newScriptPrompt;
                //newScriptPrompt.clear();
                //newScriptPrompt += "[New Script] ";
                //newScriptPrompt += filter.InputBuf;
                //if (ImGui::Button(newScriptPrompt.c_str(), buttonSize))
                //{
                //    //Ask scripting system query if file exists
                //    //scriptingSystem.addEmptyScript(filter.InputBuf);
                //    //selectedGameObject->addComponent<Copium::Script>().Name(filter.InputBuf);
                //    isAddingComponent = false;
                //}
                ImGui::End();
            }
        }

        template <typename T>
        void DisplayComponent(T& component)
        {
            static_assert(AllComponents::Has<T>());
            PRINT("Component of type: " << typeid(T).name() << "does not exist yet! ");
        }

        template <>
        void DisplayComponent<Transform>(Transform& transform)
        {
            Display("Position", transform.position);
            Display("Rotation", transform.rotation);
            Display("Scale", transform.scale);
        }

        template <>
        void DisplayComponent<BoxCollider2D>(BoxCollider2D& boxCollider2D)
        {
            Display("Bounds", boxCollider2D.bounds);
        }

        template <>
        void DisplayComponent<SpriteRenderer>(SpriteRenderer& spriteRenderer)
        {

            //DisplayDragDrop();
            //spriteRenderer.sprite.set_name()
            Display("Sprite", spriteRenderer.refTexture);
        }

        void DisplayComponents(EntityID id)
        {
            (void)id;
        }

        template <size_t ID = 0, typename Component, typename... Components >
        void DisplayComponents(EntityID id, Pack<Component, Components...> components)
        {
            Component* component{};
            MyEventSystem.publish(new GetComponentEvent<Component>{ id,component });
            if (component)
            {
                ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;
                bool enabled{};
                MyEventSystem.publish(new GetComponentEnabledEvent<Component>{ id,enabled });
                ImGui::PushID(ID);
                DisplayType("Enabled", enabled); ImGui::SameLine();
                ImGui::PopID();
                MyEventSystem.publish(new SetComponentEnabledEvent<Component>{ id,enabled });
                const char* componentName = typeid(Component).name() + strlen("struct Copium::");
                if (ImGui::CollapsingHeader(componentName, nodeFlags))
                {
                    ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
                        | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp
                        | ImGuiTableFlags_PadOuterX;
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0));
                    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 2));
                    if (ImGui::BeginTable("Component", 2, windowFlags))
                    {
                        ImGui::Indent();
                        ImGui::TableSetupColumn("Text", 0, 0.4f);
                        ImGui::TableSetupColumn("Input", 0, 0.6f);
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
                        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 0));
                        if (ImGui::BeginDragDropSource())
                        {
                            static void* container;
                            container = component;
                            ImGui::SetDragDropPayload(componentName, &container, sizeof(void*));
                            ImGui::EndDragDropSource();
                        }
                        DisplayComponent(*component);
                        ImGui::PopStyleVar();
                        ImGui::PopStyleVar();
                        ImGui::PopStyleVar();
                        ImGui::Unindent();
                        ImGui::EndTable();
                    }
                    if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x, 0.f)))
                    {
                        MyEventSystem.publish(new RemoveComponentEvent<Component>{ id });
                        //PRINT("ID: " << component->id);
                        //componentsToDelete.push_back(component->id);
                        //SceneManager::Instance()->get_current_scene()->add_unused_cid(component->id);
                    }
                    ImGui::PopStyleVar();
                    ImGui::PopStyleVar();
                }
            }
            if constexpr (sizeof...(Components) == 0)
            {
                return;
            }
            else
            {
                DisplayComponents<ID+1>(id,Pack<Components...>());
            }
        }

        void DisplayEntity(EntityID entityID)
        {
            using namespace Copium;
            Entity* selectedEntity{};
            MyEventSystem.publish(new GetEntityEvent{ MyEditorSystem.GetSelectedEntityID(),selectedEntity });
            if (!selectedEntity)
                return;
            bool active{};
            MyEventSystem.publish(new GetEntityActiveEvent{ entityID, active });
            ImGui::Checkbox("##Active", &active);
            ImGui::SameLine();
            static char buffer[256];
            strcpy(buffer, selectedEntity->name.c_str());
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##gameObjName", buffer, 256);
            ImGui::PopItemWidth();
            selectedEntity->name = buffer;
            ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH
                | ImGuiTableFlags_ScrollY;
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
            if (ImGui::BeginTable("Components", 1, tableFlags, ImVec2(0.f, ImGui::GetWindowSize().y * 0.8f)))
            {
                ImGui::TableNextColumn();
                int index = 0;
                ImGui::PushID(index++);
                DisplayComponents(entityID,AllComponents::Types());
                ImGui::PopID();
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();

            MyEventSystem.publish(new SetEntityActiveEvent{ entityID, active });

            static const float buttonSizeY = ImGui::CalcTextSize("Add Component").y;
            ImVec2 buttonSize(ImGui::GetWindowSize().x, buttonSizeY * 2);;
            ImGui::SetCursorPosY(ImGui::GetWindowSize().y - buttonSize.y);

            static bool isAddingComponent = false;

            if (ImGui::Button("Add Component", buttonSize)) {
                isAddingComponent = true;
            }

            AddComponentPanel(entityID, isAddingComponent);
        }

    }

	namespace Inspector
	{
        char nameBuffer[INPUT_BUFFER_SIZE];
        Copium::FileSystem& fileSystem{ Copium::FileSystem::Instance() };

        void AlignForWidth(float width, float alignment = 0.5f)
        {
            //ImGuiStyle& style = ImGui::GetStyle();
            float avail = ImGui::GetContentRegionAvail().x;
            float off = (avail - width) * alignment;
            if (off > 0.0f)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        }

        void Init()
        {
            isInspectorOpen = true;

            for (size_t i{ 0 }; i < 128; ++i)
            {
                nameBuffer[i] = '\0';
            }
        }

        void Update()
        {
            using namespace Copium;
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


            Copium::File* selectedFile = fileSystem.get_selected_file();
            Copium::Directory* selectedDirectory = fileSystem.get_selected_directory();
            DisplayEntity(MyEditorSystem.GetSelectedEntityID());
            //if (selectedEntity && !selectedFile)
            //{
            //    // Set flags for tables
            //    selectedGameObject->inspectorView();
            //    // Bean: This should be in the selected gameobject inspector view along the isAddingComponent part
            //AlignForWidth(buttonSize.x);
            //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
            //ImGui::PopStyleVar();
            //}
            //else if (selectedFile)
            //{
            //    selectedFile->inspector_view();
            //}
            //else if (selectedDirectory)
            //{
            //    selectedDirectory->inspector_view();
            //}
            //
            ImGui::PopStyleVar();
            ImGui::End();
            

        }

        void Exit()
        {
        }
	}
}