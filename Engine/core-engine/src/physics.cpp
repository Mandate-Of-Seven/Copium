
#include "pch.h"
#include "physics.h"
#include <windows-system.h>

namespace Copium::Physics
{
	void Physics::init() 
	{
		position = Math::Vec2(0.0, 0.0);
		velocity = Math::Vec2(0.0, 0.0);
		acceleration = Math::Vec2(0.0, 0.0);

	}
	void Physics::update()
	{
		Update_Pos(GLHelper::delta_time);
	}
}