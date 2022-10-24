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
	:Component(_gameObj,Type::Transform),
	position {_position}, rotation{ _rotation }, scale{ _scale }, parent{ nullptr }{}

glm::dvec3 TransformComponent::glmPosition() const { return position.to_glm(); }
void TransformComponent::set_position(const Copium::Math::Vec3& _position) { position = _position; }

const Copium::Math::Vec3& TransformComponent::get_rotation() { return rotation; }
glm::dvec3 TransformComponent::glmRotation() const { return rotation.to_glm(); }
void TransformComponent::set_rotation(const Copium::Math::Vec3& _rotation) { rotation = _rotation; }

const Copium::Math::Vec3& TransformComponent::get_scale() { return scale; }
glm::dvec3 TransformComponent::glmScale() const { return scale.to_glm(); }
void TransformComponent::set_scale(const Copium::Math::Vec3& _scale) { scale = _scale; }

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


