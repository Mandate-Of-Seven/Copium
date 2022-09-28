/*!***************************************************************************************
\file			game-object.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Defines GameObject Class and member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


//INCLUDES

#include "pch.h"
#include "game-object.h"
#include "scripting.h"
#include "message-system.h"

//USING

namespace 
{
    const std::string defaultGameObjName = "New GameObject"; // Append (No.) if its not the first
}


GameObject::GameObject()
    : name{ defaultGameObjName }, id{0}, parent{nullptr}
{

}

GameObject::~GameObject()
{
    for (Component* pComponent: components)
    {
        delete pComponent;
    }
    components.clear();
    children.clear();
}

std::list<Component*>& GameObject::Components()
{
    return components;
}

GameObject::GameObject
(Vector3 _position, Vector3 _rotation = { 0,0,0 }, Vector3 _scale = { 1,1,1 }) 
    : name{ defaultGameObjName }, id{0}, trans(_position, _rotation, _scale), parent{nullptr}
{

}

void GameObject::addComponent(Component* component)
{
    components.push_back(component);
}


void GameObject::addComponent(Component::Type componentType)
{
    switch (componentType)
    {
    case Component::Animator:
        components.push_back(new AnimatorComponent());
        PRINT("ADDED ANIMATOR");
        break;
    case Component::Collider:
        components.push_back(new ColliderComponent());
        PRINT("ADDED COLLIDER");
        break;
    case Component::Renderer:
        components.push_back(new RendererComponent());
        PRINT("ADDED RENDERER");
        break;
    case Component::Script:
        using namespace Copium::Message;
        MessageSystem::Instance()->
            dispatch(MESSAGE_TYPE::MT_ADD_SCRIPT);
        MESSAGE_CONTAINERS::addScript.name = "NewScript";
        MESSAGE_CONTAINERS::addScript.gameObj = this;
        PRINT("ADDED SCRIPT");
        break;
    default:
        PRINT("ADDED NOTHING");
        break;
    }
}

void GameObject::deleteComponent(Component* component)
{
    std::list<Component*>::iterator it{ components.begin() };
    ComponentID id{};
    while (id != component->ID())
    {
        ++id; ++it;
    }
    //std::cout << "Id:" << id << std::endl;
    components.erase(it);
}

void GameObject::Trans(Transform _trans) {trans = _trans;}

Transform const GameObject::Trans(){return trans;}

void GameObject::set_name(const std::string& _name){ name = _name; }
std::string& GameObject::get_name(){ return name; }

void GameObject::set_id(GameObjectID& _id) { id = _id; }
GameObjectID GameObject::get_id() const { return id; }

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

std::list<GameObject*>& GameObject::childList()
{
    return children;
}

bool GameObject::attach_child(GameObject* _child)
{
    if (!_child)
        return false;

    children.push_back(_child);
    _child->parent = this;
    return true;

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

