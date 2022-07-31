﻿/*!***************************************************************************************
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

Transform::Transform(Vector2 _position, Vector2 _rotation, Vector2 _scale)
	:position{_position}, rotation{_rotation}, scale{_scale}, parent{nullptr}{}

Vector2 const Transform::Position() { return position; }

void Transform::Position(Vector2 _position) { position = _position; }

Vector2 const Transform::Rotation() { return rotation; }

void Transform::Rotation(Vector2 _rotation) { rotation = _rotation; }

Vector2 const Transform::Scale() { return scale; }

void Transform::Scale(Vector2 _scale) { scale = _scale; }