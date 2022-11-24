/*!***************************************************************************************
\file			transform.cpp
\project
\author			Zacharie Hong
\co-authors		Matthew Lau
                Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
	Defines Transform Class to be added to GameObjects and its member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


//INCLUDES

#include "pch.h"
#include "GameObject\Components\transform-component.h"
#include <rttr/registration>
#include "Editor/editor-undoredo.h"
#include "Editor/editor-system.h"
#include "SceneManager/sm.h"

//RTTR_REGISTRATION{
//	using namespace rttr;
//registration::class_<Transform>("Transform")
//.property("Pos", &Transform::get_position, &Transform::set_position)
//.property("Rot", &Transform::get_rotation, &Transform::set_rotation)
//.property("Scale", &Transform::get_scale, &Transform::set_scale);
//
//}

namespace Copium
{


Transform::Transform
	(GameObject& _gameObj,Math::Vec3 _position, Math::Vec3 _rotation, Math::Vec3 _scale)
	:Component(_gameObj, ComponentType::Transform),
	position {_position}, rotation{ _rotation }, scale{ _scale }, parent{ nullptr }{}

glm::vec3 Transform::glmPosition() const { return position.to_glm(); }
void Transform::set_position(const Math::Vec3& _position) { position = _position; }

const Math::Vec3& Transform::get_rotation() { return rotation; }
glm::vec3 Transform::glmRotation() const { return rotation.to_glm(); }
void Transform::set_rotation(const Math::Vec3& _rotation) { rotation = _rotation; }

const Math::Vec3& Transform::get_scale() { return scale; }
glm::vec3 Transform::glmScale() const { return scale.to_glm(); }
void Transform::set_scale(const Math::Vec3& _scale) { scale = _scale; }

float temp;

void Transform::deserialize(rapidjson::Value& _value)
{
    std::cout << "Deserializing transform\n";
    Component::deserialize(_value);
	rapidjson::Value& _v = _value["Pos"].GetObj();
	position.deserialize(_v);
	_v = _value["Rot"].GetObj();
	rotation.deserialize(_v);
	_v = _value["Scale"].GetObj();
	scale.deserialize(_v);
}

// M2
void Transform::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
{
    Component::serialize(_value, _doc);

    rapidjson::Value _pos(rapidjson::kObjectType);
    rapidjson::Value _rot(rapidjson::kObjectType);
    rapidjson::Value _scale(rapidjson::kObjectType);

    rapidjson::Value type;
    std::string tc = "Transform";
    type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
    _value.AddMember("Type", type, _doc.GetAllocator()); 

    position.serialize(_pos, _doc);
    rotation.serialize(_rot, _doc);
    scale.serialize(_scale, _doc);

    _value.AddMember("Pos", _pos, _doc.GetAllocator());
    _value.AddMember("Rot", _rot, _doc.GetAllocator());
    _value.AddMember("Scale", _scale, _doc.GetAllocator());

}


void Transform::inspector_view()
{
    ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp
        | ImGuiTableFlags_PadOuterX;
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 2));
    if (ImGui::BeginTable("Component Transform", 2, windowFlags))
    {
        windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
        
        ImGui::Indent();

        ImGui::TableSetupColumn("Text", 0, 0.4f);
        ImGui::TableSetupColumn("Input", 0, 0.6f);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 0));

        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Position");
        ImGui::TableNextColumn();

        if (ImGui::BeginTable("Component Transform: Position", 3, windowFlags))
        {
            ImGui::TableNextColumn();
            ImGui::PushID(0);
            ImGui::AlignTextToFramePadding();
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
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&position.x, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
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
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&position.y, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
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
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&position.z, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = position.z;
                }
            }
            ImGui::PopID();

            ImGui::EndTable();
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Rotation");
        ImGui::TableNextColumn();
        if (ImGui::BeginTable("Component Transform: Rotation", 3, windowFlags))
        {
            ImGui::TableNextColumn();
            ImGui::PushID(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &rotation.x);
            if (ImGui::IsItemActivated())
            {
                temp = rotation.x;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != rotation.x)
                {
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&rotation.x, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = rotation.x;
                }
            }
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &rotation.y);
            if (ImGui::IsItemActivated())
            {
                temp = rotation.y;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != rotation.y)
                {
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&rotation.y, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = rotation.y;
                }
            }
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &rotation.z);
            if (ImGui::IsItemActivated())
            {
                temp = rotation.z;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != rotation.z)
                {
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&rotation.z, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = rotation.z;
                }
            }
            ImGui::PopID();

            ImGui::EndTable();
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Scale");
        ImGui::TableNextColumn();
        if (ImGui::BeginTable("Component Transform: Scale", 3, windowFlags))
        {
            ImGui::TableNextColumn();
            ImGui::PushID(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("X"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &scale.x);
            if (ImGui::IsItemActivated())
            {
                temp = scale.x;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != scale.x)
                {
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&scale.x, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = scale.x;
                }
            }
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(1);
            ImGui::Text("Y"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &scale.y);
            if (ImGui::IsItemActivated())
            {
                temp = scale.y;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != scale.y)
                {
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&scale.y, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = scale.y;
                }
            }
            ImGui::PopID();

            ImGui::TableNextColumn();
            ImGui::PushID(2);
            ImGui::Text("Z"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("", &scale.z);
            if (ImGui::IsItemActivated())
            {
                temp = scale.z;
            }
            if (ImGui::IsItemEdited())
            {
                printf("temp: %f\n", temp);
                if (temp != scale.z)
                {
                    UndoRedo::Command* tempUndo = new UndoRedo::TransformCommand(&scale.z, temp);
                    EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
                    temp = scale.z;
                }
            }
            ImGui::PopID();

            ImGui::EndTable();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::Unindent();
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
   
}
}