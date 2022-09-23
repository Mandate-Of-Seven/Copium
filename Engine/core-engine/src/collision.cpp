#include "pch.h"
#include "collision.h"
#include <windows-system.h>

namespace Copium::Collision
{

	bool Collision_RectRect(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2)
	{
		return 1;
	}
	bool Collision_PointRect(const Math::Vec2& point,
		const AABB& aabb2, const Math::Vec2& vel2)
	{
		//Check for static collision
		if ((point.X() < aabb2.min.X()) || (point.X() > aabb2.max.X()) ||
			(point.y < aabb2.min.y) || (point.y > aabb2.max.y))
		{

			float tFirst = 0;
			float tLast = GLHelper::delta_time;

			//Object b is moving away from object a on the x axis
			if (vel2.X() < 0)
			{
				// case 1, b is moving away from a and no collision
				if (point.X() > aabb2.max.X())
				{
					return 0;
				}
				// case 4 1/2
				if (point.x < aabb2.min.x)
					/*tFirst = std::max((float)(point.x - aabb2.max.x) / vel2.x), tFirst);*/
					if ((point.X() - aabb2.max.X() / vel2.X()) > tFirst)
					{
						tFirst = (point.X() - aabb2.max.X()) / vel2.X();
					}

				// case 4 2/2
				if (point.X() < aabb2.max.X())
					//tLast = min((point.x - aabb2.max.x) / vel2.x, tLast);
					if ((point.X() - aabb2.max.X() / vel2.X()) < tLast)
					{
						tLast = (point.X() - aabb2.max.X()) / vel2.X();
					}
			}
			//Object b is moving towards object a on the x axis
			if (vel2.X() > 0)
			{
				// case 2 1/2
				if (point.X() > point.X())
					/*tFirst = max((point.x - aabb2.max.x) / vel2.x, tFirst);*/
					if ((point.X() - aabb2.max.X() / vel2.X()) > tFirst)
					{
						tFirst = (point.X() - aabb2.max.X()) / vel2.X();
					}
				//case 2 2/2
				if (point.X() > aabb2.min.X())
					//tLast = min((point.x - aabb2.min.x) / vel2.x, tLast);
					if ((point.X() - aabb2.min.X() / vel2.X()) < tLast)
					{
						tLast = (point.X() - aabb2.min.X()) / vel2.X();
					}
				//case 3, b is moving away from a, no collision
				if (point.X() < aabb2.min.X())
				{
					return 0;
				}
			}
			//Object b is moving away from object a on the y axis
			if (vel2.Y() < 0)
			{
				// case 1, b is moving away from a and no collision
				if (point.Y() > aabb2.max.Y())
				{
					return 0;
				}
				// case 4 1/2
				if (point.Y() < aabb2.min.Y())
				{
					//tFirst = max((point.y - aabb2.min.y) / vel2.y, tFirst);
					if ((point.Y() - aabb2.min.Y() / vel2.Y()) > tFirst)
					{
						tFirst = (point.Y() - aabb2.min.Y()) / vel2.Y();
					}
				}
				// case 4 2/2
				if (point.Y() < aabb2.max.Y())
					//tLast = min((point.y - aabb2.max.y) / vel2.y, tLast);
					if ((point.Y() - aabb2.max.Y() / vel2.Y()) < tLast)
					{
						tLast = (point.Y() - aabb2.max.Y()) / vel2.Y();
					}
			}
			//Object b is moving away towards object a on the y axis
			if (vel2.Y() > 0)
			{	// case 2 1/2
				if (point.Y() > aabb2.max.Y())
					//tFirst = max((point.y - aabb2.max.y) / vel2.y, tFirst);
					if ((point.Y() - aabb2.max.Y() / vel2.Y()) > tFirst)
					{
					tFirst = (point.Y() - aabb2.max.Y()) / vel2.Y();
					}
				//case 2 2/2
				if (point.Y() > aabb2.min.Y())
					//tLast = min((point.y - aabb2.min.y) / vel2.y, tLast);
				if ((point.Y() - aabb2.min.Y() / vel2.Y()) < tLast)
				{
					tLast = (point.Y() - aabb2.max.Y()) / vel2.Y();
				}
				//case 3, b is moving away from a, no collision
				if (point.Y() < aabb2.min.Y())
				{
					return 0;
				}
			}

			//case 5, no intersection
			if (tFirst > tLast)
				return 0;
		}
		else
			return 1;
		//both objects are colliding
	}
}