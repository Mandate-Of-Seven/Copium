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
#include "Editor/editor-hierarchy-list.h"
#include "Files/assets-system.h"
#include <Scripting/scripting-system.h>

// Bean: Remove once we can auto select gameobjects
#include "SceneManager/scene-manager.h"
#include "Files/file-system.h"
#include <GameObject/components.h>
#include "GameObject/Components/script-component.h"
#include <GameObject/Components/collider-components.h>
#include <GameObject/Components/camera-component.h>
#include <GameObject/Components/audiosource-component.h>
#include <Animation/animation-system.h>
#include <GameObject/Components/renderer-component.h>
#include <GameObject/Components/sorting-group-component.h>
#include <GameObject/Components/ui-components.h>
#include <GameObject/Components/physics-components.h>

#define BUTTON_HEIGHT .1 //Percent
#define BUTTON_WIDTH .6 //Percent
#define INPUT_BUFFER_SIZE 128

namespace Copium
{
    namespace
    {
        using namespace Copium;

        template <typename T>
        void Display(const char* name, T& val);

        template <typename T>
        void DisplayPointer(T& container)
        {
            PRINT(typeid(T).name());
            static std::string buttonName = std::string("(") + (typeid(T).name() + strlen("class Copium::")) + ")";
            ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
        }

