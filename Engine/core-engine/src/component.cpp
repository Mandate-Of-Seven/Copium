/*!***************************************************************************************
\file			component.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Defines Component Class to be added to GameObjects and its member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "component.h"


std::map<Component::Type, const std::string> Component::componentMap
{
    {Type::Animator,"Animator"},
    {Type::Collider,"Collider"},
    {Type::Renderer,"Renderer"},
    {Type::Script,"Script"}
};

Component::Component(Component::Type _componentType) : componentType(_componentType), name{ componentMap[_componentType]} {}

void Component::destroy(){}


ComponentID const Component::ID(){return id;}

const std::string Component::Name() { return name; }

ColliderComponent::ColliderComponent() :Component(Type::Collider) { std::cout << "NAME: " << name << std::endl; }

RendererComponent::RendererComponent() :Component(Type::Renderer) { std::cout << "NAME: " << name << std::endl; }

AnimatorComponent::AnimatorComponent() :Component(Type::Animator) { std::cout << "NAME: " << name << std::endl; }

ScriptComponent::ScriptComponent() :Component(Type::Script) { std::cout << "NAME: " << name << std::endl; }