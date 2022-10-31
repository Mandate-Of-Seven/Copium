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

Component::Component(GameObject& _gameObj, ComponentType _componentType) 
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
    return MAP_COMPONENT_TYPE_NAME[componentType];
}


Component& Component::operator=(const Component& rhs)
{
    enabled = rhs.enabled;
    COPIUM_ASSERT(componentType == rhs.componentType, "TRYING TO COPY ASSIGN TWO DIFFERENT COMPONENT TYPES!");
    switch (componentType)
    {
    case ComponentType::Animator:
    {
        const AnimatorComponent* pRhs = reinterpret_cast<const AnimatorComponent*>(&rhs);
        AnimatorComponent* pLhs = reinterpret_cast<AnimatorComponent*>(this);
        break;
    }
    case ComponentType::Collider:
    {
        const ColliderComponent* pRhs = reinterpret_cast<const ColliderComponent*>(&rhs);
        ColliderComponent* pLhs = reinterpret_cast<ColliderComponent*>(this);
        break;
    }
    case ComponentType::Renderer:
    {
        const Copium::RendererComponent* pRhs = reinterpret_cast<const Copium::RendererComponent*>(&rhs);
        Copium::RendererComponent* pLhs = reinterpret_cast<Copium::RendererComponent*>(this);
        break;
    }
    case ComponentType::Script:
    {
        break;
    }
    case ComponentType::Transform:
    {
        const TransformComponent* pRhs = reinterpret_cast<const TransformComponent*>(&rhs);
        TransformComponent* pLhs = reinterpret_cast<TransformComponent*>(this);
        pLhs->position = pRhs->position;
        pLhs->rotation = pRhs->rotation;
        pLhs->scale = pRhs->scale;
        break;
    }
    default:
        break;
    }
    return *this;
}

