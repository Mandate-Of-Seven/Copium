/*!***************************************************************************************
\file			physics-system.cpp
\project
\author			Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			23/09/2022

\brief
	This file contains the function definitions for a physics system. It will perform
	physics and collision on a collider component of an object.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include "Graphics/graphics-system.h"
#include "Physics/physics-system.h"

namespace
{
	Copium::InputSystem& inputSystem{*Copium::InputSystem::Instance()};
}

namespace Copium
{
	//Collision::AABB floor = { (Math::Vec2{ -0.8f,-0.55f }), (Math::Vec2{ 0.8f,-0.45f }) }; //position of floor
	void PhysicsSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_PLAY;
	}
	void PhysicsSystem::update()
	{
		static int count = 0;
		if (inputSystem.is_key_pressed(GLFW_KEY_B) && (boxes.size() < GraphicsSystem::Instance()->get_sprites().size()))
		{
			glm::vec2 position;
			glm::vec2 size;
			Math::Vec2 convertedPos;
			Math::Vec2 convertedSize;
			size = GraphicsSystem::Instance()->get_sprites()[count]->get_size();
			convertedSize = Math::Vec2(size.x, size.y);
			RigidBodyComponent* box = new RigidBodyComponent;
			position = GraphicsSystem::Instance()->get_sprites()[count]->get_position();
			convertedPos = {position};
			box->set_vel(Math::Vec2{ 0.0, 0.0 });
			box->set_acc(Math::Vec2{ 0.0,0.0 });
			box->set_force(Math::Vec2{ 0.0,0.0 });
			box->set_mass(1.0);
			if (count == 0)
			{
				box->set_gravity(false);
			}
			else
			{
				box->set_gravity(true);
			}
			count++;
			

			box->set_shape(SQUARE);
			box->set_active(true);
			if (box->get_shape() == SQUARE)
			{
				box->set_AABB(Math::Vec2{ convertedPos.x - (convertedSize.x / 2),convertedPos.y - (convertedSize.y / 2) }, Math::Vec2{ convertedPos.x + (convertedSize.x / 2),convertedPos.y + (convertedSize.y / 2) });
			}
			boxes.push_back(box);

		}
		if (boxes.size() > 0)
		{	
			
			if (inputSystem.is_key_pressed(GLFW_KEY_I)) // move up
			{
				boxes[0]->set_force(Math::Vec2{ 0.0, 0.50 });
				std::cout << "a\n";
			}
			if (inputSystem.is_key_pressed(GLFW_KEY_K)) // move down
			{
				boxes[0]->set_force(Math::Vec2{ 0.0, -0.50 });
				std::cout << "a\n";
			}
			if (inputSystem.is_key_pressed(GLFW_KEY_L)) // move left
			{
				boxes[0]->set_force(Math::Vec2{ -0.50, 0.0 });
				std::cout << "a\n";
			}
			if (inputSystem.is_key_pressed(GLFW_KEY_J)) // move right
			{
				boxes[0]->set_force(Math::Vec2{ 0.50, 0.0 });
				std::cout << "a\n";
			}
			if (inputSystem.is_key_pressed(GLFW_KEY_O)) //reset acceleration and velocity values
			{
				boxes[0]->set_acc(Math::Vec2{ 0.0f,0.0f });
				boxes[0]->set_vel(Math::Vec2{ 0.0f,0.0f });
				std::cout << "a\n";
			}
		
		}
		if (inputSystem.is_key_pressed(GLFW_KEY_P) && inputSystem.is_key_pressed(GLFW_KEY_LEFT_SHIFT))
			{
			PhysicsSystem::toggle_step();
			}
		if (stepModeActive == true)
		{
			if (inputSystem.is_key_pressed(GLFW_KEY_0))
			{
				PhysicsSystem::update_pos();
				PhysicsSystem::check_collision();
			}
		}
		else
		{
			PhysicsSystem::update_pos();
			PhysicsSystem::check_collision();
		}
			

	}


	void PhysicsSystem::exit()
	{
		for (RigidBodyComponent* a : boxes)
		{
			delete a;
		}
	}
	void PhysicsSystem::update_pos()
	{
		float dt = float(WindowsSystem::Instance()->get_delta_time());
		Math::Vec2 velocity;
		Math::Vec2 acceleration;
		Math::Vec2 force;
		glm::vec3 glmPosition;
		glm::vec2 glmSize;
		Math::Vec2 position;
		Collision::AABB bound;
		for (int a = 0; a < boxes.size(); a++)
		{
			force = boxes[a]->get_force();
			velocity = boxes[a]->get_vel();
			acceleration = boxes[a]->get_acc();
			glmPosition = GraphicsSystem::Instance()->get_sprites()[a]->get_position();
			glmSize = GraphicsSystem::Instance()->get_sprites()[a]->get_size();
			position = Math::Vec2(glmPosition.x, glmPosition.y);
			bound = boxes[a]->get_AABB();
			if (boxes[a]->get_gravity() == true)
			{
				acceleration = (force + gravity)/ boxes[a]->get_mass();
				velocity = velocity + (acceleration * dt * 0.90);
				position = position + (velocity * dt);
				boxes[a]->set_acc(acceleration);
				boxes[a]->set_vel(velocity);
				
			}
			else
			{	
				acceleration = force / boxes[a]->get_mass();
				velocity = velocity + (acceleration * dt * 0.90);
				position = position + (velocity * dt);
				boxes[a]->set_acc(acceleration);
				boxes[a]->set_vel(velocity);
			}
			bound.max.x = position.x + (glmSize.x * 1 / 2);
			bound.max.y = position.y + (glmSize.y * 1 / 2);
			bound.min.x = position.x - (glmSize.x * 1 / 2);
			bound.min.y = position.y - (glmSize.y * 1 / 2);
			boxes[a]->set_AABB(bound.min, bound.max);
			glmPosition = { position.x, position.y, 0.f };
			GraphicsSystem::Instance()->get_sprites()[a]->set_position(glmPosition);
			
		}

	}
	void PhysicsSystem::check_collision()
	{	
		for (int a = 0; a < boxes.size(); a++)
		{
			
			Math::Vec2 velocity;
			glm::vec2 glmPosition;
			Math::Vec2 position;
			Collision::AABB bound;
			velocity = boxes[a]->get_vel();
			glmPosition = GraphicsSystem::Instance()->get_sprites()[a]->get_position();
			position = Math::Vec2(glmPosition.x, glmPosition.y);
			bound = boxes[a]->get_AABB();
			/*if ((Collision::collision_rectrect(bound, velocity, floor, Math::Vec2{ 0.0,0.0 }) == true))
			{
				boxes[a]->set_vel(Math::Vec2{ 0.0, 0.0 });
				boxes[a]->set_force(Math::Vec2{ 0.0,0.0 });
				if (boxes[a]->get_gravity() == true)
				{
					boxes[a]->set_gravity(false);
				}
			}*/
		}
	}

	void PhysicsSystem::toggle_step()
	{
		stepModeActive = !stepModeActive;
		if (stepModeActive == true)
		{
			printf("Physics in step-by-step mode\n");
		}
		else
		{
			printf("Physics in normal mode\n");
		}
	}
}

