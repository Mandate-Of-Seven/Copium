/*!***************************************************************************************
\file			physics-components.h
\project
\author			Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			24/09/2022

\brief
	// This file contains the rigidbody component. It will store all the necessary values
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
		/***************************************************************************/
		/*!
		\brief
		Constructor for the Rigidbody2D
		\param _gameObj
		Owner of this component
		*/
		/**************************************************************************/
		Rigidbody2D(GameObject& _gameObj);
		/***************************************************************************/
		/*!
		\brief
		Sets velocity of current rigidbody
		\param _velocity
		velocity to set
		*/
		/**************************************************************************/
		void const set_vel(Math::Vec2 _velocity);
		/***************************************************************************/
		/*!
		\brief
		Sets acceleration of current rigidbody
		\param _acceleration
		acceleration to set
		*/
		/**************************************************************************/
		void const set_acc(Math::Vec2 _acceleration);
		/***************************************************************************/
		/*!
		\brief
		Sets force of current rigidbody
		\param _force
		force to set
		*/
		/**************************************************************************/
		void const set_force(Math::Vec2 _force);
		/***************************************************************************/
		/*!
		\brief
		Sets mass of current rigidbody
		\param _mass
		mass to set
		*/
		/**************************************************************************/
		void const set_mass(float _mass);
		/***************************************************************************/
		/*!
		\brief
		Sets gravity of current rigidbody
		\param _gravity
		gravity to set
		*/
		/**************************************************************************/
		void const set_gravity(bool _gravity);
		/***************************************************************************/
		/*!
		\brief
		adds acceleration to current rigid body
		\param _acceleration
		acceleration to add
		*/
		/**************************************************************************/
		void const add_acc(Math::Vec2 _acceleration);
		/***************************************************************************/
		/*!
		\brief
		Checks whether an object in active
		\return
		Whether the object is active or not
		*/
		/**************************************************************************/
		bool Active() { return active; };
		/***************************************************************************/
		/*!
		\brief
		gets gravity of current rigidbody
		\return
		the gravity value
		*/
		/**************************************************************************/
		bool get_gravity();
		/***************************************************************************/
		/*!
		\brief
		gets force of current rigidbody
		\return
		the force value
		*/
		/**************************************************************************/
		Math::Vec2 get_force();
		/***************************************************************************/
		/*!
		\brief
		adds force to current rigid body
		\param _force
		force to add
		*/
		/**************************************************************************/
		void add_force(Math::Vec2 _force);
		/***************************************************************************/
		/*!
		\brief
		gets acceleration of current rigidbody
		\return
		the acceleration value
		*/
		/**************************************************************************/
		Math::Vec2 get_acc();
		/***************************************************************************/
		/*!
		gets velocity of current rigidbody
		\return
		the velocity value
		*/
		/**************************************************************************/
		Math::Vec2 get_vel();
		/***************************************************************************/
		/*!
		\brief
		gets mass of current rigidbody
		\return
		the mass value
		*/
		/**************************************************************************/
		float get_mass();
		/***************************************************************************/
		/*!
		gets shape of current rigidbody
		\return
		the shape
		*/
		/**************************************************************************/
		Shape get_shape();

		/***************************************************************************/
		/*!
		\brief
		Deserialize this component's data from specified rapidjson value
		*/
		/**************************************************************************/
		void deserialize(rapidjson::Value& _value);
		/*******************************************************************************
		/*!
		*
		\brief
		Serialize this renderer component's data to the specified rapidjson Value
		\param _value
		reference to the rapidjson Value which the renderer component's data is to 
		serialize its data to
		\param _doc
		reference to the rapidjson Document which is associated to the save file which 
		the data is being saved to
		\return
		void
		*/
		/*******************************************************************************/
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);
		/*******************************************************************************
		/*!
		*
		\brief
		Displays the inspector view with its fields
		*/
		/*******************************************************************************/
		void inspector_view();

		/*******************************************************************************
		/*!
		\brief
		Deep copies a Rigidbody2D into another
		\param gameobj
		Reference to another Rigidbody2D
		\return
		Reference to this Rigidbody2DComponent
		*/
		/*******************************************************************************/
		Component* clone(GameObject& _gameObj, ComponentID* newID = nullptr) const
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