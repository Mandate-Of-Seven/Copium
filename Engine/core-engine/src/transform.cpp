/*!***************************************************************************************
\file			transform.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
	Defines Transform Class to be added to GameObjects and its member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


//INCLUDES

#include "transform.h"

Transform::Transform() : position{0}, rotation{0}, scale{1}, parent{nullptr}{}

Transform::Transform(Vector3 _position, Vector3 _rotation, Vector3 _scale)
	:position{_position}, rotation{_rotation}, scale{_scale}, parent{nullptr}{}

Vector3 const Transform::Position() { return position; }

void Transform::Position(Vector3 _position) { position = _position; }

Vector3 const Transform::Rotation() { return rotation; }

void Transform::Rotation(Vector3 _rotation) { rotation = _rotation; }

Vector3 const Transform::Scale() { return scale; }

void Transform::Scale(Vector3 _scale) { scale = _scale; }