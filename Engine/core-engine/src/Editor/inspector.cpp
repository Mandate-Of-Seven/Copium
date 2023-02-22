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
#include <Events/events-system.h>
#include "Editor/inspector.h"
#include "Editor/editor-hierarchy-list.h"
#include "Files/assets-system.h"
#include <Scripting/scripting-system.h>

// Bean: Remove once we can auto select gameobjects
#include "Files/file-system.h"
#include <GameObject/components.h>
#include <Editor/editor-system.h>

#define BUTTON_HEIGHT .1 //Percent
#define BUTTON_WIDTH .6 //Percent
#define INPUT_BUFFER_SIZE 128

namespace Copium
{
    namespace
    {
        template <typename T>
        const char* GetTypeName() {
            #define FuncName std::string(__FUNCSIG__)
            static std::string typeName = FuncName.substr(FuncName.find_last_of('::') + 1, FuncName.find_last_of('>') - FuncName.find_last_of("::") - 1);
            return typeName.c_str();
        }

        static ImGuiTableFlags windowFlags =
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_NoBordersInBody |
            ImGuiTableFlags_NoSavedSettings |
            ImGuiTableFlags_SizingStretchProp;

        //bool isInspectorOpen;
        bool isAddingComponent {false};
        bool isAddingReference { false};
        //Either edited field, script or gameObject, or reference to other components

        void** pEditedContainer{nullptr};

        float* editedColor{ nullptr };
        Copium::SceneManager& sceneManager{ *Copium::SceneManager::Instance() };
        Copium::FileSystem& fileSystem{ *Copium::FileSystem::Instance() };
        Copium::AssetsSystem& assetsSystem{ *Copium::AssetsSystem::Instance() };
        char nameBuffer[INPUT_BUFFER_SIZE];

        using namespace Copium;

        template <typename T>
        void Display(const char* name, T& val);

