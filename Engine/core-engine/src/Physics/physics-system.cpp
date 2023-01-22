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
#include "Physics/physics-system.h"
#include <GameObject/components.h>
#include <Events/events-system.h>

namespace Copium
{
	AABB GetBoxBounds(EntityID entityID, BoxCollider2D& collider)
	{
		ComponentsArray<Transform>* pTransformsArray{};
		MyEventSystem.publish(new GetComponentsArrayEvent{ pTransformsArray });
		Transform& transform{ pTransformsArray->FindByID(entityID) };

		Math::Vec3 size{ transform.scale };
		Math::Vec3 pos{ transform.position };

		if (transform.HasParent())
		{
			Transform& parent{ pTransformsArray->FindByID(transform.parentID) };
			size.x *= parent.scale.x;
			size.y *= parent.scale.y;
			size.z *= parent.scale.z;
			pos += parent.position;
		}

		float x = (collider.bounds.max.x - collider.bounds.min.x) * size.x;
		float y = (collider.bounds.max.y - collider.bounds.min.y) * size.y;
		AABB tmp{ collider.bounds };
		tmp.max.x *= x;
		tmp.min.x *= x;
		tmp.max.y *= y;
		tmp.min.y *= y;
		tmp.max.x += pos.x;
		tmp.min.x += pos.x;
		tmp.max.y += pos.y;
		tmp.min.y += pos.y;
		return tmp;
	}

	void UpdatePositions(ComponentsArray<Rigidbody2D>& rigidBodies, EntitiesArray& entities)
	{
		float dt{};
		MyEventSystem.publish(new GetDeltaTimeEvent{ dt });
		Math::Vec2 velocity;
		Math::Vec2 acceleration;
		Math::Vec2 force;

		for (int a = 0; a < rigidBodies.GetSize(); a++)
		{
			Rigidbody2D& rb{ rigidBodies[a] };
			EntityID entityID{ rigidBodies.GetID(rb) };
			if (!entities.GetActive(entityID))
				continue;
			if (!rigidBodies.GetEnabled(entityID))
				continue;


			force = rb.force;
			velocity = rb.velocity;
			acceleration = rb.acceleration;
			if (rb.mass)
			{
				if (rb.useGravity)
				{
					acceleration = (force + gravity) / rb.mass;
					velocity += (acceleration * dt);
					//velocity *= 0.99f;
					rb.acceleration = acceleration;
					rb.velocity = velocity;
					/*pRb->set_force(Math::Vec2());*/
				}
				else
				{
					acceleration = force / rb.mass;
					velocity += (acceleration * dt);
					//velocity *= 0.99f;
					rb.acceleration = acceleration;
					rb.velocity = velocity;
					/*pRb->set_force(Math::Vec2());*/
				}
			}
		}

	}

