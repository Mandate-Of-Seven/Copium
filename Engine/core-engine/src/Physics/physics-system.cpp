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
#include <Debugging/frame-rate-controller.h>
#include "Graphics/graphics-system.h"
#include "Physics/physics-system.h"
#include <GameObject/Components/collider-components.h>
#include <Messaging/message-system.h>


namespace
{
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
		//GameObject* gameobj;
		if (sceneManager.get_current_scene() != nullptr)
		{

			//if (inputSystem.is_key_pressed(GLFW_KEY_P) && inputSystem.is_key_pressed(GLFW_KEY_LEFT_SHIFT))
			//{
			//	PhysicsSystem::toggle_step();
			//}
			//if (stepModeActive == true)
			//{
			//	if (inputSystem.is_key_pressed(GLFW_KEY_0))
			//	{
			//		PhysicsSystem::update_pos();
			//		PhysicsSystem::check_collision();
			//	}
			//}
			//else
			//{
				PhysicsSystem::update_pos();
				PhysicsSystem::check_collision();
				for (GameObject* pGameObj : sceneManager.get_current_scene()->gameObjects)
				{
					Rigidbody2D* pRb = pGameObj->getComponent<Rigidbody2D>();
					if (pRb != nullptr && pRb->Active())
					{
						pGameObj->transform.position.x += pRb->get_vel().x;
						pGameObj->transform.position.y += pRb->get_vel().y;
					}
				}
			//}
		}
	}


	void PhysicsSystem::exit()
	{

	}

	void PhysicsSystem::update_pos()
	{
		float dt = float(FrameRateController::Instance()->getDt());
		GameObject* gameobj;
		Math::Vec2 velocity;
		Math::Vec2 acceleration;
		Math::Vec2 force;

		for (int a = 0; a < sceneManager.get_current_scene()->gameObjects.size(); a++)
		{
			gameobj = sceneManager.get_current_scene()->gameObjects[a];
			Rigidbody2D* pRb = gameobj->getComponent<Rigidbody2D>();

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
						//velocity *= 0.99f;
						pRb->set_acc(acceleration);
						pRb->set_vel(velocity);
						/*pRb->set_force(Math::Vec2());*/
					}
					else
					{
						acceleration = force / pRb->get_mass();
						velocity += (acceleration * dt);
						//velocity *= 0.99f;
						pRb->set_acc(acceleration);
						pRb->set_vel(velocity);
						/*pRb->set_force(Math::Vec2());*/
					}
				}

			}
		}

	}
	void PhysicsSystem::check_collision()
	{
		GameObject* object1;
		GameObject* object2;
		size_t len = sceneManager.get_current_scene()->gameObjects.size();
		for (int a = 0; a < len; a++)
		{
			for (int b = 0; b < len; b++)
			{
				if (a == b)
				{
					continue;
				}
				object1 = sceneManager.get_current_scene()->gameObjects[a];
				object2 = sceneManager.get_current_scene()->gameObjects[b];
				Rigidbody2D* pRb1 = object1->getComponent<Rigidbody2D>();
				Rigidbody2D* pRb2 = object2->getComponent<Rigidbody2D>();
				BoxCollider2D* pCol1 = object1->getComponent<BoxCollider2D>();
				BoxCollider2D* pCol2 = object2->getComponent<BoxCollider2D>();
				if (!pCol1 || !pCol2)
				{
					continue;
				}
				if (!pRb1)
					continue;
				Math::Vec2 velocityA = pRb1->get_vel();
				Math::Vec2 velocityB;
				if (pRb2)
					velocityB = pRb2->get_vel();

				//if (!pRb1->Active())
				//	continue;

				AABB boundA = pCol1->getBounds();
				AABB boundB = pCol2->getBounds();
				if ((collision_rectrect(boundA, velocityA, boundB, velocityB) == true))
				{
					MESSAGE_CONTAINER::collisionEnter.collided = object1;
					MESSAGE_CONTAINER::collisionEnter.collidee = object2;
					MyMessageSystem.dispatch(MESSAGE_TYPE::MT_COLLISION_ENTER);
					//PRINT("COLLIDING?");
					//fix collision resolution
					collisionDirection direct = check_collision_direction(boundA, velocityA, boundB, velocityB);
					resolve_AABBcollision(object1->transform, boundA, boundB, direct);
					if (pRb1)
					{
						if (direct == collisionDirection::BOTTOM)
						{
							pRb1->set_vel(Math::Vec2(velocityA.x ,0.0));
							pRb1->set_acc(Math::Vec2(0.0, 0.0));
						}
						if (direct == collisionDirection::TOP)
						{
							pRb1->set_vel(Math::Vec2(velocityA.x, 0.0));
							pRb1->set_acc(Math::Vec2(0.0, 0.0));
						}
						if (direct == collisionDirection::RIGHT)
						{
							pRb1->set_vel(Math::Vec2(0.0, velocityA.y));
							pRb1->set_acc(Math::Vec2(0.0, 0.0));
						}
						if (direct == collisionDirection::LEFT)
						{
							pRb1->set_vel(Math::Vec2(0.0, velocityA.y));
							pRb1->set_acc(Math::Vec2(0.0, 0.0));
						}
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