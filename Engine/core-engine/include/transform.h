/*!***************************************************************************************
****
\file			transform.h
\project		
\author			Zacharie Hong
\co-authors		// DELETE IF NOT APPLICABLE

\par			Course: GAM200
\par			Section: 
\date			16/07/2022

\brief
	// BRIEF DESCRIPTION OF FILE PURPOSE

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/

#pragma once

#ifndef HEADER_TRANSFORM

#define HEADER_TRANSFORM

//INCLUDES
#include <glm/vec2.hpp> //Vec2
#include <list>

//USING
using Vector2 = glm::dvec2;

class Transform;

class Transform final
{
private:
    Vector2 position;
    Vector2 rotation;
    Vector2 scale;
    Transform* parent;
    std::list<Transform*> childList;

public:
    Transform();
    Transform(Vector2 _position, Vector2 _rotation = {0,0}, Vector2 _scale = {1,1});

    Vector2 const Position();

    void Position(Vector2 _position);

    Vector2 const Rotation();

    void Rotation(Vector2 _rotation);

    Vector2 const Scale();

    void Scale(Vector2 _scale);
};

#endif // !HEADER_TRANSFORM