	void CheckCollisions(ComponentsArray<Rigidbody2D>& rigidBodies, EntitiesArray& entities)
	{
		ComponentsArray<Transform>* pTransformsArray{};
		MyEventSystem.publish(new GetComponentsArrayEvent{ pTransformsArray });
		ComponentsArray<BoxCollider2D>* pBoxColliders{};
		MyEventSystem.publish(new GetComponentsArrayEvent(pBoxColliders));
		ComponentsArray<BoxCollider2D>& boxColliders{*pBoxColliders};
		float dt{};
		MyEventSystem.publish(new GetDeltaTimeEvent{ dt });
		for (int a = 0; a < rigidBodies.GetSize(); a++)
		{
			Rigidbody2D& lhsRb = rigidBodies[a];
			EntityID lhsID{rigidBodies.GetID(lhsRb)};
			if (!entities.GetActive(lhsID))
				continue;
			if (!rigidBodies.GetEnabled(lhsID))
				continue;
			bool hasCollider{ false };
			MyEventSystem.publish(new HasComponentEvent<BoxCollider2D>(lhsID,hasCollider));
			if (!hasCollider)
				continue;
			if (!boxColliders.GetEnabled(lhsID))
				continue;
			BoxCollider2D& lhsCol = boxColliders.FindByID(lhsID);
			Transform& lhsTransform{ pTransformsArray->FindByID(lhsID)};
			//Check with all existing colliders
			for (int b = 0; b < boxColliders.GetSize(); b++)
			{
				BoxCollider2D& rhsCol = boxColliders[b];
				EntityID rhsID{boxColliders.GetID(rhsCol)};
				if (lhsID == rhsID)
					continue;
				if (!entities.GetActive(rhsID))
					continue;
				if (!boxColliders.GetEnabled(rhsID))
					continue;
				Math::Vec2 rhsVelocity{};
				bool hasRigidBody{false};
				MyEventSystem.publish(new HasComponentEvent<Rigidbody2D>(rhsID, hasRigidBody));
				if (hasRigidBody && rigidBodies.GetEnabled(rhsID))
				{
					rhsVelocity = rigidBodies.FindByID(rhsID).velocity;
				}

				//if (!pRb1->Active())
				//	continue;

				AABB boundA = GetBoxBounds(lhsID, lhsCol);
				AABB boundB = GetBoxBounds(rhsID, rhsCol);
				if (collision_rectrect(boundA, lhsRb.velocity, boundB, rhsVelocity, dt))
				{
					//MESSAGE_CONTAINER::collisionEnter.collided = object1;
					//MESSAGE_CONTAINER::collisionEnter.collidee = object2;
					//MyMessageSystem.dispatch(MESSAGE_TYPE::MT_COLLISION_ENTER);
					//PRINT("COLLIDING?");
					//fix collision resolution
					collisionDirection direct = check_collision_direction(boundA, lhsRb.velocity, boundB, rhsVelocity);
					resolve_AABBcollision(lhsTransform.position, boundA, boundB, direct);
					if (direct == collisionDirection::BOTTOM || direct == collisionDirection::TOP)
					{
						lhsRb.velocity.y = 0.0;
						lhsRb.acceleration.y = 0.0;
					}
					else if (direct == collisionDirection::RIGHT || direct == collisionDirection::LEFT)
					{
						lhsRb.velocity.x = 0.0;
						lhsRb.acceleration.x = 0.0;
					}
				}
			}
		}
	}
	//AABB floor = { (Math::Vec2{ -0.8f,-0.55f }), (Math::Vec2{ 0.8f,-0.45f }) }; //position of floor
	void PhysicsSystem::Init()
	{
		systemFlags |= FLAG_RUN_ON_PLAY;
	}
	void PhysicsSystem::Update()
	{
		ComponentsArray<Rigidbody2D>* pRigidBodiesArray{};
		MyEventSystem.publish(new GetComponentsArrayEvent{ pRigidBodiesArray });
		ComponentsArray<Rigidbody2D>& rigidBodiesArray{ *pRigidBodiesArray };
		EntitiesArray* pEntitiesArray{};
		MyEventSystem.publish(new GetEntitiesArrayEvent{ pEntitiesArray });
		ComponentsArray<Transform>* pTransformsArray{};
		MyEventSystem.publish(new GetComponentsArrayEvent{ pTransformsArray });

		UpdatePositions(rigidBodiesArray,*pEntitiesArray);
		CheckCollisions(rigidBodiesArray, *pEntitiesArray);
		for (size_t i = 0; i < rigidBodiesArray.GetSize(); ++i)
		{
			Rigidbody2D& rb{ rigidBodiesArray[i] };
			EntityID entityID{ rigidBodiesArray.GetID(rb) };
			if (!pEntitiesArray->GetActive(entityID))
				continue;
			if (!rigidBodiesArray.GetEnabled(entityID))
				continue;
			Transform& transform{ pTransformsArray->FindByID(entityID)};
			transform.position.x += rb.velocity.x;
			transform.position.y += rb.velocity.y;
		}
	}


	void PhysicsSystem::Exit()
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