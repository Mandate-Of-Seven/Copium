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

std::map<ComponentType, const std::string> Component::componentMap
{
    {ComponentType::Animator,"AnimatorComponent"},
    {ComponentType::Collider,"ColliderComponent"},
    {ComponentType::Renderer,"RendererComponent"},
    {ComponentType::Script,"ScriptComponent"},
    {ComponentType::Transform, "TransformComponent"}
};

Component::Component::Component(GameObject& _gameObj, ComponentType _componentType) 
    : gameObj { _gameObj }, componentType{_componentType} {}


void Component::destroy() {}

ComponentID const Component::ID() { return id; }

void Component::deserialize(rapidjson::Value& _value)
{
    //std::cout << "default deserialization\n";
}

bool Component::Enabled() const noexcept{ return enabled;}

void Component::Enabled(bool _enabled) noexcept { enabled = _enabled; }

ColliderComponent::ColliderComponent(GameObject& _gameObj) 
    :Component(_gameObj, ComponentType::Collider) { std::cout << "COLLIDER CONS" << std::endl; }

AnimatorComponent::AnimatorComponent(GameObject& _gameObj) 
    :Component(_gameObj, ComponentType::Animator) { std::cout << "ANIMATOR CONS" << std::endl; }

const std::string& Component::Name() const
{
    return componentMap[componentType];
}


Component& Component::operator=(const Component& rhs)
{
    enabled = rhs.enabled;
    COPIUM_ASSERT(componentType == rhs.componentType, "TRYING TO COPY ASSIGN TWO DIFFERENT COMPONENT TYPES!");
    switch (componentType)
    {
    case ComponentType::Animator:
    {
        auto pRhs = reinterpret_cast<const AnimatorComponent*>(&rhs);
        auto pLhs = reinterpret_cast<AnimatorComponent*>(this);
        PRINT("ADDED ANIMATOR");
        break;
    }
    case ComponentType::Collider:
    {
        auto pRhs = reinterpret_cast<const ColliderComponent*>(&rhs);
        auto pLhs = reinterpret_cast<ColliderComponent*>(this);
        PRINT("ADDED COLLIDER");
        break;
    }
    case ComponentType::Renderer:
    {
        auto pRhs = reinterpret_cast<const Copium::RendererComponent*>(&rhs);
        auto pLhs = reinterpret_cast<Copium::RendererComponent*>(this);
        PRINT("ADDED SPRITE RENDERER");
        break;
    }
    case ComponentType::Script:
    {
        //MESSAGE_CONTAINERS::addScript.name = "NewScript";
        //MESSAGE_CONTAINERS::addScript.gameObj = this;
        PRINT("ADDED SCRIPT");
        break;
    }
    case ComponentType::Transform:
    {
        auto* pRhs = reinterpret_cast<const TransformComponent*>(&rhs);
        auto* pLhs = reinterpret_cast<TransformComponent*>(this);
        pLhs->position = pRhs->position;
        pLhs->rotation = pRhs->rotation;
        pLhs->scale = pRhs->scale;
        break;
    }
    default:
        PRINT("ADDED NOTHING");
        break;
    }
    return *this;
}

