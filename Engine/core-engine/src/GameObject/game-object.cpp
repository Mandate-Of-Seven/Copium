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

namespace Copium
{
    GameObject::~GameObject()
    {
        std::cout << "GameObject destructed\n";
    }


    bool GameObject::IsActive()
    {
        if (!active)
            return false;
        Transform* parent = transform.parent;
        while (parent)
        {
            if (!parent->gameObject.active)
                return false;
            parent = parent->parent;
        }
        return true;
    }

    void GameObject::SetActive(bool _active)
    {
        active = _active;
    }


    GameObject::GameObject
    (UUID _uuid, Math::Vec3 _position, Math::Vec3 _rotation, Math::Vec3 _scale )
        :
        name{ defaultGameObjName },
        transform(*this),
        active{true},
        uuid{ _uuid }
    {
        transform.position = _position;
        transform.rotation = _rotation;
        transform.scale = _scale;
    }

    bool GameObject::HasComponent(ComponentType componentType)
    {
        HasComponentType hasComponent{ *this };
        return hasComponent.HasComponent(componentType);
    }

    std::ostream& operator<<(std::ostream& _os, const GameObject& _go) 
    {
        //_os << "-----\n";
        //_os << "Name:" << _go.name << '\n'
        //    << "UID:" << _go.uuid << '\n';
        //if (_go.transform.HasParent())
        //    _os << "Parent's UID:" << _go.transform.parent->gameObj.uuid << '\n';

        //int i{ 0 };
        //for (Component* co : _go.components)
        //{
        //    _os << "Component " << i << " UID: " << co->uuid << '\n';
        //    ++i;
        //}

        //_os << "-----";

        //return _os;
        return _os;
    }


}