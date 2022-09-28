/*!***************************************************************************************
\file			collision.cpp
\project
\author			Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			23/09/2022

\brief
	This file contains the function definitions for a collision system. It has an AABB
	struct to store the AABB bounding boxes of an object and 2 functions for AABB to AABB
	collision and point to AABB collision.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Windows/windows-system.h"
#include "Physics/collision.h"

namespace Copium::Collision
{

	bool collision_rectrect(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2)
	{
		if ((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x) ||
			(aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y))
			return 0;
		else
		{

			//Initialize values to check for collision between moving rectangles
			Math::Vec2 vB;
			double tFirst = 0, tLast = Copium::Windows::WindowsSystem::Instance()->get_delta_time();
			vB.x = vel2.x - vel1.x;
			vB.y = vel2.y - vel1.y;
			//Object b is moving away from object a on the x axis
			if (vB.x < 0)
			{
				// case 1, b is moving away from a and no collision
				if (aabb1.min.x > aabb2.max.x)
				{
					return 0;
				}
				// case 4 1/2
				if (aabb1.max.x < aabb2.min.x)
					//tFirst = max((aabb1.min.x - aabb2.max.x) / vB.x, tFirst);
				{
					if ((aabb1.max.x - aabb2.min.x / vB.x) > tFirst)
					{
						tFirst = (aabb1.max.x - aabb2.min.x) / vB.x;
					}
				}
				// case 4 2/2
				if (aabb1.min.x < aabb2.max.x)
					//tLast = min((aabb1.min.x - aabb2.max.x) / vB.x, tLast);
				{
					if ((aabb1.min.x - aabb2.max.x / vB.x) < tLast)
					{
						tLast = (aabb1.min.x - aabb2.max.x) / vB.x;
					}
				}
			}
			//Object b is moving towards object a on the x axis
			if (vB.x > 0)
			{
				// case 2 1/2
				if (aabb1.min.x > aabb2.max.x)
					//tFirst = max((aabb1.min.x - aabb2.max.x) / vB.x, tFirst);
				{
					if ((aabb1.min.x - aabb2.max.x / vB.x) > tFirst)
					{
						tFirst = (aabb1.min.x - aabb2.max.x) / vB.x;
					}
				}
				//case 2 2/2
				if (aabb1.max.x > aabb2.min.x)
					//tLast = min((aabb1.max.x - aabb2.min.x) / vB.x, tLast);
				{
					if ((aabb1.max.x - aabb2.min.x / vB.x) < tLast)
					{
						tLast = (aabb1.max.x - aabb2.min.x) / vB.x;
					}
				}
				//case 3, b is moving away from a, no collision
				if (aabb1.max.x < aabb2.min.x)
				{
					return 0;
				}
			}
			//Object b is moving away from object a on the y axis
			if (vB.y < 0)
			{
				// case 1, b is moving away from a and no collision
				if (aabb1.min.y > aabb2.max.y)
				{
					return 0;
				}
				// case 4 1/2
				if (aabb1.max.y < aabb2.min.y)
					//tFirst = max((aabb1.max.y - aabb2.min.y) / vB.y, tFirst);
				{
					if ((aabb1.max.y - aabb2.min.y / vB.y) > tFirst)
					{
						tFirst = (aabb1.max.y - aabb2.min.y) / vB.y;
					}
				}
				// case 4 2/2
				if (aabb1.min.y < aabb2.max.y)
				{	//tLast = min((aabb1.min.y - aabb2.max.y) / vB.y, tLast);
					if ((aabb1.min.y - aabb2.max.y / vB.y) < tLast)
					{
						tLast = (aabb1.min.y - aabb2.max.y) / vB.y;
					}
				}
			}
			//Object b is moving towards object a on the y axis
			if (vB.y > 0)
			{	// case 2 1/2
				if (aabb1.min.y > aabb2.max.y)
				{	//tFirst = max((aabb1.min.y - aabb2.max.y) / vB.y, tFirst);
					if ((aabb1.min.y - aabb2.max.y / vB.y) > tFirst)
					{
						tFirst = (aabb1.min.y - aabb2.max.y) / vB.y;
					}
				}
				//case 2 2/2
				if (aabb1.max.y > aabb2.min.y)
				{	//tLast = min((aabb1.max.y - aabb2.min.y) / vB.y, tLast);
					if ((aabb1.max.y - aabb2.min.y / vB.y) < tLast)
					{
						tLast = (aabb1.max.y - aabb2.min.y) / vB.y;
					}
				}
				//case 3, b is moving away from a, no collision
				if (aabb1.max.y < aabb2.min.y)
				{
					return 0;
				}
			}

			//case 5, no intersection
			if (tFirst > tLast)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
			//both objects are colliding
			//return 1;

	}
	bool collision_pointrect(const Math::Vec2& point,
		const AABB& aabb2, const Math::Vec2& vel2)
	{
		//Check for static collision
		if ((point.x < aabb2.min.x) || (point.x> aabb2.max.x) ||
			(point.y < aabb2.min.y) || (point.y > aabb2.max.y))
		{

			float tFirst = 0;
			float tLast = (float)Copium::Windows::WindowsSystem::Instance()->get_delta_time();

			//Object b is moving away from object a on the x axis
			if (vel2.x < 0)
			{
				// case 1, b is moving away from a and no collision
				if (point.x > aabb2.max.x)
				{
					return 0;
				}
				// case 4 1/2
				if (point.x < aabb2.min.x)
					/*tFirst = std::max((float)(point.x - aabb2.max.x) / vel2.x), tFirst);*/
					if ((point.x - aabb2.max.x / vel2.x) > tFirst)
					{
						tFirst = (float)((point.x - aabb2.max.x) / vel2.x);
					}

				// case 4 2/2
				if (point.x < aabb2.max.x)
					//tLast = min((point.x - aabb2.max.x) / vel2.x, tLast);
					if ((point.x - aabb2.max.x / vel2.x) < tLast)
					{
						tLast = (float) ((point.x - aabb2.max.x) / vel2.x);
					}
			}
			//Object b is moving towards object a on the x axis
			if (vel2.x > 0)
			{
				// case 2 1/2
				if (point.x > point.x)
					/*tFirst = max((point.x - aabb2.max.x) / vel2.x, tFirst);*/
					if ((point.x - aabb2.max.x / vel2.x) > tFirst)
					{
						tFirst = (float) ((point.x - aabb2.max.x) / vel2.x);
					}
				//case 2 2/2
				if (point.x > aabb2.min.x)
					//tLast = min((point.x - aabb2.min.x) / vel2.x, tLast);
					if ((point.x - aabb2.min.x / vel2.x) < tLast)
					{
						tLast = (float) ((point.x - aabb2.min.x) / vel2.x);
					}
				//case 3, b is moving away from a, no collision
				if (point.x < aabb2.min.x)
				{
					return 0;
				}
			}
			//Object b is moving away from object a on the y axis
			if (vel2.x < 0)
			{
				// case 1, b is moving away from a and no collision
				if (point.x > aabb2.max.x)
				{
					return 0;
				}
				// case 4 1/2
				if (point.y < aabb2.min.y)
				{
					//tFirst = max((point.y - aabb2.min.y) / vel2.y, tFirst);
					if ((point.y - aabb2.min.y / vel2.y) > tFirst)
					{
						tFirst = (float) ((point.y - aabb2.min.y) / vel2.y);
					}
				}
				// case 4 2/2
				if (point.y < aabb2.max.y)
					//tLast = min((point.y - aabb2.max.y) / vel2.y, tLast);
					if ((point.y - aabb2.max.y / vel2.y) < tLast)
					{
						tLast = (float) ((point.y - aabb2.max.y) / vel2.y);
					}
			}
			//Object b is moving away towards object a on the y axis
			if (vel2.y > 0)
			{	// case 2 1/2
				if (point.y > aabb2.max.y)
					//tFirst = max((point.y - aabb2.max.y) / vel2.y, tFirst);
					if ((point.y - aabb2.max.y / vel2.y) > tFirst)
					{
					tFirst = (float) ((point.y - aabb2.max.y) / vel2.y);
					}
				//case 2 2/2
				if (point.y > aabb2.min.y)
					//tLast = min((point.y - aabb2.min.y) / vel2.y, tLast);
				if ((point.y - aabb2.min.y / vel2.y) < tLast)
				{
					tLast = (float) ((point.y - aabb2.max.y) / vel2.y);
				}
				//case 3, b is moving away from a, no collision
				if (point.y < aabb2.min.y)
				{
					return 0;
				}
			}

			//case 5, no intersection
			if (tFirst > tLast)
				return 0;
		}
			return 1;
		//both objects are colliding
	}
}