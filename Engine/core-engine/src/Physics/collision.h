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

namespace Copium::Collision
{
	
	struct AABB
	{
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
}