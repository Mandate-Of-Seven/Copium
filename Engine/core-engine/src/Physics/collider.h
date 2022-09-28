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
namespace Copium::Component
{
    class RigidBody
    {
	public:
		void const set_pos(Math::Vec2 _position) { position = _position; }
		void const set_vel(Math::Vec2 _velocity) { velocity = _velocity; }
		void const set_acc(Math::Vec2 _acceleration) { acceleration = _acceleration; }
		void const set_grav(Math::Vec2 _gravity) { gravity = _gravity; }
		void const set_shape(Shape _objectShape) { objectShape = _objectShape; }
		void const set_AABB(Math::Vec2 _min, Math::Vec2 _max) { boundingBox.max = _max; boundingBox.min = _min;}
		void const set_active(bool _active) { active = _active; }
		void const set_gravity(bool _gravity) { affectedGravity = _gravity; }
		bool get_active() { return active; }
		bool get_gravity() { return affectedGravity; }
		Math::Vec2 get_pos() { return position; }
		Math::Vec2 get_vel() { return velocity; }
		Shape get_shape() { return objectShape; }
		Copium::Collision::AABB get_AABB() { return boundingBox; }
		
		
	private:
		Math::Vec2 position;					//position of object- to be removed
		Math::Vec2 velocity;					//velocity of object
		Math::Vec2 acceleration;				//acceleration of object
		Math::Vec2 gravity;						//gravity affected on the object
		Shape objectShape = SQUARE;				//Shape of object
		Copium::Collision::AABB boundingBox;	//bounding box of object
		bool active = false;                    //is object active?
		bool affectedGravity = false;           //is object affected by gravity?
    };
}