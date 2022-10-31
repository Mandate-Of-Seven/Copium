/*!***************************************************************************************
\file			transform.cpp
\project
\author			Zacharie Hong
\co-authors		Matthew Lau

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
	Defines Transform Class to be added to GameObjects and its member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


//INCLUDES

#include "pch.h"
#include "GameObject\transform-component.h"
#include <rttr/registration>
#include "Editor/editor-undoredo.h"
#include "SceneManager/sm.h"

//RTTR_REGISTRATION{
//	using namespace rttr;
//registration::class_<TransformComponent>("Transform")
//.property("Pos", &TransformComponent::get_position, &TransformComponent::set_position)
//.property("Rot", &TransformComponent::get_rotation, &TransformComponent::set_rotation)
//.property("Scale", &TransformComponent::get_scale, &TransformComponent::set_scale);
//
//}

TransformComponent::TransformComponent
	(GameObject& _gameObj,Copium::Math::Vec3 _position, Copium::Math::Vec3 _rotation, Copium::Math::Vec3 _scale)
	:Component(_gameObj, ComponentType::Transform),
	position {_position}, rotation{ _rotation }, scale{ _scale }, parent{ nullptr }{}

glm::dvec3 TransformComponent::glmPosition() const { return position.to_glm(); }
void TransformComponent::set_position(const Copium::Math::Vec3& _position) { position = _position; }

const Copium::Math::Vec3& TransformComponent::get_rotation() { return rotation; }
glm::dvec3 TransformComponent::glmRotation() const { return rotation.to_glm(); }
void TransformComponent::set_rotation(const Copium::Math::Vec3& _rotation) { rotation = _rotation; }

const Copium::Math::Vec3& TransformComponent::get_scale() { return scale; }
glm::dvec3 TransformComponent::glmScale() const { return scale.to_glm(); }
void TransformComponent::set_scale(const Copium::Math::Vec3& _scale) { scale = _scale; }

float temp;

void TransformComponent::deserialize(rapidjson::Value& _value)
{

	rapidjson::Value& _v = _value["Pos"].GetObj();
	position.deserialize(_v);
	std::cout << "Position:" << position;
	_v = _value["Rot"].GetObj();
	rotation.deserialize(_v);
	std::cout << "Rotation:" << rotation;
	_v = _value["Scale"].GetObj();
	scale.deserialize(_v);
	std::cout << "Scale:" << scale;
}

void TransformComponent::inspector_view()
{
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
            ImGui::InputFloat("", &position.x);
            if (ImGui::IsItemActivated())
            {
                temp = position.x;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n",temp);
                if (temp!=position.x)
                {
                    Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::TransformCommand(&position.x, temp);
                    Copium::NewSceneManager::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = position.x;
                }
            }
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &position.y);
            if (ImGui::IsItemActivated())
            {
                temp = position.y;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != position.y)
                {
                    Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::TransformCommand(&position.y, temp);
                    Copium::NewSceneManager::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = position.y;
                }
            }
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &position.z);
            if (ImGui::IsItemActivated())
            {
                temp = position.z;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != position.z)
                {
                    Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::TransformCommand(&position.z, temp);
                    Copium::NewSceneManager::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = position.z;
                }
            }
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
            ImGui::InputFloat("", &rotation.x);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &rotation.y);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &rotation.z);
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
            ImGui::InputFloat("", &scale.x);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &scale.y);
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &scale.z);
            ImGui::PopID();

            ImGui::EndTable();
        }

        ImGui::Unindent();
        ImGui::EndTable();
    }


}