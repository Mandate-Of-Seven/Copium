
#include "pch.h"
#include "physics.h"
#include <graphics.h>
#include <windows-system.h>
#include <windows-input.h>

namespace Copium::Physics
{
	void Physics::init() 
	{	

	}
	void Physics::update()
	{
		if (WindowsInput::isKeyPressed(GLFW_KEY_H))
		{
			Copium::Component::Collider* box = new Copium::Component::Collider;
		}
	}
	void Physics::exit()
	{

	}
	void Physics::update_pos(float dt)
	{	
		/*velocity = velocity + (gravity * dt);*/
		/*position = position + (velocity * dt);
		printf("%f %f\n", position.x, position.y);*/
	}
}