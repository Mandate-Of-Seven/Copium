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

		RigidBodyComponent(GameObject& _gameObj)
			:Component(_gameObj, ComponentType::RigidBody), boundingBox() {
			velocity = Math::Vec2(0.0,0.0);
			acceleration = Math::Vec2(0.0, 0.0);
			force = Math::Vec2(0.0, 0.0);
			mass = 1.0;
			affectedGravity = true;
			objectShape = Shape::SQUARE;
		}

		void const set_vel(Math::Vec2 _velocity) { velocity = _velocity; }
		void const set_acc(Math::Vec2 _acceleration) { acceleration = _acceleration; }
		void const set_force(Math::Vec2 _force) { force = _force; }
		void const set_shape(Shape _objectShape) { objectShape = _objectShape; }
		void const set_AABB(Math::Vec2 _min, Math::Vec2 _max) { boundingBox.max = _max; boundingBox.min = _min;}
		void const set_mass(float _mass) { mass = _mass; }
		void const Active(bool _active) { active = _active; }
		void const set_gravity(bool _gravity) { affectedGravity = _gravity; }
		void const add_acc(Math::Vec2 _acceleration) { acceleration += _acceleration; }
		bool Active() { return active; }
		bool get_gravity() { return affectedGravity; }
		Math::Vec2 get_force() { return force; }
		void add_force(Math::Vec2 _force) { force += _force; }
		Math::Vec2 get_acc() { return acceleration; }
		Math::Vec2 get_vel() { return velocity; }
		float get_mass() { return mass; }
		Shape get_shape() { return objectShape; }
		AABB get_AABB() { 
			Math::Vec3& size{ gameObj.Transform().scale };
			Math::Vec3& pos{ gameObj.Transform().position };
			float x = (boundingBox.max.x - boundingBox.min.x) * size.x;
			float y = (boundingBox.max.y - boundingBox.min.y) * size.y;
			AABB tmp{ boundingBox };
			tmp.max.x *= x; 
			tmp.min.x *= x; 
			tmp.max.y *= y; 
			tmp.min.y *= y;
			tmp.max.x += pos.x;
			tmp.min.x += pos.x;
			tmp.max.y += pos.y;
			tmp.min.y += pos.y;
			return tmp;
		}

		void deserialize(rapidjson::Value& _value)
		{
			// deserializeee

			if (_value.HasMember("Shape"))
				objectShape = static_cast<Shape>(_value["Shape"].GetInt());

			if (_value.HasMember("Grav"))
				affectedGravity = _value["Grav"].GetBool();

			if (_value.HasMember("Active"))
				active = _value["Active"].GetBool();
		}
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
		{

			std::cout << "serializing rigidbody component\n";
			rapidjson::Value type;
			std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
			type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
			_value.AddMember("Type", type, _doc.GetAllocator());

			int objShape = static_cast<int>(objectShape);
			_value.AddMember("Shape", objShape, _doc.GetAllocator());

			rapidjson::Value bb(rapidjson::kObjectType);
			boundingBox.serialize(bb, _doc);
			_value.AddMember("BoundingBox", bb, _doc.GetAllocator());

			_value.AddMember("Grav", affectedGravity, _doc.GetAllocator());
			_value.AddMember("Active", active, _doc.GetAllocator());

		}
		void inspector_view()
		{
			ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
				| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
			if (ImGui::BeginTable("RigidbodyComponent", 2, windowFlags))
			{
				ImGui::Indent();
				// Flip
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Active");
				ImGui::TableNextColumn();
				ImGui::Checkbox("##Active", &active);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Apply Gravity");
				ImGui::TableNextColumn();
				ImGui::Checkbox("##Gravity", &affectedGravity);

				ImGui::Unindent();
				ImGui::EndTable();
			}
		}
		
		
	private:
		Math::Vec2 velocity;					//velocity of object
		Math::Vec2 acceleration;				//acceleration of object
		Math::Vec2 force;						//forces acting on object
		float mass;								//mass of object
		Shape objectShape;						//Shape of object
		AABB boundingBox;			//bounding box of object
		bool active;							//is object active?
		bool affectedGravity;					//is object affected by gravity?
    };
}