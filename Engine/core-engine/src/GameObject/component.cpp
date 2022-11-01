/*!***************************************************************************************
\file			component.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Defines Component Class to be added to GameObjects and its member functions

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include <rapidjson/document.h>
#include "GameObject/component.h"
#include "renderer-component.h"
#include "transform-component.h"

namespace Copium
{

Component::Component(GameObject& _gameObj, ComponentType _componentType) 
    : gameObj{ _gameObj }, componentType{ _componentType }, enabled{true} {}

void Component::destroy() {}

ComponentID const Component::ID() { return id; }

void Component::deserialize(rapidjson::Value& _value)
{
    //std::cout << "default deserialization\n";
}
void Component::serialize(rapidjson::Value& _value, rapidjson::Document& _doc) {

}


bool Component::Enabled() const noexcept{ return enabled;}

void Component::Enabled(bool _enabled) noexcept { enabled = _enabled; }

ColliderComponent::ColliderComponent(GameObject& _gameObj) 
    :Component(_gameObj, ComponentType::Collider) { std::cout << "COLLIDER CONS" << std::endl; }

AnimatorComponent::AnimatorComponent(GameObject& _gameObj) 
    :Component(_gameObj, ComponentType::Animator) { std::cout << "ANIMATOR CONS" << std::endl; }

const std::string& Component::Name() const
{
    return MAP_COMPONENT_TYPE_NAME[componentType];
}
}