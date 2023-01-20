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
#include <limits>
#include "Windows/windows-system.h"
#include <Physics/collision.h>
#include <Debugging/frame-rate-controller.h>
#include "GameObject/components.h"

namespace Copium
{

	bool collision_rectrect(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2, float dt)
	{
		if ((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x) ||
			(aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y))
			return 0;
		//Initialize values to check for collision between moving rectangles
		Math::Vec2 vB;
		double tFirst = 0, tLast = dt;
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
		return 1;
		//both objects are colliding

	}
	bool collision_pointrect(const Math::Vec2& point,
		const AABB& aabb2, const Math::Vec2& vel2, float dt)
	{
		//Check for static collision
		if ((point.x < aabb2.min.x) || (point.x > aabb2.max.x) ||
			(point.y < aabb2.min.y) || (point.y > aabb2.max.y))
		{
			float tFirst = 0;
			float tLast = dt;

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

	bool static_collision_pointrect
	(const Math::Vec2& point,const AABB& aabb2)
	{
		if ((point.x < aabb2.min.x) || (point.x > aabb2.max.x) ||
			(point.y < aabb2.min.y) || (point.y > aabb2.max.y))
		{
			return 0;
		}
		return 1;
	}
	bool collision_pointcircle(const Math::Vec2& point, const Circle& circle)
	{
		float distx = point.x - circle.centre.x;
		float disty = point.y - circle.centre.y;
		float distance = sqrtf((distx * distx) + (disty * disty));
		if (distance <= circle.radius)
		{
			return 1;
		}
		return 0;
	}
	bool collision_circlecircle(const Circle& circle1, const Math::Vec2& vel1,
		const Circle& circle2, const Math::Vec2& vel2, float& interTime)
	{
		Math::Vec2 vB = vel1 - vel2;
		float vBmag = sqrtf(Math::vec2_dotproduct(vB, vB));
		Math::Vec2 vBn = vB / vBmag;
		Circle circle3;
		circle3.centre = circle2.centre;
		circle3.radius = circle1.radius + circle2.radius;
		Math::Vec2 vecBC = circle1.centre - circle2.centre;
		float m = Math::vec2_dotproduct(vecBC, vBn);
		float distSqr = Math::vec2_squaredistance(circle1.centre, circle2.centre) - (m * m);
		float radSqr = circle3.radius * circle3.radius;
		if (m < 0 && Math::vec2_distance(circle1.centre, circle2.centre) > circle3.radius)
		{
			return 0;
		}
		if (distSqr >= radSqr)
		{
			return 0;
		}
		float s = sqrtf(radSqr - distSqr);
		interTime = (m - s) / vBmag;
		if (interTime > 0 && interTime < 1)
		{
			return 1;
		}
		return 0;
	}

	bool collision_circlesquare(const Circle& circle1, const Math::Vec2& vel1,
		const AABB& aabb, const Math::Vec2& vel2)
	{
		Math::Vec2 pointtoCheck;
		pointtoCheck = Math::Vec2(circle1.centre.x, circle1.centre.y);

		Math::Vec2 vB = vel1 - vel2;
		Math::Vec2 pointtoCheckVel;
		pointtoCheckVel = Math::Vec2(circle1.centre.x + vB.x, circle1.centre.y + vB.y);

		Circle circleTleft;
		circleTleft.centre = Math::Vec2(aabb.min.x, aabb.max.y);
		circleTleft.radius = circle1.radius;

		Circle circleTright;
		circleTright.centre = Math::Vec2(aabb.max.x, aabb.max.y);
		circleTright.radius = circle1.radius;

		Circle circleBleft;
		circleBleft.centre = Math::Vec2(aabb.min.x, aabb.min.y);
		circleBleft.radius = circle1.radius;

		Circle circleBright;
		circleBright.centre = Math::Vec2(aabb.max.x, aabb.min.y);
		circleBright.radius = circle1.radius;

		AABB vert;
		vert.min = Math::Vec2(aabb.min.x, aabb.min.y - circle1.radius);
		vert.max = Math::Vec2(aabb.max.x, aabb.max.y + circle1.radius);
		AABB hor;
		hor.min = Math::Vec2(aabb.min.x - circle1.radius, aabb.min.y);
		hor.max = Math::Vec2(aabb.max.x + circle1.radius, aabb.max.y);

		if (static_collision_pointrect(pointtoCheck, vert) == 0 && static_collision_pointrect(pointtoCheck, hor) == 0
			&& collision_pointcircle(pointtoCheck, circleTleft) == 0 && collision_pointcircle(pointtoCheck, circleTright) == 0
			&& collision_pointcircle(pointtoCheck, circleBleft) == 0 && collision_pointcircle(pointtoCheck, circleBright) == 0)
		{
			if (static_collision_pointrect(pointtoCheckVel, vert) == 0 && static_collision_pointrect(pointtoCheckVel, hor) == 0
				&& collision_pointcircle(pointtoCheckVel, circleTleft) == 0 && collision_pointcircle(pointtoCheckVel, circleTright) == 0
				&& collision_pointcircle(pointtoCheckVel, circleBleft) == 0 && collision_pointcircle(pointtoCheckVel, circleBright) == 0)
			{
				return 0;
			}
		}
		
		return 1;
	}

	
	collisionDirection check_collision_direction(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2)
	{
		Math::Vec2 vB = vel1 - vel2;
		//std::cout << vB.x << " " << vB.y << std::endl;
		float xEntry = 0.0f;
		float xExit = 0.0f;
		float yEntry = 0.0f;
		float yExit = 0.0f;
		//double tFirst = 0;
		//double tLast = Copium::MyFrameRateController.deltaTime;

		if (vB.x < 0)
		{
			xEntry = (aabb1.min.x - aabb2.max.x) / vB.x;
			xExit = (aabb1.min.x - aabb2.max.x) / vB.x;
		}
		if (vB.x > 0)
		{
			xEntry = (aabb1.max.x - aabb2.min.x) / vB.x;
			xExit = (aabb1.max.x - aabb2.min.x) / vB.x;
		}
		if (vB.y < 0)
		{
			yEntry = (aabb1.min.y - aabb2.max.y) / vB.y;
			yExit = (aabb1.min.y - aabb2.max.y) / vB.y;
		}
		if (vB.y > 0)
		{	
			yEntry = (aabb1.max.y - aabb2.min.y) / vB.y;
			yExit = (aabb1.max.y - aabb2.min.y) / vB.y;			
		}
		if (vB.x == 0)
		{
			xEntry = std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		}
		if (vB.y == 0)
		{
			yEntry = std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		}

		if (xEntry < yEntry)
		{
			if (vB.x > 0)
			{	

				return collisionDirection::RIGHT;
				
			}
			if (vB.x < 0)
			{	
				return collisionDirection::LEFT;				
			}
		}
		if (yEntry < xEntry)
		{
			if (vB.y > 0)
			{
				return collisionDirection::TOP;
			}
			if (vB.y < 0)
			{
				return collisionDirection::BOTTOM;
			}
		}
		return collisionDirection::NONE;
	}

	void resolve_AABBcollision(Transform& transform1,AABB& aabb1, AABB& aabb2, collisionDirection direction)
	{		
		float resolvePos = 0.0f;
		if (direction == collisionDirection::TOP)
		{
			resolvePos = aabb1.max.y - aabb2.min.y;
			transform1.position.y -= resolvePos;
		}
		if (direction == collisionDirection::BOTTOM)
		{
			resolvePos = aabb1.min.y - aabb2.max.y;
			transform1.position.y -= resolvePos;			
		}
		if (direction == collisionDirection::LEFT)
		{
			resolvePos = aabb1.min.x - aabb2.max.x;
			transform1.position.x -= resolvePos;
		}
		if (direction == collisionDirection::RIGHT)
		{
			resolvePos = aabb1.max.x - aabb2.min.x;
			transform1.position.x -= resolvePos;
		}
		//std::cout << resolvePos << std::endl;
	}
}