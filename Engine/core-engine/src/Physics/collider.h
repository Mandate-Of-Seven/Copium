/*!***************************************************************************************
\file			collider.h
\project
\author			Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			24/09/2022

\brief
	// This file contains the collider component. It will store all the necessary values
	to perform physics on an object.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "Physics/collision.h"
#include "Math/math-library.h"
#include "GameObject/component.h"
#include <rapidjson/document.h>

namespace Copium
{
    class RigidBodyComponent : public Component
    {
	public:
		void const set_vel(Math::Vec2 _velocity) { velocity = _velocity; }
		void const set_acc(Math::Vec2 _acceleration) { acceleration = _acceleration; }
		void const set_shape(Shape _objectShape) { objectShape = _objectShape; }
		void const set_AABB(Math::Vec2 _min, Math::Vec2 _max) { boundingBox.max = _max; boundingBox.min = _min;}
		void const set_active(bool _active) { active = _active; }
		void const set_gravity(bool _gravity) { affectedGravity = _gravity; }
		void const add_acc(Math::Vec2 _acceleration) { acceleration += _acceleration; }
		bool get_active() { return active; }
		bool get_gravity() { return affectedGravity; }
		Math::Vec2 get_acc() { return acceleration; }
		Math::Vec2 get_vel() { return velocity; }
		Shape get_shape() { return objectShape; }
		Collision::AABB get_AABB() { return boundingBox; }

		void deserialize(rapidjson::Value& _value)
		{
			// deserializeee
			if (_value.HasMember("Velocity"))
			{
				rapidjson::Value& _v = _value["Velocity"].GetObj();
				velocity.deserialize(_v);
			}
			if (_value.HasMember("Acceleration"))
			{
				rapidjson::Value& _v = _value["Acceleration"].GetObj();
				acceleration.deserialize(_v);
			}

			if (_value.HasMember("Shape"))
				objectShape = static_cast<Shape>(_value["Shape"].GetInt());

			if (_value.HasMember("BoundingBox"))
			{
				rapidjson::Value& _v = _value["BoundingBox"].GetObj();
				boundingBox.deserialize(_v);
			}


			if (_value.HasMember("Grav"))
				affectedGravity = _value["Grav"].GetBool();

		}
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
		{

			std::cout << "serializing rigidbody component\n";
			rapidjson::Value type;
			std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
			type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
			_value.AddMember("Type", type, _doc.GetAllocator());

			rapidjson::Value vel(rapidjson::kObjectType);
			velocity.serialize(vel, _doc);
			_value.AddMember("Velocity", vel, _doc.GetAllocator());

			rapidjson::Value accel(rapidjson::kObjectType);
			acceleration.serialize(vel, _doc);
			_value.AddMember("Acceleration", accel, _doc.GetAllocator());

			int objShape = static_cast<int>(objectShape);
			_value.AddMember("Shape", objShape, _doc.GetAllocator());

			rapidjson::Value bb(rapidjson::kObjectType);
			boundingBox.serialize(bb, _doc);
			_value.AddMember("BoundingBox", bb, _doc.GetAllocator());

			_value.AddMember("Grav", affectedGravity, _doc.GetAllocator());

		}
		void inspector_view()
		{
			//ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			//	| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
			//if (ImGui::BeginTable("Component Sprite Renderer", 2, windowFlags))
			//{
			//	ImGui::Indent();
			//	// Sprite
			//	// Extern source file

			//	ImGui::TableSetupColumn("Text", 0, 0.4f);
			//	ImGui::TableSetupColumn("Input", 0, 0.6f);

			//	ImGui::TableNextRow();
			//	ImGui::TableNextColumn();
			//	ImGui::Text("Sprite");
			//	ImGui::TableNextColumn();
			//	ImGui::PushItemWidth(-FLT_MIN);
			//	//ImGui::InputInt("", &spriteID, 0);

			//	// Color
			//	ImGui::TableNextRow();
			//	ImGui::TableNextColumn();
			//	ImGui::Text("Color");
			//	ImGui::TableNextColumn();
			//	//openPopup = ImGui::ColorButton("Color", color, miscFlags, ImVec2(FLT_MAX, 0));

			//	// Flip
			//	ImGui::TableNextRow();
			//	ImGui::TableNextColumn();
			//	ImGui::Text("Flip");
			//	ImGui::TableNextColumn();
			//	//ImGui::Checkbox("X", spriteRenderer.access_flip_x());
			//	//ImGui::SameLine(0.f, sameLinePadding);
			//	//ImGui::Checkbox("Y", spriteRenderer.access_flip_y());

			//	ImGui::Unindent();
			//	ImGui::EndTable();
			//}
		}
		
		
	private:
		Math::Vec2 velocity;					//velocity of object
		Math::Vec2 acceleration;				//acceleration of object
		Shape objectShape;						//Shape of object
		Collision::AABB boundingBox;	//bounding box of object
		bool active = false;                    //is object active?
		bool affectedGravity = false;           //is object affected by gravity?
    };
}