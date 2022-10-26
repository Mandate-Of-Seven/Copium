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
#include "GameObject/component.h"
#include "GameObject/renderer-component.h"
#include "Editor/inspector.h"
#include "Math/math-library.h"

std::map<Component::Type, const std::string> Component::componentMap
{
    {Type::Animator,"Animator"},
    {Type::Collider,"Collider"},
    {Type::SpriteRenderer,"Sprite Renderer"},
    {Type::Script,"NewScript"},
    {Type::Transform, "Transform"}
};

Component::Component() {}

Component::Component(Component::Type _componentType) : componentType(_componentType) {}

void Component::destroy() {}

ComponentID const Component::ID() { return id; }

Component::Type Component::get_type() { return componentType; }

void Component::deserialize(rapidjson::Value& _value)
{
    std::cout << "default deserialization\n";
}

bool Component::Enabled() const noexcept{ return enabled;}

void Component::Enabled(bool _enabled) noexcept { enabled = _enabled; }

ColliderComponent::ColliderComponent() :Component(Type::Collider)
{
    PRINT("Collider Component constructed");
}

AnimatorComponent::AnimatorComponent() :Component(Type::Animator) 
{ 
    PRINT("Animator Component constructed");
}

TransformComponent::TransformComponent() :Component(Type::Transform)
{ 
    PRINT("Transform Component constructed");
}

void TransformComponent::deserialize(rapidjson::Value& _value)
{
    t.deserialize(_value);
}

void TransformComponent::inspector_view(GameObject& _gameObject)
{
    Transform trans{ _gameObject.Trans()->get_transform()};
    Copium::Math::Vec3 position = trans.get_position();
    Copium::Math::Vec3 rotation = trans.get_rotation();
    Copium::Math::Vec3 scale = trans.get_scale();

    ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
    if (ImGui::BeginTable("Component Transform", 2, windowFlags))
    {
        ImGui::Indent();

        ImGui::TableSetupColumn("Text", 0, 0.4f);
        ImGui::TableSetupColumn("Input", 0, 0.6f);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Position");
        ImGui::TableNextColumn();
        if (ImGui::BeginTable("Component Transform: Position", 3, windowFlags))
        {
            ImGui::TableNextColumn();
            ImGui::PushID(0);
            ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &position.x);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &position.y);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &position.z);
            ImGui::PopID();

            ImGui::EndTable();
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Rotation");
        ImGui::TableNextColumn();
        if (ImGui::BeginTable("Component Transform: Rotation", 3, windowFlags))
        {
            ImGui::TableNextColumn();
            ImGui::PushID(0);
            ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &rotation.x);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &rotation.y);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &rotation.z);
            ImGui::PopID();

            ImGui::EndTable();
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Scale");
        ImGui::TableNextColumn();
        if (ImGui::BeginTable("Component Transform: Scale", 3, windowFlags))
        {
            ImGui::TableNextColumn(); 
            ImGui::PushID(0);
            ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &scale.x);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &scale.y);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputDouble("", &scale.z);
            ImGui::PopID();

            ImGui::EndTable();
        }
        
        ImGui::Unindent();
        ImGui::EndTable();
    }
    
    trans.set_position(position);
    trans.set_rotation(rotation);
    trans.set_scale(scale);
    _gameObject.Trans()->set_transform(trans);
}

const std::string& Component::Name()
{
    return componentMap[componentType];
}


