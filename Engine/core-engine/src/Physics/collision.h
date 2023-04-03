/*!***************************************************************************************
\file			collision.h		
\project		
\author			Abdul Hadi (95%)
				Zacharie Hong (5%)

\par			Course: GAM250
\par			Section: 
\date			10/03/2023

\brief
	This file contains the function declarations for a collision system. It has an AABB
	struct to store the AABB bounding boxes of an object and 2 functions for AABB to AABB
	collision and point to AABB collision.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef COLLISION_H
#define COLLISION_H

#include "Math/math-library.h"

enum class Shape : int
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
namespace Copium
{
	
	struct AABB
	{
		AABB(Math::Vec2 _min = { -0.5f,-0.5f }, Math::Vec2 _max = { 0.5f,0.5f }) : max{ _max }, min{ _min }
		{

		}

		/**************************************************************************/
		/*!
			\brief
				Transform AABB into world space by a position and scale
			\param pos
				Position of bounding box
			\param size
				Size of bounding box
			\return
				Relative bounding box
		*/
		/**************************************************************************/
		AABB GetRelativeBounds(const Math::Vec3& pos,const Math::Vec3& size)
		{
			AABB tmp{ *this };
			tmp.max.x *= size.x;
			tmp.min.x *= size.x;
			tmp.max.y *= size.y;
			tmp.min.y *= size.y;
			tmp.max.x += pos.x;
			tmp.min.x += pos.x;
			tmp.max.y += pos.y;
			tmp.min.y += pos.y;
			return tmp;
		}

		Math::Vec2 min;
		Math::Vec2 max;
	};

	struct Circle
	{
		Circle(Math::Vec2 _centre = { 0.5f, 0.5f }, float _radius = { 0.5f }) : centre(_centre), radius(_radius)
		{

		}
		Math::Vec2 centre;
		float radius;
	};

	struct Ray
	{
		Ray(Math::Vec2 _pt0 = { 0.5f, 0.5f }, Math::Vec2 _dir = {0.5f, 0.0f}) : pt0(_pt0), dir(_dir)
		{

		}
		Math::Vec2 pt0;
		Math::Vec2 dir;
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
		const AABB& aabb2, const Math::Vec2& vel2, double dt);
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
		const AABB& aabb2, const Math::Vec2& vel2, double dt);
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
	Checks for static collision between a point and a circle
	\param point
	The point to check for collision
	\param circle
	The circle of the second object to check
	\return
	1 if the objects are colliding, 0 otherwise.
	*/
	/**************************************************************************/
	bool collision_pointcircle(const Math::Vec2& point, const Circle& circle);
	/***************************************************************************/
	/*!
	\brief
	Checks for collision between two circles
	\param circle1
	The first circle to check
	\param vel1
	The velocity of the first object
	\param circle2
	The second circle to check
	\param vel1
	The velocity of the second object
	\param interTime
	the time of intersections between the two objects
	\return
	1 if the objects are colliding, 0 otherwise.
	*/
	/**************************************************************************/
	bool collision_circlecircle(const Circle& circle1, const Math::Vec2& vel1,
		const Circle& circle2, const Math::Vec2& vel2, float& interTime);
	/***************************************************************************/
	/*!
	\brief
	Checks for collision between circle and a square
	\param circle1
	The first circle to check
	\param vel1
	The velocity of the first object
	\param aabb
	The AABB of the second object to check
	\param vel2
	Velocity of the second object.
	\return
	1 if the objects are colliding, 0 otherwise.
	*/
	/**************************************************************************/
	bool collision_circlesquare(const Circle& circle1, const Math::Vec2& vel1,
		const AABB& aabb, const Math::Vec2& vel2);

	/***************************************************************************/
	/*!
	\brief
	Checks for the shortest collision direction to resolve
	\param aabb1
	The AABB of the first object to check direction
	\param vel1
	The velocity of the first object
	\param aabb2
	The AABB of the second object to check for direction
	\param vel2
	The velocity of the second object
    \return
	The enum class collisiondirection , to show which direction of collision
	to resolve

*/
/**************************************************************************/
	collisionDirection check_collision_direction(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2);
	/***************************************************************************/
	/*!
	\brief
	Resolves collision based on the direction the collision was detected
	\param aabb1
	The first gameobject to resolve 
	\param aabb2
	The second gameobject to resolve
	\param direction
	the direction to resolve
	
	*/
	/**************************************************************************/
	void resolve_AABBcollision(Math::Vec3& position, AABB& aabb1, AABB& aabb2, collisionDirection direction);

}

#endif // !COLLISION_H
