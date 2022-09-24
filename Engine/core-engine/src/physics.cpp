/*!***************************************************************************************
\file			physics.cpp
\project
\author			Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			23/09/2022

\brief
	This file contains the function definitions for a physics system. It will perform
	physics and collision on a collider component of an object.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "windows-system.h"
#include "windows-input.h"
#include "physics.h"
#include "graphics-system.h"

namespace Copium::Physics
{
	void Physics::init() 
	{	

	}
	void Physics::update()
	{
		int count = 0;
		if (WindowsInput::isKeyPressed(GLFW_KEY_H))
		{
			if (count == 0)
			{
				glm::vec2 a = { 0.0,0.0 };
				Copium::Component::Collider* box = new Copium::Component::Collider;

				if (Copium::Graphics::GraphicsSystem::Instance()->get_sprites().size() != 0)
				{
					a = Copium::Graphics::GraphicsSystem::Instance()->get_sprites()[0]->get_position();
				}
				printf("%f\n", a.x);
				box->set_pos(Math::Vec2{ 0.0, 0.0 });
				box->set_vel(Math::Vec2{ 0.0, -1.0 });
				box->set_shape(SQUARE);
				if (box->get_shape() == SQUARE)
				{
					box->set_AABB(Math::Vec2{ -1.0,-1.0 }, Math::Vec2{ 1.0,1.0 });
				}
				boxes.push_back(box);
				count++;
			}
			if (count == 1)
			{
				Copium::Component::Collider* box = new Copium::Component::Collider;
				box->set_pos(Math::Vec2{ 0.0, -10.0 });
				box->set_vel(Math::Vec2{ 0.0, 0.0 });
				box->set_shape(SQUARE);
				if (box->get_shape() == SQUARE)
				{
					box->set_AABB(Math::Vec2{ -1.0,(-1.0+(-10.0)) }, Math::Vec2{ 1.0,(1.0+(-10.0))});
				}
				boxes.push_back(box);
				count = 0;
			}
		}
		update_pos(Copium::Windows::WindowsSystem::Instance()->get_delta_time());
	}
	void Physics::exit()
	{
		for (Copium::Component::Collider* a : boxes)
		{
			delete a;
		}
	}
	void Physics::update_pos(float dt)
	{
		for (int a = 0; a < boxes.size(); a++)
		{
			Math::Vec2 velocity;
			Math::Vec2 position;
			Copium::Collision::AABB bound;
			velocity = boxes[a]->get_vel();
			position = boxes[a]->get_pos();
			bound = boxes[a]->get_AABB();
			position = position + (velocity * dt);
			boxes[a]->set_pos(position);
			bound.max = bound.max + (velocity * dt);
			bound.min = bound.min + (velocity * dt);
			boxes[a]->set_AABB(bound.min, bound.max);
			//printf("box %d position: X:%f Y: %f \nAABB min: X:%f Y%f\n AABB Max: X:%f Y:%f\n",a, position.x, position.y, bound.min.x, bound.min.y, bound.max.x, bound.max.y);
			for (int b = 0; b < boxes.size(); b++)
			{
				Math::Vec2 velocityB;
				Math::Vec2 positionB;
				Copium::Collision::AABB boundB;
				velocityB = boxes[b]->get_vel();
				positionB = boxes[b]->get_pos();
				boundB = boxes[b]->get_AABB();
				if (a == b)
					continue;
				else
					if (boxes[a]->get_shape() == SQUARE)
					{
						if (boxes[b]->get_shape() == SQUARE)
						{
							if ((Copium::Collision::collision_rectrect(bound, velocity, boundB, velocityB) == true))
							{
								boxes[a]->set_vel(Math::Vec2{ 0.0, 0.0 });
								//printf("collide\n");
							}
						
								//printf("no collide\n");
						}
					}

			}
		}
	}
}