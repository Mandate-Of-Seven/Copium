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
#include "GameObject/component.h"

std::map<Component::Type, const std::string> Component::componentMap
{
    {Type::Animator,"Animator"},
    {Type::Collider,"Collider"},
    {Type::Renderer,"Renderer"},
    {Type::Script,"NewScript"},
    {Type::Transform, "Transform"}
};

Component::Component::Component(GameObject& _gameObj, Component::Type _componentType) 
    : gameObj { _gameObj }, componentType{_componentType} {}

void Component::destroy() {}

ComponentID const Component::ID() { return id; }

void Component::deserialize(rapidjson::Value& _value)
{
    std::cout << "default deserialization\n";
}

bool Component::Enabled() const noexcept{ return enabled;}

void Component::Enabled(bool _enabled) noexcept { enabled = _enabled; }




ColliderComponent::ColliderComponent(GameObject& _gameObj) 
    :Component(_gameObj,Type::Collider) { std::cout << "COLLIDER CONS" << std::endl; }

RendererComponent::RendererComponent(GameObject& _gameObj) 
    :Component(_gameObj,Type::Renderer) { std::cout << "RENDERER CONS" << std::endl; }

AnimatorComponent::AnimatorComponent(GameObject& _gameObj) 
    :Component(_gameObj,Type::Animator) { std::cout << "ANIMATOR CONS" << std::endl; }

TransformComponent::TransformComponent(GameObject& _gameObj) 
    :Component(_gameObj,Type::Transform)
{ 
    std::cout << "TRANSFORM CONS" << std::endl; 
}
void TransformComponent::deserialize(rapidjson::Value& _value)
{
    t.deserialize(_value);
}

const std::string& Component::Name()
{
    return componentMap[componentType];
}


