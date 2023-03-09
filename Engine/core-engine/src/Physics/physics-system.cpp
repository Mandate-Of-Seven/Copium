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
#include <Events/events-system.h>


namespace
{
	Copium::SceneManager& sceneManager{ *Copium::SceneManager::Instance() };
}

namespace Copium
{
	ComponentsArray<Rigidbody2D>* pRigidBodiesArray;
	ComponentsArray<BoxCollider2D>* pBoxCollider2DArray;

	void UpdatePositions()
	{
		double dt{MyFrameRateController.getDt()};
		Math::Vec2 velocity;
		Math::Vec2 acceleration;
		Math::Vec2 force;

		for (Rigidbody2D& rb : *pRigidBodiesArray)
		{
			if (!rb.gameObj.IsActive())
				continue;
			if (!rb.enabled)
				continue;


			force = rb.force;
			velocity = rb.velocity;
			acceleration = rb.acceleration;
			if (rb.mass)
			{
				if (rb.useGravity)
				{
					acceleration = (force + gravity) / rb.mass;
					velocity += (acceleration * (float)dt);
					//velocity *= 0.99f;
					rb.acceleration = acceleration;
					rb.velocity = velocity;
					/*pRb->set_force(Math::Vec2());*/
				}
				else
				{
					acceleration = force / rb.mass;
					velocity += (acceleration * (float)dt);
					//velocity *= 0.99f;
					rb.acceleration = acceleration;
					rb.velocity = velocity;
					/*pRb->set_force(Math::Vec2());*/
				}
			}
		}

	}

	void CheckCollisions()
	{
		double dt{MyFrameRateController.getDt()};
		for (Rigidbody2D& rb : *pRigidBodiesArray)
		{
			GameObject& gameObject = rb.gameObj;
			if (!rb.enabled || !gameObject.IsActive())
				continue;
			BoxCollider2D* pCollider{gameObject.GetComponent<BoxCollider2D>()};
			if (!pCollider || !pCollider->enabled)
				continue;
			//Check with all existing colliders
			for (BoxCollider2D& rhsCol : *pBoxCollider2DArray)
			{
				//Same Collider
				if (pCollider == &rhsCol)
					continue;
				GameObject& rhsGameObject = rhsCol.gameObj;
				if (!rhsCol.enabled|| !rhsGameObject.IsActive())
					continue;
				Math::Vec2 rhsVelocity{};
				Rigidbody2D* pRhsRb{ rhsGameObject.GetComponent<Rigidbody2D>()};
				if (pRhsRb && pRhsRb->enabled)
				{
					rhsVelocity = pRhsRb->velocity;
				}

				//if (!pRb1->Active())
				//	continue;
				Transform& lhsTrans{ gameObject.transform };
				Transform& rhsTrans{ rhsGameObject.transform };
				AABB boundA = pCollider->bounds.GetRelativeBounds(lhsTrans.GetWorldPosition(),lhsTrans.GetWorldScale());
				AABB boundB = rhsCol.bounds.GetRelativeBounds(rhsTrans.GetWorldPosition(), rhsTrans.GetWorldScale());
				if (collision_rectrect(boundA, rb.velocity, boundB, rhsVelocity,dt))
				{
					//MESSAGE_CONTAINER::collisionEnter.collided = object1;
					//MESSAGE_CONTAINER::collisionEnter.collidee = object2;
					//MyMessageSystem.dispatch(MESSAGE_TYPE::MT_COLLISION_ENTER);
					//PRINT("COLLIDING?");
					//fix collision resolution
					collisionDirection direct = check_collision_direction(boundA, rb.velocity, boundB, rhsVelocity);
					resolve_AABBcollision(lhsTrans.position, boundA, boundB, direct);
					if (direct == collisionDirection::BOTTOM || direct == collisionDirection::TOP)
					{
						rb.velocity.y = 0.0;
						rb.acceleration.y = 0.0;
					}
					else if (direct == collisionDirection::RIGHT || direct == collisionDirection::LEFT)
					{
						rb.velocity.x = 0.0;
						rb.acceleration.x = 0.0;
					}
				}
			}
		}
	}
	//AABB floor = { (Math::Vec2{ -0.8f,-0.55f }), (Math::Vec2{ 0.8f,-0.45f }) }; //position of floor
	void PhysicsSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_PLAY;
	}
	void PhysicsSystem::update()
	{
		if (!pRigidBodiesArray)
			return;
		UpdatePositions();
		CheckCollisions();
		for (Rigidbody2D& rb : *pRigidBodiesArray)
		{
			if (!rb.enabled || rb.gameObj.IsActive())
				continue;
			Transform& transform{ rb.gameObj.transform };
			transform.position.x += rb.velocity.x;
			transform.position.y += rb.velocity.y;
		}
	}


	void PhysicsSystem::exit()
	{

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