/*!***************************************************************************************
\file			collision.h		
\project		
\author			Abdul Hadi

\par			Course: GAM200
\par			Section: 
\date			23/09/2022

\brief
	This file contains the function declarations for a collision system. It has an AABB
	struct to store the AABB bounding boxes of an object and 2 functions for AABB to AABB
	collision and point to AABB collision.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "Math/math-library.h"
enum Shape
{	DOT,
	SQUARE
};
enum class collisionDirection : int
{
	NONE,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};
namespace Copium::Collision
{
	
	struct AABB
	{
		/***************************************************************************/
		/*!
		\brief
			Deserialize this AABB's data from the specified rapidjson Value

		\param _value
			reference to the rapidjson Value from which this AABB's data is deserialized from

		\return
			void
		*/
		/**************************************************************************/
		void deserialize(rapidjson::Value& _value)
		{
			if (_value.HasMember("Min"))
			{
				rapidjson::Value& _v = _value["Min"].GetObj();
				min.deserialize(_v);
			}
			if (_value.HasMember("Max"))
			{
				rapidjson::Value& _v = _value["Max"].GetObj();
				max.deserialize(_v);
			}
		}
		/***************************************************************************/
		/*!
		\brief
			Serialize this AABB's data to the specified rapidjson Value

		\param _value
			reference to the rapidjson Value to which this AABB's data is to be serialized to

		\param _doc
			reference to the rapidjson Document that is associated with the save file

		\return
			void
		*/
		/**************************************************************************/
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
		{
			rapidjson::Value minimum(rapidjson::kObjectType);
			rapidjson::Value maximum(rapidjson::kObjectType);

			min.serialize(minimum, _doc);
			max.serialize(maximum, _doc);

			_value.AddMember("Min", minimum, _doc.GetAllocator());
			_value.AddMember("Max", maximum, _doc.GetAllocator());

		}
		Math::Vec2 min;
		Math::Vec2 max;
	};
	/***************************************************************************/
   /*!
   \brief
	   Checks for collision between 2 rectangles
   \param aabb1
	   The AABB of the first object to check
   \param vel1
	   Velocity of the first object
   \param aabb2
	   The AABB of the second object to check
   \param vel2
       Velocity of the second object.
   \return
   1 if the objects are colliding, 0 otherwise.
   */
   /**************************************************************************/
	bool collision_rectrect(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2);
	/***************************************************************************/
   /*!
   \brief
	   Checks for collision between a rectangle and a point
   \param point
	   The point to check for collision
   \param aabb2
	   The AABB of the second object to check
   \param vel2
	   Velocity of the second object.
   \return
   1 if the objects are colliding, 0 otherwise.
   */
   /**************************************************************************/
	bool collision_pointrect(const Math::Vec2& point,
		const AABB& aabb2, const Math::Vec2& vel2);
	/***************************************************************************/
	/*!
	\brief
	Checks for static collision between a rectangle and a point
	\param point
	The point to check for collision
	\param aabb2
	The AABB of the second object to check
	\return
	1 if the objects are colliding, 0 otherwise.
	*/
	/**************************************************************************/
	bool static_collision_pointrect(const Math::Vec2& point,const AABB& aabb2);

	/***************************************************************************/
	/*!
	\brief
	Resolves collision based on the direction the collision was detected
	\param aabb1
	The AABB of the first object to resolve
	\param aabb2
	The AABB of the second object to resolve
	
	*/
	/**************************************************************************/
	void resolve_collision(const AABB& aabb1, const AABB& aabb2, collisionDirection direction);

	collisionDirection check_collision_direction(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2);
}