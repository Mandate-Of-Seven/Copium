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
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
	Copium::NewSceneManager& sceneManager{ *Copium::NewSceneManager::Instance() };
}

namespace Copium
{
	//AABB floor = { (Math::Vec2{ -0.8f,-0.55f }), (Math::Vec2{ 0.8f,-0.45f }) }; //position of floor
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
				if (pRb != nullptr && pRb->Active())
				{
					Math::Vec2 position{ gameobj->Transform().position.x, gameobj->Transform().position.y };
					Math::Vec2 size{ gameobj->Transform().scale.x, gameobj->Transform().scale.y };

					if (inputSystem.is_key_pressed(GLFW_KEY_I)) // move up
					{
						//pRb->set_vel(Math::Vec2{ 0.0, 0.10 });
						pRb->add_force(Math::Vec2{ 0.0, 30.f });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_K)) // move down
					{
						//pRb->set_vel(Math::Vec2{ 0.0, -0.10 });
						//gameobj->Transform().position.y -= 1.f;
						pRb->add_force(Math::Vec2{ 0.0, -30.f });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_L)) // move right
					{
						//pRb->set_vel(Math::Vec2{ 0.1, 0.0});
						//gameobj->Transform().position.x += 1.f;
						pRb->add_force(Math::Vec2{ 30.f, 0.0 });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_J)) // move left
					{
						//pRb->set_vel(Math::Vec2{ -0.1, 0.0});
						//gameobj->Transform().position.x -= 1.f;
						pRb->add_force(Math::Vec2{ -30.f, 0.0 });
					}
					if (inputSystem.is_key_pressed(GLFW_KEY_O)) //reset acceleration and velocity values
					{
						//pRb->add_force(Math::Vec2{ 0.0f,0.0f });
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
				for (GameObject* pGameObj : sceneManager.get_current_scene()->get_gameobjectvector())
				{
					RigidBodyComponent* pRb = pGameObj->getComponent<RigidBodyComponent>();
					if (pRb != nullptr && pRb->Active())
					{
						pGameObj->Transform().position.x += pRb->get_vel().x;
						pGameObj->Transform().position.y += pRb->get_vel().y;
					}
				}
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

		for (int a = 0; a < sceneManager.get_current_scene()->get_gameobjectvector().size(); a++)
		{
			gameobj = sceneManager.get_current_scene()->get_gameobjectvector()[a];
			RigidBodyComponent* pRb = gameobj->getComponent<RigidBodyComponent>();

			if (pRb != nullptr && pRb->Active())
			{
				force = pRb->get_force();
				velocity = pRb->get_vel();
				acceleration = pRb->get_acc();
				if (pRb->get_mass() != 0)
				{
					if (pRb->get_gravity() == true)
					{
						acceleration = (force + gravity) / pRb->get_mass();
						velocity += (acceleration * dt);
						velocity *= 0.99f;
						pRb->set_acc(acceleration);
						pRb->set_vel(velocity);
						pRb->set_force(Math::Vec2());
					}
					else
					{
						acceleration = force / pRb->get_mass();
						velocity += (acceleration * dt);
						velocity *= 0.99f;
						pRb->set_acc(acceleration);
						pRb->set_vel(velocity);
						pRb->set_force(Math::Vec2());
					}
				}

			}
		}

	}
	void PhysicsSystem::check_collision()
	{
		GameObject* object1;
		GameObject* object2;
		size_t len = sceneManager.get_current_scene()->get_gameobjectvector().size();
		for (int a = 0; a < len; a++)
		{
			for (int b = 0; b < len; b++)
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
					RigidBodyComponent* pRb1 = object1->getComponent<RigidBodyComponent>();
					if (!pRb1->Active())
						continue;
					RigidBodyComponent* pRb2 = object2->getComponent<RigidBodyComponent>();
					Math::Vec2 velocityA = pRb1->get_vel();
					Math::Vec2 velocityB = pRb2->get_vel();
					AABB boundA = pRb1->get_AABB();
					AABB boundB = pRb2->get_AABB();
					if ((collision_rectrect(boundA, velocityA, boundB, velocityB) == true))
					{
						//PRINT("COLLIDING?");
						//fix collision resolution
						collisionDirection direct = check_collision_direction(boundA, velocityA, boundB, velocityB);
						resolve_collision(*object1, *object2, direct);
						pRb1->set_vel(Math::Vec2(0.0, 0.0));
						pRb1->set_acc(Math::Vec2(0.0, 0.0));
						pRb1->set_force(Math::Vec2(0.0, 0.0));
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