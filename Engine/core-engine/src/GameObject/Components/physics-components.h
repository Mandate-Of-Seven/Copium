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
#include "GameObject/Components/component.h"

namespace Copium
{
    class Rigidbody2D : public Component
    {
	public:

		Rigidbody2D(GameObject& _gameObj);

		void const set_vel(Math::Vec2 _velocity);
		void const set_acc(Math::Vec2 _acceleration);
		void const set_force(Math::Vec2 _force);
		void const set_mass(float _mass);
		void const set_gravity(bool _gravity);
		void const add_acc(Math::Vec2 _acceleration);
		bool Active() { return active; };
		bool get_gravity();
		Math::Vec2 get_force();
		void add_force(Math::Vec2 _force);
		Math::Vec2 get_acc();
		Math::Vec2 get_vel();
		float get_mass();
		Shape get_shape();


		void deserialize(rapidjson::Value& _value);
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);
		void inspector_view();


		Component* clone(GameObject& _gameObj) const
		{
			Rigidbody2D* component = new Rigidbody2D(_gameObj);
			component->mass = mass;
			component->active = active;
			component->affectedGravity = affectedGravity;
			return component;
		}
		
	private:
		Math::Vec2 velocity;					//velocity of object
		Math::Vec2 acceleration;				//acceleration of object
		Math::Vec2 force;						//forces acting on object
		float mass;								//mass of object
		bool active;							//is object active?
		bool affectedGravity;					//is object affected by gravity?
    };
}