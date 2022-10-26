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
#include "GameObject/renderer-component.h"
#include "GameObject/component.h"

//USING

namespace 
{
    const std::string defaultGameObjName = "New GameObject"; // Append (No.) if its not the first
    Copium::MessageSystem& messageSystem{*Copium::MessageSystem::Instance()};
}


GameObjectID GameObject::count = 1;

GameObject::~GameObject()
{
    for (std::list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
    {
        if (*iter)
        {
            delete* iter;

        }
    }
}



GameObject::GameObject
(Copium::Math::Vec3 _position, Copium::Math::Vec3 _rotation, Copium::Math::Vec3 _scale)
    : 
    name{ defaultGameObjName }, parent{nullptr}, parentid{0}, 
    transform(*this,_position, _rotation, _scale)
{
    id = count++;
    messageSystem.subscribe(Copium::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
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

Component* GameObject::addComponent(const Component& component)
{
    Component* tmp = addComponent(component.componentType);
    *tmp = component;
    return tmp;
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
    case ComponentType::Collider:
        component = new ColliderComponent(*this);
        PRINT("ADDED COLLIDER");
        break;
    case ComponentType::Renderer:
        component = new Copium::RendererComponent(*this);
        PRINT("ADDED SPRITE RENDERER");
        break;
    case ComponentType::Script:
        //MESSAGE_CONTAINERS::addScript.name = "NewScript";
        //MESSAGE_CONTAINERS::addScript.gameObj = this;
        PRINT("ADDED SCRIPT");
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
    PRINT("Component of Type " << Component::componentMap[componentType] << " does not exist on " << name);
}

void GameObject::removeComponent(ComponentID componentID)
{
    auto it{ components.begin() };
    while (it != components.end())
    {
        if ((*it)->ID() == id)
        {
            delete* it;
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

TransformComponent& GameObject::Transform()  
{
    return transform;
}

void GameObject::set_name(const std::string& _name){ name = _name; }
std::string GameObject::get_name() const{ return name; }

//void GameObject::set_id(GameObjectID& _id) { id = _id; }
GameObjectID GameObject::get_id() const { return id; }

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
    
     id = _value["ID"].GetInt();

    if (!_value.HasMember("Name"))
        return false;
   
    name = _value["Name"].GetString();

    if (!_value.HasMember("PID"))
        return false;

    parentid = _value["PID"].GetInt();

    return true;
}


void GameObject::handleMessage(Copium::MESSAGE_TYPE mType)
{
    using namespace Copium;
    //MT_SCRIPTING_UPDATED
    MESSAGE_CONTAINER::reflectCsGameObject.ID = id;
    messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
}


GameObject& GameObject::operator=(const GameObject& rhs)
{
    transform = rhs.transform;
    for (Component* component : components)
    {
        delete component;
    }
    components.clear();
    for (const Component* component: rhs.components)
    {
        addComponent(*component);
    }
    return *this;
}


void GameObject::inspectorView()
{
    transform.inspector_view();
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH
        | ImGuiTableFlags_ScrollY;
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
    if (ImGui::BeginTable("Components", 1, tableFlags, ImVec2(0.f, 450.f)))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
        for (Component* component : components)
        {

            if (ImGui::CollapsingHeader(component->Name().c_str(), nodeFlags))
            {
                component->inspector_view();
            }
            ImGui::TableNextColumn();
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}