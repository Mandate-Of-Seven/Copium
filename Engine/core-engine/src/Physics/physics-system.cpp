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
#include "SceneManager/sm.h"

namespace
{
	Copium::InputSystem& inputSystem{*Copium::InputSystem::Instance()};
	Copium::NewSceneManager& sceneManager{ *Copium::NewSceneManager::Instance()};
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
		GameObject* gameobj;
		if (sceneManager.get_current_scene() != nullptr)
		{
			for (size_t a = 0; a < sceneManager.get_current_scene()->get_gameobjectvector().size(); a++)
			{
				gameobj = sceneManager.get_current_scene()->get_gameobjectvector()[a];
				RigidBodyComponent* pRb = gameobj->getComponent<RigidBodyComponent>();
				if (pRb != nullptr)
				{
					PRINT("GAMEOBJ " << gameobj->get_name() << "HAS RIGIDBODY");
					Math::Vec2 position{ gameobj->Transform().position.x, gameobj->Transform().position.y };
					Math::Vec2 size{ gameobj->Transform().scale.x, gameobj->Transform().scale.y };
					pRb->set_active(true);
					if (pRb->get_shape() == SQUARE)
					{
						pRb->set_AABB(Math::Vec2{ position.x - (0.5f * size.x),position.y - (0.5f * size.y) }, Math::Vec2{ position.x + (0.5f * size.x),position.y - (0.5f * size.y) });
					}


					if (inputSystem.is_key_pressed(GLFW_KEY_I)) // move up
					{
						pRb->set_force(Math::Vec2{ 0.0, 0.50 });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_K)) // move down
					{
						pRb->set_force(Math::Vec2{ 0.0, -0.50 });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_L)) // move right
					{
						pRb->set_force(Math::Vec2{ 0.50, 0.0 });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_J)) // move left
					{
						pRb->set_force(Math::Vec2{ -0.50, 0.0 });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_O)) //reset acceleration and velocity values
					{
						pRb->set_force(Math::Vec2{ 0.0f,0.0f });
						pRb->set_acc(Math::Vec2{ 0.0f,0.0f });
						pRb->set_vel(Math::Vec2{ 0.0f,0.0f });
					}
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
			

	}


	void PhysicsSystem::exit()
	{

	}
	void PhysicsSystem::update_pos()
	{
		float dt = float(WindowsSystem::Instance()->get_delta_time());
		GameObject* gameobj;
		Math::Vec2 velocity;
		Math::Vec2 acceleration;
		Math::Vec2 force;
		float mass;
		
		Collision::AABB bound;
		for (int a = 0; a < sceneManager.get_current_scene()->get_gameobjectvector().size(); a++)
		{
			gameobj = sceneManager.get_current_scene()->get_gameobjectvector()[a];
			RigidBodyComponent* pRb = gameobj->getComponent<RigidBodyComponent>();

			if (pRb != nullptr)
			{
				force = pRb->get_force();
				velocity = pRb->get_vel();
				acceleration = pRb->get_acc();
				Math::Vec2 size{ gameobj->Transform().scale.x, gameobj->Transform().scale.y };
				Math::Vec2 position{ gameobj->Transform().position.x, gameobj->Transform().position.y };
				bound = pRb->get_AABB();
				if (pRb->get_mass() != 0)
				{
					if (pRb->get_gravity() == true)
					{
						acceleration = (force + gravity) / pRb->get_mass();
						velocity = velocity + (acceleration * dt * 0.80);
						position = position + (velocity);
						
						pRb->set_acc(acceleration);
						pRb->set_vel(velocity);
						

					}
					else
					{
						acceleration += force / pRb->get_mass();
						velocity = velocity + (acceleration * dt * 0.80);
						position = position + (velocity);
						pRb->set_acc(acceleration);
						pRb->set_vel(velocity);
					}
				}
					if (position.y < -2.0)
					{
						position.y = -2.0;
					}
					if (position.y > 2.0)
					{
						position.y = 2.0;
					}
					if (position.x > 2.0)
					{
						position.x = 2.0;
					}
					if (position.x < -2.0)
					{
						position.x = -2.0;
					}
				bound.max.x = position.x + (size.x * 1 / 2);
				bound.max.y = position.y + (size.y * 1 / 2);
				bound.min.x = position.x - (size.x * 1 / 2);
				bound.min.y = position.y - (size.y * 1 / 2);
				pRb->set_AABB(bound.min, bound.max);
				
				gameobj->Transform().position.x = position.x;
				gameobj->Transform().position.y = position.y;
			
			}
		}

	}
	void PhysicsSystem::check_collision()
	{	
		GameObject* object1;
		GameObject* object2;

		for (int a = 0; a < sceneManager.get_current_scene()->get_gameobjectvector().size(); a++)
		{
			for (int b = 0; b < sceneManager.get_current_scene()->get_gameobjectvector().size(); b++)
			{
				if (a == b)
				{
					continue;
				}
				object1 = sceneManager.get_current_scene()->get_gameobjectvector()[a];
				object2 = sceneManager.get_current_scene()->get_gameobjectvector()[b];
				if (object1->getComponent<RigidBodyComponent>() == nullptr || 
					object2->getComponent<RigidBodyComponent>() == nullptr)
				{

				}
				else
				{
					Math::Vec2 velocityA;
					Math::Vec2 velocityB;
					Math::Vec2 positionA{ object1->Transform().position.x, object1->Transform().position.y };
					Math::Vec2 positionB{ object2->Transform().position.x, object2->Transform().position.y };
					Collision::AABB boundA;
					Collision::AABB boundB;
					
					velocityA = object1->getComponent<RigidBodyComponent>()->get_vel();
					velocityB = object2->getComponent<RigidBodyComponent>()->get_vel();
					boundA = object1->getComponent<RigidBodyComponent>()->get_AABB();
					boundB = object2->getComponent<RigidBodyComponent>()->get_AABB();
					printf("box a:x min %f y min %f \n box a:x max %f y max %f\n", boundA.min.x, boundA.min.y, boundA.max.x, boundA.max.y);
					printf("box b:x min %f b min %f \n box b:x max %f b max %f\n", boundB.min.x, boundB.min.y, boundB.max.x, boundB.max.y);
					if ((Collision::collision_rectrect(boundA, velocityA, boundB, velocityB) == true))
					{
						object1->getComponent<RigidBodyComponent>()->set_vel(Math::Vec2{ 0.0, 0.0 });
						object1->getComponent<RigidBodyComponent>()->set_force(Math::Vec2{ 0.0,0.0 });
					}
				}
			}
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

