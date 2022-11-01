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
namespace Copium
{
    class RigidBodyComponent: public Component
    {
	public:

		RigidBodyComponent(GameObject& _gameObj)
			:Component(_gameObj, ComponentType::RigidBody) {
			velocity = Math::Vec2(0.0,0.0);
			acceleration = Math::Vec2(0.0, 0.0);
			force = Math::Vec2(0.0, 0.0);
			mass = 1.0;
			affectedGravity = true;
			objectShape = SQUARE;
		}
		void const set_vel(Math::Vec2 _velocity) { velocity = _velocity; }
		void const set_acc(Math::Vec2 _acceleration) { acceleration = _acceleration; }
		void const set_force(Math::Vec2 _force) { force = _force; }
		void const set_shape(Shape _objectShape) { objectShape = _objectShape; }
		void const set_AABB(Math::Vec2 _min, Math::Vec2 _max) { boundingBox.max = _max; boundingBox.min = _min;}
		void const set_mass(float _mass) { mass = _mass; }
		void const set_active(bool _active) { active = _active; }
		void const set_gravity(bool _gravity) { affectedGravity = _gravity; }
		void const add_acc(Math::Vec2 _acceleration) { acceleration += _acceleration; }
		bool get_active() { return active; }
		bool get_gravity() { return affectedGravity; }
		Math::Vec2 get_force() { return force; }
		Math::Vec2 get_acc() { return acceleration; }
		Math::Vec2 get_vel() { return velocity; }
		float get_mass() { return mass; }
		Shape get_shape() { return objectShape; }
		Collision::AABB get_AABB() { return boundingBox; }


		


		void inspector_view() {};

		/***************************************************************************/
		/*!
		\brief
			Deserialize this component's data from specified rapidjson value
		*/
		/**************************************************************************/
		//void deserialize(rapidjson::Value& _value);

		RigidBodyComponent& operator=(const RigidBodyComponent& rhs) { (void)rhs; return *this; }
		
		
	private:
		Math::Vec2 velocity;					//velocity of object
		Math::Vec2 acceleration;				//acceleration of object
		Math::Vec2 force;						//forces acting on object
		float mass;								//mass of object
		Shape objectShape;						//Shape of object
		Collision::AABB boundingBox;	//bounding box of object
		bool active;                    //is object active?
		bool affectedGravity;           //is object affected by gravity?
    };
}