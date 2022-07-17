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

#include "game-object.h"

//USING

namespace 
{
    const std::string defaultGameObjName = "New GameObject"; // Append (No.) if its not the first
}


GameObject::GameObject()
    : name{ defaultGameObjName }, id{0}
{

}

GameObject::GameObject
(Vector2 _position, Vector2 _rotation = { 0,0 }, Vector2 _scale = { 1,1 }) 
    : name{ defaultGameObjName }
{
}

void GameObject::addComponent(Component* component)
{
    components.push_back(component);
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