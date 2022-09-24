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
    //components.clear();
}

std::list<Component*>& GameObject::Components()
{
    return components;
}

GameObject::GameObject
(Copium::Math::Vec3 _position, Copium::Math::Vec3 _rotation = { 0,0,0 }, Copium::Math::Vec3 _scale = { 1,1,1 })
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
        break;
    case Component::Collider:
        components.push_back(new ColliderComponent());
        break;
    case Component::Renderer:
        components.push_back(new RendererComponent());
        break;
    //case Component::Script:
    //    components.push_back(new ScriptComponent());
        //break;
    default:
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

Transform const GameObject::Trans() const {return trans;}

void GameObject::set_name(const std::string& _name){ name = _name; }
std::string GameObject::get_name() const{ return name; }

void GameObject::set_id(GameObjectID& _id) { id = _id; }
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

    rapidjson::Value& _t = _value["Transform"].GetObject();
    trans.deserialize(_t);


    //std::cout << trans.Position() << '|' << trans.Rotation() << '|' << trans.Scale() << std::endl;



    return true;
}

