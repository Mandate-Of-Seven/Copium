/*!***************************************************************************************
\file			game-object.cpp
\project
\author			Zacharie Hong (50%), 
\co-author      Matthew Lau (50%)

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Defines GameObject Class and member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


//INCLUDES

#include "pch.h"
#include "GameObject/game-object.h"
#include "GameObject/Components/audiosource-component.h"
#include "GameObject/Components/camera-component.h"
#include "GameObject/Components/collider-components.h"
#include "GameObject/Components/component.h"
#include "GameObject/Components/physics-components.h"
#include "GameObject/Components/renderer-component.h"
#include "GameObject/Components/script-component.h"
#include "GameObject/Components/ui-components.h"
#include "GameObject/Components/sorting-group-component.h"
#include "Animation/animation-system.h"
#include "SceneManager/scene-manager.h"
#include <mono/jit/jit.h>

//USING

namespace 
{
    const std::string defaultGameObjName = "New GameObject"; // Append (No.) if its not the first
    Copium::MessageSystem& messageSystem{*Copium::MessageSystem::Instance()};
    Copium::SceneManager& sceneManager{ *Copium::SceneManager::Instance() };
}

namespace Copium
{
    ComponentID GameObject::assign_id()
    {
        if (!sceneManager.get_current_scene())
        {
            return 0;
        }
        //sceneManager.get_current_scene()->incr_component_count();
        return sceneManager.get_current_scene()->assignComponentID();
    }

    Component* GameObject::addComponent(ComponentType componentType)
    {
        switch (componentType)
        {
        case ComponentType::Animator:
            return &addComponent<Animator>();
        case ComponentType::BoxCollider2D:
            return &addComponent<BoxCollider2D>();
        case ComponentType::Camera:
            return &addComponent<Camera>();
        case ComponentType::Rigidbody2D:
            return &addComponent<Rigidbody2D>();
        case ComponentType::SpriteRenderer:
            return &addComponent<SpriteRenderer>();
        case ComponentType::Script:
            return &addComponent<Script>();
        case ComponentType::Button:
            return &addComponent<Button>();
        case ComponentType::Image:
            return &addComponent<ImageComponent>();
        case ComponentType::Text:
            return &addComponent<Text>();
        case ComponentType::AudioSource:
            return &addComponent<AudioSource>();
        case ComponentType::SortingGroup:
            return &addComponent<SortingGroup>();
        default:
            PRINT("ADDED NOTHING, MAYBE ADDED THE COMPONENT TO THE GAMEOBJECT.CPP");
            break;
        }
        return nullptr;
    }


    GameObject::~GameObject()
    {
        std::cout << "GameObject destructed\n";
        messageSystem.unsubscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);

        for (auto iter = components.begin(); iter != components.end(); ++iter)
        {
            if (*iter)
            {
                delete (*iter);
            }
        }
    }


    bool GameObject::isActive()
    {
        if (transform.hasParent())
        {
            return transform.parent->gameObj.isActive();
        }
        return active;
    }

    void GameObject::setActive(bool _active)
    {
        active = _active;
    }


GameObject::GameObject(const GameObject& rhs) : transform(*this), id{rhs.id}
{
    transform.position = rhs.transform.position;
    transform.rotation = rhs.transform.rotation;
    transform.scale = rhs.transform.scale;
    active = rhs.active;
    name = rhs.name;
    PRINT("GAMEOBJECT COPY CONSTRUCTOR!");
    messageSystem.subscribe(MESSAGE_TYPE::MT_CREATE_CS_GAMEOBJECT, this);
    MESSAGE_CONTAINER::reflectCsGameObject.gameObjID = id;
    MESSAGE_CONTAINER::reflectCsGameObject.componentIDs.clear();
    transform.id = rhs.transform.id;
    for (Component* pComponent : rhs.components)
    {

        Component* newComponent = pComponent->clone(*this);
        newComponent->id = pComponent->id;
        components.push_back(newComponent);
    }

    //if(MySceneManager.get_current_scene())
    //    MySceneManager.get_current_scene()->add_gameobject(this);

    //messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
    //for (Transform* pTransform : rhs.transform.children)
    //{
    //    GameObject* child = new GameObject(pTransform->gameObj);
    //    child->transform.setParent(&transform);
    //}


}

GameObject::GameObject
(GameObjectID _id, Math::Vec3 _position, Math::Vec3 _rotation, Math::Vec3 _scale)
    :
    name{ defaultGameObjName },
    transform(*this, _position, _rotation, _scale),
    active{true},
    id{ _id }
{
    messageSystem.subscribe(MESSAGE_TYPE::MT_CREATE_CS_GAMEOBJECT, this);
    MESSAGE_CONTAINER::reflectCsGameObject.gameObjID = id;
    MESSAGE_CONTAINER::reflectCsGameObject.componentIDs.clear();
    for (Component* pComponent : components)
    {
        MESSAGE_CONTAINER::reflectCsGameObject.componentIDs.push_back(pComponent->id);
    }
    messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
    //PRINT("GAMEOBJECT ID CONSTRUCTED: " << id);
    transform.id = assign_id();
}

GameObject& GameObject::operator=(const GameObject& _src)
{
    name = _src.get_name();
    transform.position = _src.transform.position;
    transform.rotation = _src.transform.rotation;
    transform.scale = _src.transform.scale;
    for (Component* co : components)
    {
        if (co)
        {
            sceneManager.get_current_scene()->add_unused_cid(co->id);
            delete co;
            co = nullptr;
        }
    }
    components.clear();
    
    for (Component* pComponent : _src.components)
    {
        components.push_back(pComponent->clone(*this));
    }
    return *this;
}


Component* GameObject::getComponent(ComponentType componentType)
{
    for (Component* pComponent : components)
    {
        if (pComponent->componentType == componentType)
            return pComponent;
    }
    return nullptr;
}



void GameObject::removeComponent(ComponentType componentType)
{
    if (componentType == ComponentType::Transform)
    {
        PRINT("CANNOT REMOVE TRANSFORM");
        return;
    }
    auto it{ components.begin() };
    while (it != components.end())
    {
        if ((*it)->componentType == componentType)
        {
            delete* it;
            components.erase(it);
            return;
        }
        ++it;
    }
    PRINT("Component of Type " << MAP_COMPONENT_TYPE_NAME[componentType] << " does not exist on " << name);
}

void GameObject::removeComponent(ComponentID componentID)
{
    auto it{ components.begin() };
    while (it != components.end())
    {
        if ((*it)->id == componentID)
        {
            delete *it;
            components.erase(it);
            return;
        }
        ++it;
    }
}


bool GameObject::hasComponent(ComponentType componentType) const
{
    auto it{ components.begin() };
    while (it != components.end())
    {
        if ((*it)->componentType == componentType)
        {
            return true;
        }
        ++it;
    }
    return false;
}

void GameObject::set_name(const std::string& _name){ name = _name; }
std::string GameObject::get_name() const{ return name; }

bool GameObject::deserialize(rapidjson::Value& _value) {

    if (!_value.HasMember("Name"))
        return false;
   
    name = _value["Name"].GetString();

    return true;
}

void GameObject::handleMessage(MESSAGE_TYPE mType)
{
    //MT_SCRIPTING_UPDATED
    MESSAGE_CONTAINER::reflectCsGameObject.gameObjID = id;
    MESSAGE_CONTAINER::reflectCsGameObject.componentIDs.clear();
    for (Component* pComponent : components)
    {
        MESSAGE_CONTAINER::reflectCsGameObject.componentIDs.push_back(pComponent->id);
    }
    messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
}


void GameObject::inspectorView()
{
    // Gameobject Basic Information
    ImGui::Checkbox("##Active", &active);
    ImGui::SameLine();
    static char buffer[256];
    strcpy(buffer, name.c_str());
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##gameObjName", buffer, 256);
    ImGui::PopItemWidth();
    name = buffer;

    // Layer
    ImGui::Text("Layer");
    ImGui::SameLine();
    
    // Bean: This is temporary and should be referenced from the sorting layer editor
    std::string items[] = { "Default", "Player", "UI", "Background" };
    static int itemCurrentIdx = 0;
    const char* previewItem = items[itemCurrentIdx].c_str();
    if (ImGui::BeginCombo(" ", previewItem))
    {
        for (int i = 0; i < IM_ARRAYSIZE(items); i++)
        {
            const bool isSelected = (itemCurrentIdx == i);
            if(ImGui::Selectable(items[i].c_str(), isSelected))
                itemCurrentIdx = i;

            // Bean: Change the gameobjects current layer
            if(isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Components
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH
        | ImGuiTableFlags_ScrollY;
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;
    if (ImGui::BeginTable("Components", 1, tableFlags, ImVec2(0.f, ImGui::GetWindowSize().y * 0.8f)))
    {
        ImGui::TableNextColumn();
        static std::vector<ComponentID> componentsToDelete;
        componentsToDelete.clear();

        int index = 0;
        ImGui::PushID(index++);
        if (ImGui::CollapsingHeader("Transform", nodeFlags))
        {
            transform.inspector_view();
        }
        ImGui::PopID();

        for (Component* component : components)
        {
            ImGui::PushID(index++);
            bool enabled = component->Enabled();
            ImGui::Checkbox("##Active", &enabled);
            component->Enabled(enabled);
            ImGui::SameLine();
            const std::string& componentName{ component->Name() };
            if (ImGui::CollapsingHeader(componentName.c_str(), nodeFlags))
            {
                if (ImGui::BeginDragDropSource())
                {
                    static void* container;
                    container = component;
                    ImGui::SetDragDropPayload(componentName.c_str(), &container, sizeof(void*));
                    ImGui::EndDragDropSource();
                }
                component->inspector_view();
                if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x, 0.f)))
                {
                    PRINT("ID: " << component->id);
                    componentsToDelete.push_back(component->id);
                    SceneManager::Instance()->get_current_scene()->add_unused_cid(component->id);
                }

            }
            ImGui::PopID();
        }
        for (ComponentID componentId : componentsToDelete)
        {
            removeComponent(componentId);
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}


bool GameObject::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
{
    _value.AddMember("ID", id, _doc.GetAllocator());

    rapidjson::Value _name;
    _name.SetString(name.c_str(), rapidjson::SizeType(name.length()), _doc.GetAllocator());
    _value.AddMember("Name", _name, _doc.GetAllocator());

    rapidjson::Value _components(rapidjson::kArrayType);
    rapidjson::Value transformComponent(rapidjson::kObjectType);
    transform.serialize(transformComponent, _doc);
    _components.PushBack(transformComponent, _doc.GetAllocator());
    for (auto iter = components.begin(); iter != components.end(); ++iter)
    {
        rapidjson::Value comp(rapidjson::kObjectType);
        // Serialize each component
        (*iter)->serialize(comp, _doc);
        _components.PushBack(comp, _doc.GetAllocator());
    }
    _value.AddMember("Components", _components, _doc.GetAllocator());

    //rapidjson::Value kids(rapidjson::kArrayType);
    //for (auto iter = transform.children.begin(); iter != transform.children.end(); ++iter)
    //{
    //    rapidjson::Value cgo(rapidjson::kObjectType);
    //    (*iter)->gameObj.serialize(cgo, _doc);
    //    kids.PushBack(cgo, _doc.GetAllocator());
    //}
    //_value.AddMember("Children", kids, _doc.GetAllocator());

    return true;

}


}