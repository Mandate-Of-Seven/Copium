#include <pch.h>
#include <GameObject/Components/physics-components.h>

namespace Copium
{
	Rigidbody2D::Rigidbody2D(GameObject& _gameObj)
		:Component(_gameObj, ComponentType::Rigidbody2D) {
		velocity = Math::Vec2(0.0, 0.0);
		acceleration = Math::Vec2(0.0, 0.0);
		force = Math::Vec2(0.0, 0.0);
		mass = 1.0;
		affectedGravity = true;
		objectShape = Shape::SQUARE;
	}
	void const Rigidbody2D::set_vel(Math::Vec2 _velocity) { velocity = _velocity; }
	void const Rigidbody2D::set_acc(Math::Vec2 _acceleration) { acceleration = _acceleration; }
	void const Rigidbody2D::set_force(Math::Vec2 _force) { force = _force; }
	void const Rigidbody2D::set_shape(Shape _objectShape) { objectShape = _objectShape; }
	void const Rigidbody2D::set_mass(float _mass) { mass = _mass; }
	void const Rigidbody2D::Active(bool _active) { active = _active; }
	void const Rigidbody2D::set_gravity(bool _gravity) { affectedGravity = _gravity; }
	void const Rigidbody2D::add_acc(Math::Vec2 _acceleration) { acceleration += _acceleration; }
	bool Rigidbody2D::Active() { return active; }
	bool Rigidbody2D::get_gravity() { return affectedGravity; }
	Math::Vec2 Rigidbody2D::get_force() { return force; }
	void Rigidbody2D::add_force(Math::Vec2 _force) { force += _force; }
	Math::Vec2 Rigidbody2D::get_acc() { return acceleration; }
	Math::Vec2 Rigidbody2D::get_vel() { return velocity; }
	float Rigidbody2D::get_mass() { return mass; }
	Shape Rigidbody2D::get_shape() { return objectShape; }
	void Rigidbody2D::deserialize(rapidjson::Value& _value)
	{
		// deserializeee

		if (_value.HasMember("Shape"))
			objectShape = static_cast<Shape>(_value["Shape"].GetInt());

		if (_value.HasMember("Grav"))
			affectedGravity = _value["Grav"].GetBool();

		if (_value.HasMember("Active"))
			active = _value["Active"].GetBool();
	}
	void Rigidbody2D::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{

		std::cout << "serializing Rigidbody component\n";
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		int objShape = static_cast<int>(objectShape);
		_value.AddMember("Shape", objShape, _doc.GetAllocator());


		_value.AddMember("Grav", affectedGravity, _doc.GetAllocator());
		_value.AddMember("Active", active, _doc.GetAllocator());

	}
	void Rigidbody2D::inspector_view()
	{
		ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Rigidbody2D", 2, windowFlags))
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
}