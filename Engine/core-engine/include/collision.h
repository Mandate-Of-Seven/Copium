
#include "math-library.h"


namespace Copium::Collision
{
	
	struct AABB
	{
		Math::Vec2 max;
		Math::Vec2 min;
	};

	bool Collision_RectRect(const AABB& aabb1, const Math::Vec2& vel1,
		const AABB& aabb2, const Math::Vec2& vel2);

	bool Collision_PointRect(const Math::Vec2& point,
		const AABB& aabb2, const Math::Vec2& vel2);
}