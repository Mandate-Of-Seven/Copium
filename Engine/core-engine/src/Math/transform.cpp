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
#include "Math/transform.h"
#include <rttr/registration>

RTTR_REGISTRATION{
	using namespace rttr;
registration::class_<Transform>("Transform")
.property("Pos", &Transform::get_position, &Transform::set_position)
.property("Rot", &Transform::get_rotation, &Transform::set_rotation)
.property("Scale", &Transform::get_scale, &Transform::set_scale);

}

Transform::Transform() : position{ Copium::Math::Vec3() }, rotation{ Copium::Math::Vec3() }, scale{ Copium::Math::Vec3(1,1,1) }, parent{nullptr}{}

Transform::Transform(Copium::Math::Vec3 _position, Copium::Math::Vec3 _rotation, Copium::Math::Vec3 _scale)
	:position{_position}, rotation{_rotation}, scale{_scale}, parent{nullptr}{}

const Copium::Math::Vec3& Transform::get_position() { return position; }
glm::dvec3 Transform::glmPosition() const { return position.to_glm(); }
void Transform::set_position(const Copium::Math::Vec3& _position) { position = _position; }

const Copium::Math::Vec3& Transform::get_rotation() { return rotation; }
glm::dvec3 Transform::glmRotation() const { return rotation.to_glm(); }
void Transform::set_rotation(const Copium::Math::Vec3& _rotation) { rotation = _rotation; }

const Copium::Math::Vec3& Transform::get_scale() { return scale; }
glm::dvec3 Transform::glmScale() const { return position.to_glm(); }
void Transform::set_scale(const Copium::Math::Vec3& _scale) { scale = _scale; }

bool Transform::deserialize(rapidjson::Value& _value)
{
	
	if (!_value.HasMember("Pos"))
		return false;
	rapidjson::Value& _v = _value["Pos"].GetObj();
	position.deserialize(_v);
	//std::cout << "Position:" << position;

	if (!_value.HasMember("Rot"))
		return false;
	_v = _value["Rot"].GetObj();
	rotation.deserialize(_v);
	//std::cout << "Rotation:" << rotation;

	if (!_value.HasMember("Scale"))
		return false;
	_v = _value["Scale"].GetObj();
	scale.deserialize(_v);
	//std::cout << "Scale:" << scale;

	return true;
}


