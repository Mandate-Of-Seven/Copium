/*!***************************************************************************************
\file			game-object.cpp
\project
\author			Zacharie Hong (50%), Matthew Lau (50%)

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
#include "GameObject/Components/renderer-component.h"
#include "GameObject/Components/script-component.h"
#include "GameObject/Components/component.h"
#include "GameObject/Components/physics-components.h"
#include "GameObject/Components/collider-components.h"
#include "GameObject/Components/ui-components.h"
#include "SceneManager/sm.h"

//USING

namespace 
{
    const std::string defaultGameObjName = "New GameObject"; // Append (No.) if its not the first
    Copium::MessageSystem& messageSystem{*Copium::MessageSystem::Instance()};
    Copium::NewSceneManager& sceneManager{ *Copium::NewSceneManager::Instance() };
}

namespace Copium
{

GameObjectID GameObject::count = 1;

GameObject::~GameObject()
{
    messageSystem.unsubscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
    for (std::list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
    {
        if (*iter)
        {
            delete (*iter);
        }
    }
}


GameObject::GameObject(const GameObject& rhs) : transform(*this), id{ count++ }, parent{ nullptr }, parentid{0}
{
    messageSystem.subscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
    MESSAGE_CONTAINER::reflectCsGameObject.ID = id;
    messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
    transform = rhs.transform;
    name = rhs.name;
    for (Component* pComponent : rhs.components)
    {
        Component* component = nullptr;
        switch (pComponent->componentType)
        {
        case ComponentType::Animator:
        {
            component = new AnimatorComponent(*this);
            *component = *(reinterpret_cast<AnimatorComponent*>(pComponent));
            PRINT("ADDED ANIMATOR");
            break;
        }
        case ComponentType::BoxCollider2D:
        {
            component = new BoxCollider2D(*this);
            *component = *(reinterpret_cast<BoxCollider2D*>(pComponent));
            PRINT("ADDED COLLIDER");
            break;
        }
        case ComponentType::Rigidbody2D:
        {
            component = new Rigidbody2D(*this);
            *component = *(reinterpret_cast<Rigidbody2D*>(pComponent));
            PRINT("ADDED Rigidbody");
            break;
        }
        case ComponentType::SpriteRenderer:
        {
            component = new SpriteRenderer(*this);
            *component = *(reinterpret_cast<SpriteRenderer*>(pComponent));
            PRINT("ADDED SPRITE RENDERER");
            break;
        }
        case ComponentType::Script:
        {
            component = new ScriptComponent(*this);
            *component = *(reinterpret_cast<ScriptComponent*>(pComponent));
            PRINT("ADDED SCRIPT");
            break;
        }
        case ComponentType::Button:
        {
            component = new ButtonComponent(*this);
            *component = *(reinterpret_cast<ButtonComponent*>(pComponent));
            PRINT("ADDED UI BUTTON");
            break;
        }
        case ComponentType::Image:
        {
            component = new ImageComponent(*this);
            *component = *(reinterpret_cast<ImageComponent*>(pComponent));
            PRINT("ADDED UI IMAGE");
            break;
        }
        case ComponentType::Text:
        {
            component = new TextComponent(*this);
            *component = *(reinterpret_cast<TextComponent*>(pComponent));
            PRINT("ADDED UI TEXT");
            break;
        }
        }
        if (component)
            components.push_back(component);
    }
    for (GameObject* pGameObj : rhs.children)
    {
        GameObject* child = sceneManager.get_gof().build_gameobject(*pGameObj);
        children.push_back(child);
        child->set_parent(this);
    }
}

GameObject::GameObject
(Math::Vec3 _position, Math::Vec3 _rotation, Math::Vec3 _scale)
    : 
    name{ defaultGameObjName }, parent{nullptr}, parentid{0}, 
    transform(*this, _position, _rotation, _scale), id{count++}
{
    messageSystem.subscribe(MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
    MESSAGE_CONTAINER::reflectCsGameObject.ID = id;
    messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
    PRINT("GAMEOBJECT ID CONSTRUCTED: " << id);
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

Component* GameObject::addComponent(ComponentType componentType)
{
    Component* component = nullptr;
    switch (componentType)
    {
    case ComponentType::Animator:
        component = new AnimatorComponent(*this);
        PRINT("ADDED ANIMATOR");
        break;
    case ComponentType::BoxCollider2D:
        component = new BoxCollider2D(*this);
        PRINT("ADDED COLLIDER");
        break;
    case ComponentType::Rigidbody2D:
        component = new Rigidbody2D(*this);
        PRINT("ADDED Rigidbody");
        break;
    case ComponentType::SpriteRenderer:
        component = new SpriteRenderer(*this);
        PRINT("ADDED SPRITE RENDERER");
        break;
    case ComponentType::Script:
        component = new ScriptComponent(*this);
        PRINT("ADDED SCRIPT");
        break;
    case ComponentType::Button:
        if (hasComponent(componentType))
        {
            break;
        }
        if (!hasComponent(ComponentType::Image))
            addComponent<ImageComponent>();
        if (!hasComponent(ComponentType::Text))
            addComponent<TextComponent>();
        component = new ButtonComponent(*this);
        PRINT("ADDED UI BUTTON");
        break;
    case ComponentType::Image:
        component = new ImageComponent(*this);
        PRINT("ADDED UI IMAGE");
        break;
    case ComponentType::Text:
        component = new TextComponent(*this);
        PRINT("ADDED UI TEXT");
        break;
    default:
        PRINT("ADDED NOTHING");
        break;
    }
    if (component)
        components.push_back(component);
    return component;
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

Transform& GameObject::Transform()  
{
    return transform;
}

void GameObject::set_name(const std::string& _name){ name = _name; }
std::string GameObject::get_name() const{ return name; }

void GameObject::set_ppid(GameObjectID& _id) { parentid = _id; }
GameObjectID GameObject::get_ppid() const { return parentid; }

bool GameObject::is_parent() const 
{
    if (children.size())
        return true;
    else
        return false;
}
bool GameObject::has_parent() const
{
    if (parent)
        return true;
    else
        return false;
}
GameObject* GameObject::get_parent() { return parent; }
void GameObject::set_parent(GameObject* _parent) { parent = _parent; }

const std::list<GameObject*>& GameObject::childList() const
{
    return children;
}

std::list<GameObject*>& GameObject::mchildList()
{
    return children;
}

bool GameObject::attach_child(GameObject* _child)
{
    if (!_child)
        return false;

    children.push_back(_child);
    _child->set_parent(this);
    _child->parentid = id;
    std::cout << "child attached\n";

    return true;

}
bool GameObject::deattach_child(GameObject* _child)
{
    for (std::list<GameObject*>::iterator iter = children.begin(); iter != children.end(); ++iter)
    {
        if (*iter == _child)
        {
            _child->parent = nullptr;
            _child->parentid = 0;
            children.erase(iter);
            return true;
        }
    }

    return false;
}

bool GameObject::deserialize(rapidjson::Value& _value) {
    if (!_value.HasMember("ID"))
        return false;

    if (!_value.HasMember("Name"))
        return false;
   
    name = _value["Name"].GetString();

    if (!_value.HasMember("PID"))
        return false;

    parentid = _value["PID"].GetInt();

    return true;
}


void GameObject::handleMessage(MESSAGE_TYPE mType)
{
    using namespace Copium;
    //MT_SCRIPTING_UPDATED
    MESSAGE_CONTAINER::reflectCsGameObject.ID = id;
    messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
}


void GameObject::inspectorView()
{
    static char buffer[256];
    strcpy(buffer, name.c_str());
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##gameObjName", buffer,256);
    ImGui::PopItemWidth();
    name = buffer;
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH
        | ImGuiTableFlags_ScrollY;
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
    if (ImGui::CollapsingHeader("Transform", nodeFlags))
    {
        transform.inspector_view();     
    }
    if (ImGui::BeginTable("Components", 1, tableFlags, ImVec2(0.f, ImGui::GetWindowSize().y/2.f)))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        static std::vector<ComponentID> componentsToDelete;
        componentsToDelete.clear();
        int index = 0;
        for (Component* component : components)
        {
            const std::string& componentName{ component->Name() };
            ImGui::PushID(index);
            if (ImGui::CollapsingHeader(componentName.c_str(), nodeFlags))
            {
                component->inspector_view();
                if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x, 0.f)))
                {
                    PRINT("ID: " << component->id);
                    componentsToDelete.push_back(component->id);
                }
            }
            ImGui::PopID();
            ++index;
            ImGui::TableNextColumn();
        }
        for (ComponentID componentId : componentsToDelete)
        {
            removeComponent(componentId);
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}


// M2
bool GameObject::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
{

    _value.AddMember("ID", id, _doc.GetAllocator());

    rapidjson::Value _name;
    _name.SetString(name.c_str(), rapidjson::SizeType(name.length()), _doc.GetAllocator());
    _value.AddMember("Name", _name, _doc.GetAllocator());

    _value.AddMember("PID", parentid, _doc.GetAllocator());

    rapidjson::Value _components(rapidjson::kArrayType);
    rapidjson::Value transformComponent(rapidjson::kObjectType);
    transform.serialize(transformComponent, _doc);
    _components.PushBack(transformComponent, _doc.GetAllocator());
    for (std::list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
    {
        rapidjson::Value comp(rapidjson::kObjectType);
        // Serialize each component
        (*iter)->serialize(comp, _doc);
        _components.PushBack(comp, _doc.GetAllocator());
    }
    _value.AddMember("Components", _components, _doc.GetAllocator());
    

    //Recursively serialize children and their children and so on
    rapidjson::Value _children(rapidjson::kArrayType);
    for (std::list<GameObject*>::const_iterator cit = childList().cbegin(); cit != childList().cend(); ++cit)
    {
        rapidjson::Value cgo(rapidjson::kObjectType);
        (*cit)->serialize(cgo, _doc);
        _children.PushBack(cgo, _doc.GetAllocator());
    }
    _value.AddMember("Children", _children, _doc.GetAllocator());
    return true;

}


}