        //template <>
        //void DisplayPointer(const Texture& container)
        //{
        //    static std::string buttonName{};
        //    const std::string& filePath{ container.get_file_path() };
        //    size_t offset = filePath.find_last_of("/");
        //    if (offset == std::string::npos)
        //    {
        //        buttonName = (filePath.c_str());
        //    }
        //    else
        //    {
        //        buttonName = (filePath.c_str() + offset + 1);
        //    }
        //    buttonName += '(';
        //    buttonName += typeid(Texture).name() + strlen("class Copium::");
        //    buttonName += ')';
        //    ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
        //}

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
                    container = (T*)(*reinterpret_cast<void**>(payload->Data));
                }
                ImGui::EndDragDropTarget();
            }
        }

        void DisplayType(const char* name, bool& val)
        {
            static std::string idName{};
            idName = "##";
            idName += name;
            ImGui::Checkbox(idName.c_str(), &val);
        }

        void DisplayType(const char* name, int& val)
        {
            static std::string idName{};
            idName = "##";
            idName += name;
            ImGui::DragInt(idName.c_str(), &val);
        }

        void DisplayType(const char* name, unsigned int& val)
        {
            static std::string idName{};
            idName = "##";
            idName += name;
            int container = val;
            ImGui::DragInt(idName.c_str(), &container, 1, 0);
            val = container;
        }

        void DisplayType(const char* name, float& val)
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

        void DisplayType(const char* name, Math::Vec2& val)
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
            DisplayType(name, val);
        }


    }

    namespace
    {
        using namespace Copium;

        template <typename Component, typename... Components>
        struct AddComponentStruct
        {
            constexpr AddComponentStruct(TemplatePack<Component,Components...> pack){}
            AddComponentStruct() {}

            static bool AddComponent(ImGuiTextFilter& filter, GameObject& gameObj) { (void)filter; (void)gameObj; return false; }

            template <typename T, typename... Ts>
            static bool AddComponent(ImGuiTextFilter& filter, GameObject& gameOb)
            {
                static ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= (float)BUTTON_HEIGHT;
                static const char* name = GetComponentType<T>::name;
                if (filter.PassFilter(name) && ImGui::Button(name, buttonSize))
                {
                    T* component;
                    MyEventSystem.publish(new AddComponentEvent{ gameObjID,component });
                    return true;
                }
                if constexpr (sizeof...(Ts) == 0)
                {
                    return false;
                }
                else
                {
                    return AddComponent<Ts...>(filter, gameObjID);
                }
            }
        };
        using AddAllComponentsStruct = decltype(AddComponentStruct(ComponentTypes()));
        bool AddComponent(ImGuiTextFilter& filter, GameObject& gameObj) { AddAllComponentsStruct::AddComponent(filter, gameObj); }

        void AddComponentPanel(GameObject& gameObj, bool& open)
        {
            if (open)
            {
                ImGui::Begin("Add Component", &open);
                //AlignForWidth(ImGui::GetWindowSize().x);
                static ImGuiTextFilter filter;
                ImGui::PushItemWidth(-1);
                filter.Draw("##ComponentName");
                ImGui::PopItemWidth();
                if (AddComponent(filter, gameObj))
                    open = false;

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

        template <>
        void DisplayComponent<Rigidbody2D>(Rigidbody2D& rb2D)
        {

            //DisplayDragDrop();
            //spriteRenderer.sprite.set_name()
            Display("Mass", rb2D.mass);
            Display("Use Gravity", rb2D.useGravity);
            Display("Is Kinematic", rb2D.isKinematic);
        }

        template <>
        void DisplayComponent<Animator>(Animator& animator)
        {

            //DisplayDragDrop();
            //spriteRenderer.sprite.set_name()
            Display("Loop", animator.loop);
            ImGui::TableNextColumn();
            ImGui::Text("Play Animation");
            std::string toggleAnimation = "Play";
            if (animator.status == Animator::AnimatorStatus::playing)
                toggleAnimation = "Stop";
            ImGui::TableNextColumn();
            if (ImGui::Button(toggleAnimation.c_str(), ImVec2(ImGui::GetColumnWidth() * 0.2f, 0.f)))
            {
                Animation* anim{ animator.GetCurrentAnimation() };

                if (animator.status == Animator::AnimatorStatus::idle)
                    animator.status = Animator::AnimatorStatus::playing;
                else
                {
                    animator.status = Animator::AnimatorStatus::idle;
                    anim->ResetFrame();
                }

            }

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Number of Animations:");
            ImGui::TableNextColumn();
            ImGui::Text("%d", animator.animationCount);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Button("Add Animation"))
            {
                animator.AddAnimation();
            }

            // For each animation display appropriate things
            for (int i{ 0 }; i < animator.animations.size(); ++i)
            {
                ImGui::PushID(i);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Animation %d", i + 1);

                ImGui::TableNextRow();
                Display("Number of Frames", animator.animations[i].frameCount);
                Display("Columns", animator.animations[i].spriteSheet.xColumns);
                Display("Rows", animator.animations[i].spriteSheet.yRows);
                ImGui::PopID();

                ImGui::PushID(i + 1);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Time Delay:");
                ImGui::TableNextColumn();
                if (ImGui::DragFloat("", &animator.animations[i].timeDelay, 0.1f))
                {
                    animator.animations[i].timeDelay = animator.animations[i].timeDelay < 0.f ? 0.f : animator.animations[i].timeDelay;

                }
                ImGui::PopID();

                Display("Sprite", animator.animations[i].spriteSheet.refTexture);
            }
        }

        template<typename Component, typename... Components>
        struct DisplayComponentsStruct
        {
        public:
            constexpr DisplayComponentsStruct(TemplatePack<Component, Components...> pack) {}
            DisplayComponentsStruct() = delete;
            DisplayComponentsStruct(GameObject& gameObj) { DisplayComponent(gameObj.transform); Display<Component, Components>(gameObj); }
        private:
            template<typename T, typename... Ts>
            void Display(GameObject& gameObj)
            {
                static_assert(std::is_base_of<Component,T>());
                ComponentsPtrArray<T>& components{gameObj.GetComponents<T>()};
                for (T* component: components)
                {
                    ImGui::PushID(component->uuid);
                    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;
                    DisplayType("Enabled", component->enabled); ImGui::SameLine();
                    if (ImGui::CollapsingHeader(GetComponentType(T)::name, nodeFlags))
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
                                container = &component;
                                ImGui::SetDragDropPayload(GetComponentType(T)::name, &container, sizeof(void*));
                                ImGui::EndDragDropSource();
                            }
                            DisplayComponent(component);
                            ImGui::PopStyleVar();
                            ImGui::PopStyleVar();
                            ImGui::PopStyleVar();
                            ImGui::Unindent();
                            ImGui::EndTable();
                        }
                        if constexpr (!std::is_same<Component, Transform>())
                        {
                            if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x, 0.f)))
                            {
                                PRINT("DELETING COMPONENT OF TYPE: " << typeid(Component).name());
                                //MyEventSystem.publish(new RemoveComponentEvent<Component>{ id });
                            }
                        }
                        ImGui::PopStyleVar();
                        ImGui::PopStyleVar();
                    }
                    ImGui::PopID();
                }
                if constexpr (sizeof...(Components) == 0)
                {
                    return;
                }
                else
                {
                    Display<Ts...>(gameObj);
                }
            }
        };
        using DisplayAllComponentsStruct = decltype(DisplayComponentsStruct(ComponentTypes()));
        void DisplayComponents(GameObject& gameObj){DisplayAllComponentsStruct obj{ gameObj };}

        void DisplayGameObject(GameObject& gameObject)
        {
            ImGui::Checkbox("##Active", &gameObject.active);
            ImGui::SameLine();
            static char buffer[256];
            strcpy(buffer, gameObject.name.c_str());
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##gameObjName", buffer, 256);
            ImGui::PopItemWidth();
            gameObject.name = buffer;
            ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH
                | ImGuiTableFlags_ScrollY;
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
            if (ImGui::BeginTable("Components", 1, tableFlags, ImVec2(0.f, ImGui::GetWindowSize().y * 0.8f)))
            {
                ImGui::TableNextColumn();
                ImGui::PushID(gameObject.uuid);
                DisplayComponents(gameObject);
                ImGui::PopID();
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
            static const float buttonSizeY = ImGui::CalcTextSize("Add Component").y;
            ImVec2 buttonSize(ImGui::GetWindowSize().x, buttonSizeY * 2);;
            ImGui::SetCursorPosY(ImGui::GetWindowSize().y - buttonSize.y);

            static bool isAddingComponent = false;

            if (ImGui::Button("Add Component", buttonSize)) {
                isAddingComponent = true;
            }

            AddComponentPanel(gameObject, isAddingComponent);
        }

    }
    namespace
    {
        //bool isInspectorOpen;
        bool isAddingComponent;
        Copium::SceneManager& sceneManager{ *Copium::SceneManager::Instance() };
        Copium::FileSystem& fileSystem{ *Copium::FileSystem::Instance() };
        Copium::AssetsSystem& assetsSystem{ *Copium::AssetsSystem::Instance() };
        char nameBuffer[INPUT_BUFFER_SIZE];
    }
    void EditorInspector::AlignforWidth(float width, float alignment)
    {
        //ImGuiStyle& style = ImGui::GetStyle();
        float avail = ImGui::GetContentRegionAvail().x;
        float off = (avail - width) * alignment;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    }
    void EditorInspector::init()
    {
        isAddingComponent = false;
        isInspectorOpen = true;

        for (size_t i{ 0 }; i < 128; ++i)
        {
            nameBuffer[i] = '\0';
        }
    }
    void EditorInspector::update()
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

        isFocused = ImGui::IsWindowFocused();

        Copium::GameObject* selectedGameObject = sceneManager.selectedGameObject;
        Copium::File* selectedFile = fileSystem.get_selected_file();
        Copium::Directory* selectedDirectory = fileSystem.get_selected_directory();
        if (selectedGameObject && !selectedFile)
        {
            // Set flags for tables
            selectedGameObject->inspectorView();


            // Bean: This should be in the selected gameobject inspector view along the isAddingComponent part
            float textWidth = ImGui::CalcTextSize("Add Component").x;
            float padding = 150.f;
            float indent = (ImGui::GetContentRegionAvail().x - textWidth - padding) * 0.5f;
            ImGui::Indent(indent);

            //AlignForWidth(buttonSize.x);
            ImVec2 buttonSize(textWidth + padding, 0.f);
            if (ImGui::Button("Add Component", buttonSize)) {
                isAddingComponent = true;
                targetGameobjectName = sceneManager.selectedGameObject->uuid;
            }
        }
        else if (selectedFile)
        {
            selectedFile->inspector_view();
        }
        else if (selectedDirectory)
        {
            selectedDirectory->inspector_view();
        }

        ImGui::PopStyleVar();
        ImGui::End();

        // Adding new components
        if (isAddingComponent)
        {
            if (sceneManager.selectedGameObject != nullptr && sceneManager.selectedGameObject->uuid == targetGameobjectName)
            {
                ImGui::Begin("Add Component", &isAddingComponent);
                AlignforWidth(ImGui::GetWindowSize().x);
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
                        selectedGameObject->AddComponent(it->first);
                        isAddingComponent = false;
                        break;
                    }
                }
                for (auto& nameToScriptClass : MyScriptingSystem.getScriptFiles())
                {
                    const std::string& name{ nameToScriptClass.filename().stem().string() };
                    if (!MyScriptingSystem.isScript(name))
                        continue;
                    if (filter.PassFilter(name.c_str()) && ImGui::Button((name + "[Script]").c_str(), buttonSize)) {
                        selectedGameObject->AddComponent<Copium::Script>().Name(name);
                        isAddingComponent = false;
                    }
                }
                static std::string newScriptPrompt;
                newScriptPrompt.clear();
                newScriptPrompt += "[New Script]";
                newScriptPrompt += filter.InputBuf;
                if (ImGui::Button(newScriptPrompt.c_str(), buttonSize))
                {
                    //Ask scripting system query if file exists
                    MyScriptingSystem.addEmptyScript(filter.InputBuf);
                    selectedGameObject->AddComponent<Copium::Script>().Name(filter.InputBuf);
                    isAddingComponent = false;
                }

                newScriptPrompt.clear();
                newScriptPrompt += "[New Scriptable Object]";
                newScriptPrompt += filter.InputBuf;
                if (ImGui::Button(newScriptPrompt.c_str(), buttonSize))
                {
                    std::ofstream file(Paths::assetPath + "\\Scripts\\" + filter.InputBuf + ".so");
                    file << "using System;\n";
                    file << "using CopiumEngine;\n\n";
                    file << "public class " << filter.InputBuf << ": ScriptableObject\n{\n\n";
                    file << "}\n";
                    file.close();
                    //selectedGameObject->addComponent<Copium::Script>().Name(filter.InputBuf);

                    isAddingComponent = false;
                }

                ImGui::End();
            }
            else
            {
                isAddingComponent = false;
            }
        }
    }
    void EditorInspector::exit()
    {

    }
}