        template <typename T>
        void DisplayPointer(T& container)
        {
            if constexpr (std::is_same<T, Script>())
            {
                static std::string buttonName{};
                buttonName = container.gameObj.name + " (" + container.Name() + ")";
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
            else if constexpr (std::is_same<T, GameObject>())
            {
                static std::string buttonName;
                buttonName = container.name + " (GameObject)";
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
            else if constexpr (ComponentTypes::has<T>())
            {
                static std::string buttonName;
                buttonName = container.gameObj.name + " (" + GetComponentType<T>::name + ")";
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
            else if constexpr (std::is_same<T, IUIComponent>())
            {
                static std::string buttonName{};
                buttonName = ((Component&)container).gameObj.name + " (" + GetTypeName<T>() + ")";
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
            else
            {
                static std::string buttonName { GetTypeName<T>() };
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
        }

        template <>
        void DisplayPointer(Texture& container)
        {
            static std::string buttonName{};
            const std::string& filePath{ container.get_file_path() };
            size_t offset = filePath.find_last_of("\\");
            if(offset != 0)
            {
                buttonName = filePath.substr(offset + 1, filePath.length() - offset) + " (Texture)";
            }
            else
            {
                buttonName = "None (Texture)";
            }
            
            ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
        }


        //Field name, Container
        template <typename T>
        void AddReferencePanel(T*& container, const char* alternateName = nullptr)
        {
            //ZACH: If no one is adding reference or the container does not match
            if (!isAddingReference || (T**)pEditedContainer != &container)
            {
                return;
            }
            Scene* pScene = MySceneManager.get_current_scene();
            if (!pScene)
                return;
            static ImGuiTextFilter filter;
            static std::string windowName;
            windowName = "Add ";
            if (alternateName)
            {
                windowName += alternateName;
                windowName += " [";
                windowName += GetTypeName<T>();
                windowName += "]";
            }
            else
            {
                windowName += GetTypeName<T>();
            }
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos(ImVec2(100.f, 100.f), ImGuiCond_FirstUseEver);
            windowName += " Reference";
            if (ImGui::Begin(windowName.c_str(), &isAddingReference))
            {
                ImGui::PushItemWidth(-1);
                filter.Draw("##References");
                ImGui::PopItemWidth();
                static std::string buttonName{};
                if constexpr (std::is_same<T, GameObject>())
                {
                    for (T& gameObject : pScene->gameObjects)
                    {
                        ImVec2 buttonSize = ImGui::GetWindowSize();
                        buttonSize.y *= (float)BUTTON_HEIGHT;
                        buttonName = gameObject.name + " [GameObject]";
                        if (filter.PassFilter(gameObject.name.c_str()) && ImGui::Button(buttonName.c_str(), buttonSize))
                        {
                            isAddingReference = false;
                            container = &gameObject;
                            break;
                        }
                    }
                }
                else if constexpr (ComponentTypes::has<T>())
                {
                    for (T& component : pScene->componentArrays.GetArray<T>())
                    {
                        if constexpr (std::is_same<T, Script>())
                        {
                            if (component.Name() != alternateName)
                                continue;
                        }
                        buttonName = component.gameObj.name;
                        buttonName += " [";
                        if (alternateName)
                        {
                            buttonName += alternateName;
                            buttonName += " - ";
                        }
                        buttonName += GetTypeName<T>();
                        buttonName += "]";
                        ImVec2 buttonSize = ImGui::GetWindowSize();
                        buttonSize.y *= (float)BUTTON_HEIGHT;
                        if (filter.PassFilter(component.gameObj.name.c_str()) && ImGui::Button(buttonName.c_str(), buttonSize))
                        {
                            isAddingReference = false;
                            container = &component;
                            break;
                        }
                    }
                }
                else if constexpr (std::is_same<T, IUIComponent>())
                {
                    for (Image& component : pScene->componentArrays.GetArray<Image>())
                    {
                        buttonName = component.gameObj.name;
                        buttonName += " [Image]";
                        ImVec2 buttonSize = ImGui::GetWindowSize();
                        buttonSize.y *= (float)BUTTON_HEIGHT;
                        if (filter.PassFilter(component.gameObj.name.c_str()) && ImGui::Button(buttonName.c_str(), buttonSize))
                        {
                            isAddingReference = false;
                            container = &component;
                            break;
                        }
                    }
                    for (Text& component : pScene->componentArrays.GetArray<Text>())
                    {
                        buttonName = component.gameObj.name;
                        buttonName += " [Text]";
                        ImVec2 buttonSize = ImGui::GetWindowSize();
                        buttonSize.y *= (float)BUTTON_HEIGHT;
                        if (filter.PassFilter(component.gameObj.name.c_str()) && ImGui::Button(buttonName.c_str(), buttonSize))
                        {
                            isAddingReference = false;
                            container = &component;
                            break;
                        }
                    }
                }
                else if constexpr (std::is_same<T, Texture>()) 
                {
                    for (int i = 0; i < MyAssetSystem.GetTextures().size(); i++)
                    {
                        std::string path = MyAssetSystem.GetTexture(i)->get_file_path();
                        buttonName = path;
                        buttonName += " [Texture]";
                        ImVec2 buttonSize = ImGui::GetWindowSize();
                        buttonSize.y *= (float)BUTTON_HEIGHT;
                        if (filter.PassFilter(path.c_str()) && ImGui::Button(buttonName.c_str(), buttonSize))
                        {
                            // Attach Reference
                            container = MyAssetSystem.GetTexture(i);
                            isAddingReference = false;
                            break;
                        }
                    }
                }
                ImGui::End();
            }
            //Reset the edited container back to false
            if (isAddingReference == false)
            {
                pEditedContainer = nullptr;
            }
        }

        template <typename T>
        void DisplayType(const char* name, T*& container)
        {
            //static_assert(std::is_same<T, Script>());
            static std::string buttonName{};
            if (container == nullptr)
            {
                buttonName = "None ";
                buttonName += '(';
                buttonName += GetTypeName<T>();
                buttonName += ')';
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
            else
            {
                DisplayPointer(*container);
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                PRINT("Double clicked on " << GetTypeName<T>() << " reference field");
                isAddingReference = true;
                pEditedContainer = reinterpret_cast<void**>(&container);
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetTypeName<T>());
                if (payload)
                {
                    container = (T*)(*reinterpret_cast<void**>(payload->Data));
                }
                ImGui::EndDragDropTarget();
            }
            AddReferencePanel(container);
        }

        void DisplayType(const char* name, Script*& container, const char* scriptName)
        {
            static std::string buttonName{};
            if (container == nullptr)
            {
                buttonName = "None ";
                buttonName += '(';
                buttonName += scriptName;
                buttonName += " - Script";
                buttonName += ')';
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
            else
            {
                DisplayPointer(*container);
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                std::cout << "double clicked on game object reference field\n";
                isAddingReference = true;
                pEditedContainer = reinterpret_cast<void**>(&container);

            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(scriptName);
                if (payload)
                {
                    container = (Script*)(*reinterpret_cast<void**>(payload->Data));
                }
                ImGui::EndDragDropTarget();
            }
            AddReferencePanel(container,scriptName);
        }

        template <>
        void DisplayType(const char* name, Texture*& container)
        {
            static std::string buttonName{};
            if (container == nullptr)
            {
                buttonName = "None (Texture)";
                ImGui::Button(buttonName.c_str(), ImVec2(-FLT_MIN, 0.f));
            }
            else
            {
                buttonName = container->get_file_path() + " (Texture)";
                DisplayPointer(*container);
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                isAddingReference = true;
                pEditedContainer = reinterpret_cast<void**>(&container);
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem");
                if (payload)
                {
                    std::string str = (const char*)(payload->Data);
                    for (int i = 0; i < MyAssetSystem.GetTextures().size(); i++)
                    {
                        if (!MyAssetSystem.GetTexture(i)->get_file_path().compare(str))
                        {
                            // Attach Reference
                            container = MyAssetSystem.GetTexture(i);
                            break;
                        }
                    }
                }
                ImGui::EndDragDropTarget();
            }
            AddReferencePanel(container);
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

        template <size_t SZ>
        void DisplayType(const char* name, char (&val)[SZ])
        {
            static std::string idName{};
            idName = "##";
            idName += name;
            ImGui::InputTextMultiline(idName.c_str(), val, SZ, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
        }

        void DisplayType(const char* name, char*& val)
        {
            static std::string idName{};
            idName = "##";
            idName += name;
            ImGui::InputTextMultiline(idName.c_str(), val, TEXT_BUFFER_SIZE, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
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
        }

        void DisplayType(const char* name, double& val)
        {
            static std::string idName{};
            idName = "##";
            idName += name;
            float temp{(float)val};
            ImGui::DragFloat(idName.c_str(), &temp, 0.15f);
            val = temp;
        }

        void DisplayType(const char* name, Math::Vec3& val)
        {
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
            ImGui::AlignTextToFramePadding();
            ImGui::TableNextColumn();
            ImGui::Text(name);
            ImGui::TableNextColumn();
            DisplayType(name, val);
        }

        void Display(const char* name, Script*& val, const char* scriptName)
        {
            ImGui::AlignTextToFramePadding();
            ImGui::TableNextColumn();
            ImGui::Text(name);
            ImGui::TableNextColumn();
            DisplayType(name, val, scriptName);
        }

        void Display(const char* string)
        {
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(string);
        }

        void Display(const char* name, Field& field)
        {
            switch (field.fType)
            {
                case FieldType::Float:
                    Display(name, field.Get<float>());
                    break;
                case FieldType::Double:
                    Display(name, field.Get<double>());
                    break;
                case FieldType::Bool:
                    Display(name, field.Get<bool>());
                    break;
                case FieldType::Char:
                    //Display(name, field.Get<char>());
                    break;
                case FieldType::Short:
                    Display(name, field.Get<int>());
                    break;
                case FieldType::Int:
                    Display(name, field.Get<int>());
                    break;
                case FieldType::Long:
                    Display(name, field.Get<int>());
                    break;
                case FieldType::UShort:
                    break;
                case FieldType::UInt:
                    break;
                case FieldType::ULong:
                    break;
                case FieldType::String:
                {
                    Display(name, (char*&)field.data);
                    break;
                }
                case FieldType::Vector2:
                {
                    Display(name, field.Get<Math::Vec2>());
                    break;
                }
                case FieldType::Vector3:
                {
                    Display(name, field.Get<Math::Vec3>());
                    break;
                }
            }
        }

        template <typename T>
        void DisplayColor(const char* name, T& val)
        {
            static_assert(sizeof(T) == sizeof(float) * 4);
            ImGui::TableNextColumn();
            ImGui::Text(name);
            ImGui::TableNextColumn();
            static ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
                | ImGuiColorEditFlags_NoLabel;
            if (ImGui::ColorButton(name, reinterpret_cast<ImVec4&>(val), miscFlags, ImVec2(FLT_MAX, 0)))
            {
                editedColor = reinterpret_cast<float*>(&val);
            }

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

            static bool AddComponent(ImGuiTextFilter& filter, GameObject& gameObj) { return AddComponentHelper<Component,Components...>(filter, gameObj); }

            template <typename T, typename... Ts>
            static bool AddComponentHelper(ImGuiTextFilter& filter, GameObject& gameObj)
            {
                ImVec2 buttonSize = ImGui::GetWindowSize();
                buttonSize.y *= (float)BUTTON_HEIGHT;
                if constexpr (std::is_same<Script, T>())
                {
                    static std::vector<const char*> scriptNames;
                    MyEventSystem->publish(new ScriptGetNamesEvent(scriptNames));
                    for (const char* name : scriptNames)
                    {
                        static std::string newName;
                        newName = name;
                        newName += " [Script]";
                        if (filter.PassFilter(newName.c_str()) && ImGui::Button(newName.c_str(), buttonSize))
                        {
                            T* component;
                            MyEventSystem->publish(new ComponentAddEvent<Script>{ gameObj,component,name});
                            return true;
                        }
                    }
                    scriptNames.clear();
                }
                else
                {
                    static const char* name = GetComponentType<T>::name;
                    PRINT(name);
                    if (filter.PassFilter(name) && ImGui::Button(name, buttonSize))
                    {
                        T* component;
                        MyEventSystem->publish(new ComponentAddEvent{ gameObj,component });
                        return true;
                    }
                }
                if constexpr (sizeof...(Ts) != 0)
                {
                    return AddComponentHelper<Ts...>(filter, gameObj);
                }
                else
                {
                    return false;
                }
            }
        };
        using AddAllComponentsStruct = decltype(AddComponentStruct(ComponentTypes()));
        bool AddComponent(ImGuiTextFilter& filter, GameObject& gameObj) { return AddAllComponentsStruct::AddComponent(filter, gameObj); }

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
                //    //scriptingSystem.addNoneScript(filter.InputBuf);
                //    //selectedGameObject->addComponent<Copium::Script>().Name(filter.InputBuf);
                //    isAddingComponent = false;
                //}
                ImGui::End();
            }
        }


        template <typename T>
        void DisplayComponent(T& component)
        {
            //PRINT("Component of type: " << GetComponentType<T>::name << " does not exist yet! ");
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
            Display("Sprite", spriteRenderer.sprite.refTexture);
            DisplayColor("Color", spriteRenderer.sprite.color);
            if (spriteRenderer.sprite.refTexture)
            {
                std::string filePath = spriteRenderer.sprite.refTexture->get_file_path();
                uint64_t pathID = std::hash<std::string>{}(filePath);
                MetaID metaID = MyAssetSystem.GetMetaID(pathID);
                spriteRenderer.sprite.spriteID = metaID.uuid;
                size_t pos = filePath.find_last_of('\\');
                spriteRenderer.sprite.sprite_name = filePath.substr(pos + 1, filePath.length() - pos);
            }
        }

        template <>
        void DisplayComponent<Text>(Text& text)
        {
            Display("Font Size",text.fSize);
            Display("Content", text.content);
            Display("Wrapping", text.wrapper);
            DisplayColor("Color", text.color);
            //DisplayDragDrop();
            //spriteRenderer.sprite.set_name()
        }

        template <>
        void DisplayComponent<Image>(Image& image)
        {
            Display("Image", image.sprite.refTexture);
            DisplayColor("Color", image.sprite.color);

            //Update sprite data
            if (image.sprite.refTexture)
            {
                std::string filePath = image.sprite.refTexture->get_file_path();
                uint64_t pathID = std::hash<std::string>{}(filePath);
                MetaID metaID = MyAssetSystem.GetMetaID(pathID);
                image.sprite.spriteID = metaID.uuid;
                size_t pos = filePath.find_last_of('\\');
                image.sprite.sprite_name = filePath.substr(pos + 1, filePath.length() - pos);
            }
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
        void DisplayComponent<Script>(Script& script)
        {
            static Field buffer(FieldType::None, 128);
            for (auto pair : script.fieldDataReferences)
            {
                const char* name = pair.first.c_str();
                Field& field{ pair.second };
            //    //Component Enum + ComponentType Enum
                if (field.fType >= FieldType::Component)
                {
                    ComponentType cType = (ComponentType)field.fType;
                    switch (cType) {
                    case ComponentType::Animator:
                        Display(name, (Animator*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::AudioSource:
                        Display(name, (AudioSource*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::BoxCollider2D:
                        Display(name, (BoxCollider2D*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::Button:
                        Display(name, (Button*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::Camera:
                        Display(name, (Camera*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::Image:
                        Display(name, (Image*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::Rigidbody2D:
                        Display(name, (Rigidbody2D*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::SpriteRenderer:
                        Display(name, (SpriteRenderer*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::Script:
                        Display(name, (Script*&)script.fieldComponentReferences[name], field.typeName.c_str());
                        break;
                    case ComponentType::Transform:
                        Display(name, (Transform*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::Text:
                        Display(name, (Text*&)script.fieldComponentReferences[name]);
                        break;
                    case ComponentType::SortingGroup:
                        Display(name, (SortingGroup*&)script.fieldComponentReferences[name]);
                        break;
                    default:
                        // handle invalid case
                        break;
                    }
                }
                else if (field.fType == FieldType::GameObject)
                {
                    Display(name,script.fieldGameObjReferences[name]);
                }
                else
                {
                    buffer.fType = field.fType;
                    MyEventSystem->publish(new ScriptGetFieldEvent(script, name, field.data));
                    Display(name, field);
                    MyEventSystem->publish(new ScriptSetFieldEvent(script, name, field.data));
                }
            }
        }

        template <>
        void DisplayComponent<Button>(Button& btn)
        {

            //DisplayDragDrop();
            //spriteRenderer.sprite.set_name()
            Display("Target Graphic", btn.targetGraphic);
            Display("Bounds", btn.bounds);
            DisplayColor("Normal Color", btn.normalColor);
            DisplayColor("Hover Color", btn.hoverColor);
            DisplayColor("Clicked Color", btn.clickedColor);

            // If it has an image, set bounds based on image once
            static bool setOnce = false;
            if (btn.gameObj.HasComponent<Image>() && !setOnce)
            {
                Image* image = btn.gameObj.GetComponent<Image>();
                Texture* texture = image->sprite.refTexture;
                
                float height = texture->get_pixel_height();
                float width = texture->get_pixel_width();

                btn.bounds.max.x *= (1 / (float)width * 0.5f);
                btn.bounds.max.y *= (1 / (float)height * 0.5f);
                btn.bounds.min.x *= (1 / (float)width * 0.5f);
                btn.bounds.min.y *= (1 / (float)height * 0.5f);

                setOnce = true;
            }
            
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
            Display("Number of Animations:");
            Display(std::to_string(animator.get_animation_vector().size()).c_str());
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
                Display("Columns", animator.animations[i].spriteSheet.columns);
                Display("Rows", animator.animations[i].spriteSheet.rows);
                Display("Time Delay", animator.animations[i].timeDelay);
                Display("Sprite", animator.animations[i].spriteSheet.texture);

                // Update sprite data
                if (animator.animations[i].spriteSheet.texture)
                {
                    std::string filePath = animator.animations[i].spriteSheet.texture->get_file_path();
                    uint64_t pathID = std::hash<std::string>{}(filePath);
                    MetaID metaID = MyAssetSystem.GetMetaID(pathID);
                    animator.animations[i].spriteSheet.spriteID = metaID.uuid;
                    size_t pos = filePath.find_last_of('\\');
                    animator.animations[i].spriteSheet.name = filePath.substr(pos + 1, filePath.length() - pos);
                }

                ImGui::PopID();
            }
        }


        template <>
        void DisplayComponent<AudioSource>(AudioSource& audioSource)
        {
            ImGui::Indent();
            // Sprite
            // Extern source file

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Audio File");
            ImGui::TableNextColumn();
            ImGui::Button(audioSource.alias.c_str(), ImVec2(-FLT_MIN, 0.f));
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
                {
                    std::string str = (const char*)(payload->Data);
                    size_t fileDot = str.find_last_of(".");
                    std::string extension = str.substr(fileDot, str.size());

                    if (extension == ".wav")
                    {
                        audioSource.stop_sound();//stop any currently playing audio

                        size_t lastSlash = str.find_last_of("\\");
                        std::string temp = str.substr(lastSlash + 1);
                        size_t lastDot = temp.find_last_of(".");
                        audioSource.alias = temp.substr(0, lastDot);
                        //std::cout << "Alias: " << temp << "\n";


                        if (MySoundSystem.soundList.find(audioSource.alias) == MySoundSystem.soundList.end())//if its true it means file doesnt exist yet
                        {
                            std::cout << "New sound file detected: " << str << " / Alias (" << audioSource.alias << ")\n";
                            SoundSystem::Instance()->CreateSound(str, audioSource.alias);
                        }
                        else
                        {
                            MySoundSystem.soundList[audioSource.alias].first->getVolume(&audioSource.volume);
                        }
                    }
                    else
                    {
                        std::cout << "Wrong file type\n";
                        Window::EditorConsole::editorLog.add_logEntry("AudioSource only accepts the.wav file format");
                        Window::EditorConsole::editorLog.bring_to_front();
                    }

                }
                ImGui::EndDragDropTarget();
            }


            //drop down for channel sellection
            if (ImGui::BeginCombo("##combo", audioSource.channel.c_str()))
            {
                for (int n = 0; n < IM_ARRAYSIZE(audioSource.channelName); n++)
                {
                    bool is_selected = (audioSource.channel == audioSource.channelName[n]);
                    if (ImGui::Selectable(audioSource.channelName[n], is_selected))
                    {
                        audioSource.channel = audioSource.channelName[n];
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::Text("Channel");

            //volume slider
            ImGui::SliderFloat("Volume", &audioSource.volume, 0.0f, 1.0f, "%.2f");
            SoundSystem::Instance()->soundList[audioSource.alias].first->setVolume(audioSource.volume);

            ImGui::Checkbox("Overlap", &audioSource.overLap);
            ImGui::Checkbox("Loop", &audioSource.loop);

            if (ImGui::Button("Preview")) //play
            {
                if (audioSource.alias.size())
                {
                    if (audioSource.channel == "Default")
                    {
                        MySoundSystem.Play(audioSource.alias, MySoundSystem.channelDefault, audioSource.overLap, audioSource.loop, audioSource.loopCount);
                    }
                    else if (audioSource.channel == "BGM")
                    {
                        MySoundSystem.Play(audioSource.alias, MySoundSystem.channelBGM, audioSource.overLap, audioSource.loop, audioSource.loopCount);
                    }
                    else if (audioSource.channel == "SFX")
                    {
                        MySoundSystem.Play(audioSource.alias, MySoundSystem.channelSFX, audioSource.overLap, audioSource.loop, audioSource.loopCount);
                    }
                    else if (audioSource.channel == "Voice")
                    {
                        MySoundSystem.Play(audioSource.alias, MySoundSystem.channelVoice, audioSource.overLap, audioSource.loop, audioSource.loopCount);
                    }
                    else if (true)
                    {
                        PRINT("No channel detected, Playing on default");
                        MySoundSystem.Play(audioSource.alias, MySoundSystem.channelDefault, audioSource.overLap, audioSource.loop, audioSource.loopCount);
                    }
                }
                else
                {
                    Window::EditorConsole::editorLog.add_logEntry("Error: No audio file to preview");
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Stop")) //stop
            {
                if (audioSource.alias.size())
                {
                    MySoundSystem.Stop(audioSource.alias);
                }
                else
                {
                    Window::EditorConsole::editorLog.add_logEntry("Error: No audio file to stop");
                }
            }


            if (ImGui::Button("Stop ALL"))
            {
                if (audioSource.alias.size())
                {
                    std::cout << "Stopping all audio\n";
                    MySoundSystem.StopAll();
                }
            }

            ImGui::Unindent();
            //ImGui::EndTable();

        }

        template <>
        void DisplayComponent<Camera>(Camera& camera)
        {
            bool openPopup = false;

            glm::vec4 clrGLM = camera.GetBackgroundColor();

            ImVec4 color = { clrGLM.r, clrGLM.g, clrGLM.b, clrGLM.a };

            static ImVec4 backupColor;

            ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
                | ImGuiColorEditFlags_NoLabel;

            ImGui::Indent();
            // Sprite
            // Extern source file

            // Background Color
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Color");
            ImGui::TableNextColumn();
            openPopup = ImGui::ColorButton("Color", color, miscFlags, ImVec2(FLT_MAX, 0));

            // Projection
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Projection");
            ImGui::TableNextColumn();

            ImGui::Text("Orthographic"); ImGui::SameLine();
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            bool temp = camera.IsOrthographic();
            if (ImGui::Checkbox("", &temp))
            {
                camera.SetOrthographic(temp);
            }
            ImGui::PopItemFlag();

            // Clipping Planes
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Clipping Planes");
            ImGui::TableNextColumn();

            ImGui::PushID(0);
            ImGui::Text("Near"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            float temp2 = camera.GetNearClip();
            if (ImGui::InputFloat("", &temp2))
            {
                camera.SetNearClip(temp2);
            }
            if (ImGui::IsItemEdited())
                camera.update_ortho_projection();
            ImGui::PopID();

            ImGui::PushID(1);
            ImGui::Text("Far"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            float temp3 = camera.GetFarClip();
            if (ImGui::InputFloat("", &temp3))
            {
                camera.SetFarClip(temp3);
            }
            if (ImGui::IsItemEdited())
                camera.update_ortho_projection();
            ImGui::PopID();

            ImGui::Unindent();
            //ImGui::EndTable();
        }


        template <>
        void DisplayComponent<SortingGroup>(SortingGroup& sortingGroup)
        {

            // Bean: This is temporary and should be referenced from the sorting layer editor
            SortingLayers& editorSortingLayer = *MyEditorSystem.getLayers()->SortLayers();
            std::vector<Layer>& sortingLayers = editorSortingLayer.GetSortingLayers();
            
            const char* previewItem =nullptr;
            for (Layer& lay : sortingLayers)
            {
                if (lay.layerID == (unsigned int)sortingGroup.sortingLayer)
                {
                    //PRINT("Layer Name: " << lay.name);
                    previewItem = lay.name.c_str();
                    break;
                }
            }
            if (!previewItem)
                previewItem = "NULL";

            

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Layer Selection");
            ImGui::TableNextColumn();

            if (ImGui::BeginCombo("##LayerSelection", previewItem))
            {
                for (int i = 0; i < editorSortingLayer.GetSortingLayers().size(); i++)
                {
                    const bool isSelected = ((unsigned int)sortingGroup.sortingLayer == editorSortingLayer.GetSortingLayers()[i].layerID);

                    //ImGuiSelectableFlags flags = ImGuiSelectableFlags_AllowItemOverlap;
                    //char* name = sortingLayers[i].name.data();
                    int index = sortingLayers[i].layerID + 1;
                    std::string label = "##" + std::to_string(index);
                    if (ImGui::Selectable(label.c_str(), isSelected))
                    {
                        index = editorSortingLayer.GetSortingLayers()[i].layerID;
                        if (index != sortingGroup.sortingLayer)
                        {
                            sortingGroup.sortingLayer = index;
                            MyEditorSystem.getLayers()->SortLayers()->RemoveGameObject(sortingGroup.sortingLayer, *MyEditorSystem.pSelectedGameObject);
                            MyEditorSystem.getLayers()->SortLayers()->AddGameObject(editorSortingLayer.GetSortingLayers()[i].layerID, *MyEditorSystem.pSelectedGameObject);

                            // Michael Buble sort here
                            Layer* layer = &editorSortingLayer.GetSortingLayers()[i];
                            bool swapped{ false };
                            for (size_t j{ 0 }; j < layer->gameObjects.size() - 1; ++j)
                            {
                                for (size_t k{ 0 }; k < layer->gameObjects.size() - 1 - j; ++k)
                                {
                                    SortingGroup* sg1{ nullptr }, * sg2{ nullptr };

                                    if (!layer->gameObjects[k] && layer->gameObjects[k + 1])
                                    {
                                        std::swap(layer->gameObjects[k], layer->gameObjects[k + 1]);
                                        swapped = true;
                                        continue;
                                    }

                                    if (layer->gameObjects[k] && layer->gameObjects[k + 1])
                                    {
                                        Component* co1 = layer->gameObjects[k]->GetComponent<SortingGroup>();
                                        Component* co2 = layer->gameObjects[k + 1]->GetComponent<SortingGroup>();

                                        if (co1 && co2)
                                        {
                                            sg1 = reinterpret_cast<SortingGroup*>(co1);
                                            sg2 = reinterpret_cast<SortingGroup*>(co2);

                                            if (sg1->GetOrderInLayer() > sg2->GetOrderInLayer())
                                            {
                                                std::swap(layer->gameObjects[k], layer->gameObjects[k + 1]);
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
            if (ImGui::DragInt("", &sortingGroup.orderInLayer, 1.f, 0, 100))
            {
                //PRINT("changing order in layer");
                // Sort the layer based on all order ids
                // Michael Buble sort here
                Layer* layer{ nullptr };
                for (Layer& lay : sortingLayers)
                {
                    if (lay.layerID == (unsigned int)sortingGroup.sortingLayer)
                    {
                        //PRINT("Layer Name: " << lay.name);
                        layer = &lay;
                        break;
                    }
                }
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
                                Component* co1 = layer->gameObjects[j]->GetComponent<SortingGroup>();
                                Component* co2 = layer->gameObjects[j + 1]->GetComponent<SortingGroup>();

                                if (co1 && co2)
                                {
                                    sg1 = reinterpret_cast<SortingGroup*>(co1);
                                    sg2 = reinterpret_cast<SortingGroup*>(co2);

                                    if (sg1->GetOrderInLayer() > sg2->GetOrderInLayer())
                                    {

                                        PRINT(sg1->GetOrderInLayer() << '|' << sg2->GetOrderInLayer());
                                        std::swap(layer->gameObjects[j], layer->gameObjects[j + 1]);
                                        swapped = true;
                                    }
                                }
                            }

                        }

                        if (!swapped)
                        {
                            PRINT("No swapping detected, breaking out of loop!");
                            break;

                        }
                    }
                }
            }



        }


        template <typename T>
        void DisplayComponentHelper(T& component)
        {
            ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;
            static std::string name{};
            if constexpr (std::is_same<T, Script>())
            {
                name = (component.name + " [Script]");
            }
            else
            {
                name = GetComponentType<T>::name;
            }
            if (ImGui::CollapsingHeader(name.c_str(), nodeFlags))
            {
                if (ImGui::BeginDragDropSource())
                {
                    void* container = &component;
                    if constexpr (std::is_same<T, Script>())
                    {
                        ImGui::SetDragDropPayload(component.Name().c_str(), &container, sizeof(void*));
                        ImGui::EndDragDropSource();
                    }
                    else
                    {
                        ImGui::SetDragDropPayload(GetTypeName<T>(), &container, sizeof(void*));
                        ImGui::EndDragDropSource();
                    }
                }
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
                    DisplayComponent(component);
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


        template<typename T, typename... Ts>
        struct DisplayComponentsStruct
        {
        public:
            constexpr DisplayComponentsStruct(TemplatePack<T, Ts...> pack) {}
            DisplayComponentsStruct() = delete;
            DisplayComponentsStruct(GameObject& gameObj) 
            {
                ImGui::TableNextColumn();
                DisplayComponentHelper(gameObj.transform);
                DisplayNext<T, Ts...>(gameObj); 
            }
        private:
            template<typename T1, typename... T1s>
            void DisplayNext(GameObject& gameObj)
            {
                ComponentsPtrArray<T1>& components{gameObj.GetComponents<T1>()};
                for (T1* component : components)
                {
                    ImGui::PushID(component->uuid);
                    DisplayType("Enabled", component->enabled); ImGui::SameLine();
                    DisplayComponentHelper(*component);
 
                    if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x, 0.f)))
                    {
                        PRINT("DELETING");
                        MyEventSystem->publish(new ComponentDeleteEvent<T1>(*component));
                    }
                    ImGui::PopID();
                }
                if constexpr (sizeof...(T1s) != 0)
                {
                    DisplayNext<T1s...>(gameObj);
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

        MyEventSystem->subscribe(this, &EditorInspector::CallbackStartPreview);
        MyEventSystem->subscribe(this, &EditorInspector::CallbackStopPreview);

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

        Copium::GameObject* selectedGameObject = MyEditorSystem.pSelectedGameObject;
        Copium::File* selectedFile = fileSystem.get_selected_file();
        Copium::Directory* selectedDirectory = fileSystem.get_selected_directory();
        if (selectedGameObject)
        {
            // Set flags for tables
            DisplayGameObject(*selectedGameObject);


            // Bean: This should be in the selected gameobject inspector view along the isAddingComponent part
            //float textWidth = ImGui::CalcTextSize("Add Component").x;
            //float padding = 150.f;
            //float indent = (ImGui::GetContentRegionAvail().x - textWidth - padding) * 0.5f;
            //ImGui::Indent(indent);

            ////AlignForWidth(buttonSize.x);
            //ImVec2 buttonSize(textWidth + padding, 0.f);
            //if (ImGui::Button("Add Component", buttonSize)) {
            //    isAddingComponent = true;
            //    targetGameobjectName = sceneManager.selectedGameObject->uuid;
            //}
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


        bool editing = editedColor;
        if (editing)
            ImGui::OpenPopup("Color");
        if (ImGui::BeginPopupModal("Color", &editing, ImGuiTableFlags_NoBordersInBody))
        {
            static ImGuiWindowFlags miscFlags = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview;
            ImGui::ColorPicker4("##", editedColor, miscFlags);

            ImGui::EndPopup();
        }
        if (!editing)
            editedColor = nullptr;

        ImGui::End();

        // Adding new components
        //if (isAddingComponent)
        //{
        //    if (sceneManager.selectedGameObject != nullptr && sceneManager.selectedGameObject->uuid == targetGameobjectName)
        //    {
        //        ImGui::Begin("Add Component", &isAddingComponent);
        //        AlignforWidth(ImGui::GetWindowSize().x);
        //        ImVec2 buttonSize = ImGui::GetWindowSize();
        //        buttonSize.y *= (float)BUTTON_HEIGHT;
        //        static ImGuiTextFilter filter;
        //        ImGui::PushItemWidth(-1);
        //        filter.Draw("##ComponentName");
        //        ImGui::PopItemWidth();
        //        std::map<Copium::ComponentType, std::string>::iterator it;
        //        for (it = Copium::MAP_COMPONENT_TYPE_NAME.begin();
        //            it != Copium::MAP_COMPONENT_TYPE_NAME.end(); ++it)
        //        {
        //            if (it->first == Copium::ComponentType::Script)
        //                continue;
        //            const std::string& componentName{ it->second };
        //            if (filter.PassFilter(componentName.c_str()) && ImGui::Button(componentName.c_str(), buttonSize))
        //            {
        //                selectedGameObject->AddComponent(it->first);
        //                isAddingComponent = false;
        //                break;
        //            }
        //        }
        //        for (auto& nameToScriptClass : MyScriptingSystem.getScriptFiles())
        //        {
        //            const std::string& name{ nameToScriptClass.filename().stem().string() };
        //            if (!MyScriptingSystem.isScript(name))
        //                continue;
        //            if (filter.PassFilter(name.c_str()) && ImGui::Button((name + "[Script]").c_str(), buttonSize)) {
        //                selectedGameObject->AddComponent<Copium::Script>().Name(name);
        //                isAddingComponent = false;
        //            }
        //        }
        //        static std::string newScriptPrompt;
        //        newScriptPrompt.clear();
        //        newScriptPrompt += "[New Script]";
        //        newScriptPrompt += filter.InputBuf;
        //        if (ImGui::Button(newScriptPrompt.c_str(), buttonSize))
        //        {
        //            //Ask scripting system query if file exists
        //            MyScriptingSystem.addNoneScript(filter.InputBuf);
        //            selectedGameObject->AddComponent<Copium::Script>().Name(filter.InputBuf);
        //            isAddingComponent = false;
        //        }

        //        newScriptPrompt.clear();
        //        newScriptPrompt += "[New Scriptable Object]";
        //        newScriptPrompt += filter.InputBuf;
        //        if (ImGui::Button(newScriptPrompt.c_str(), buttonSize))
        //        {
        //            std::ofstream file(Paths::assetPath + "\\Scripts\\" + filter.InputBuf + ".so");
        //            file << "using System;\n";
        //            file << "using CopiumEngine;\n\n";
        //            file << "public class " << filter.InputBuf << ": ScriptableObject\n{\n\n";
        //            file << "}\n";
        //            file.close();
        //            //selectedGameObject->addComponent<Copium::Script>().Name(filter.InputBuf);

        //            isAddingComponent = false;
        //        }

        //        ImGui::End();
        //    }
        //    else
        //    {
        //        isAddingComponent = false;
        //    }
        //}
    }
    void EditorInspector::exit()
    {

    }

    void EditorInspector::CallbackStartPreview(StartPreviewEvent* pEvent)
    {
        editedColor = nullptr;
        isAddingComponent = false;
    }

    void EditorInspector::CallbackStopPreview(StopPreviewEvent* pEvent)
    {
        editedColor = nullptr;
        isAddingComponent = false;
    }

}
