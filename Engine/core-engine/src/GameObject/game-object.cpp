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

//USING

namespace 
{
    const std::string defaultGameObjName = "New GameObject"; // Append (No.) if its not the first
    Copium::Message::MessageSystem& messageSystem{*Copium::Message::MessageSystem::Instance()};
}


GameObjectID GameObject::count = 1;

GameObject::GameObject()
    : name{ defaultGameObjName }, parent{nullptr}, parentid{0}
{
    id = count++;
    messageSystem.subscribe(Copium::Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
}

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

std::list<Component*>& GameObject::Components()
{
    return components;
}

GameObject::GameObject
(Copium::Math::Vec3 _position, Copium::Math::Vec3 _rotation = { 0,0,0 }, Copium::Math::Vec3 _scale = { 1,1,1 })
    : name{ defaultGameObjName }, trans(_position, _rotation, _scale), parent{nullptr}, parentid{0}
{
    id = count++;
}

void GameObject::addComponent(Component* component)
{
    components.push_back(component);
}


void GameObject::addComponent(Component::Type componentType)
{
    switch (componentType)
    {
    case Component::Type::Animator:
        components.push_back(new AnimatorComponent(*this));
        PRINT("ADDED ANIMATOR");
        break;
    case Component::Type::Collider:
        components.push_back(new ColliderComponent(*this));
        PRINT("ADDED COLLIDER");
        break;
    case Component::Type::Renderer:
        components.push_back(new Copium::RendererComponent());
        PRINT("ADDED RENDERER");
        break;
    case Component::Type::Script:
        using namespace Copium::Message;
        //MESSAGE_CONTAINERS::addScript.name = "NewScript";
        //MESSAGE_CONTAINERS::addScript.gameObj = this;
        PRINT("ADDED SCRIPT");
        break;
    case Component::Type::Transform:
        components.push_back(new TransformComponent(*this));
        break;
    default:
        PRINT("ADDED NOTHING");
        break;
    }
}

void GameObject::deleteComponent(Component* component)
{
    std::list<Component*>::iterator it{ components.begin() };
    ComponentID _id{};
    while (_id != component->ID())
    {
        ++_id; ++it;
    }
    //std::cout << "Id:" << id << std::endl;
    components.erase(it);
}

void GameObject::Trans(Transform _trans) {trans = _trans;}

TransformComponent* GameObject::Trans()  
{
    for (std::list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
    {
        Component::Type tmp =   (*iter)->get_type();
        if ((*iter)->componentMap.find(tmp) == (*iter)->componentMap.end())
        {
            std::cout << "cannot find component type\n";
            continue;
        }

        std::cout << "Component Type:" << (*iter)->componentMap[tmp] << std::endl;

        if (tmp == Component::Type::Transform)
            return reinterpret_cast<TransformComponent*>(*iter);

    }

    return nullptr;

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


void GameObject::handleMessage(Copium::Message::MESSAGE_TYPE mType)
{
    PRINT("HANDLE ID: " << id);
    using namespace Copium::Message;
    //MT_SCRIPTING_UPDATED
    MESSAGE_CONTAINER::reflectCsGameObject.ID = id;
    messageSystem.dispatch(MESSAGE_TYPE::MT_REFLECT_CS_GAMEOBJECT);
}

