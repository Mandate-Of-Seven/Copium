#include <pch.h>
#include <GameObject/ecs.h>

namespace Copium
{
	void EntityComponentSystem::CallbackInstantiateEntity(InstantiateEntityEvent* pEvent)
	{
		size_t sparseIndex = entities.entities.Add();
		PRINT("SPARSE INDEX: " << sparseIndex);
		size_t denseIndex = entities.entities.GetDenseIndex(sparseIndex);
		std::stringstream name;
		name << "New GameObject(" << denseIndex << ")";
		entities[sparseIndex].name = name.str();
		entities.SetActive(denseIndex, true);
		if (pEvent->pEntityID)
		{
			*pEvent->pEntityID = denseIndex;
			AddComponent<Transform>(sparseIndex);
		}
	}

	void EntityComponentSystem::CallbackGetEntitiesArray(GetEntitiesArrayEvent* pEvent)
	{
		pEvent->pContainer = &entities;
	}

	void EntityComponentSystem::CallbackSetParent(SetParentEvent* pEvent)
	{
		SetParent(pEvent->childID,pEvent->parentID);
	}


	void EntityComponentSystem::CallbackSwapEntities(SwapEntitiesEvent* pEvent)
	{
		SwapEntities(pEvent->lhs,pEvent->rhs);
	}

	void EntityComponentSystem::SwapEntities(EntityID lhs, EntityID rhs)
	{
		size_t lhsIndex{};
		size_t rhsIndex{};
		bool lhsFound{ false };
		bool rhsFound{ false };
		size_t (&indexes)[MAX_ENTITIES]{entities.entities.GetIndexes()};
		//Get Sparse index of lhs and rhs
		//Swap places of the indexes
		for (size_t i = 0; i < entities.GetSize(); ++i)
		{
			EntityID entityID{indexes[i]};
			if (entityID == lhs)
			{
				lhsFound = true;
				lhsIndex = i;
			}
			else if (entityID == rhs)
			{
				rhsFound = true;
				rhsIndex = i;
			}
			if (lhsFound && rhsFound)
			{
				PRINT("BEFORE: "<< indexes[lhsIndex] <<" AND " << indexes[rhsIndex] << " SWAPPED!");
				indexes[lhsIndex] = rhs;
				indexes[rhsIndex] = lhs;
				PRINT("AFTER: " << indexes[lhsIndex] << " AND " << indexes[rhsIndex] << " SWAPPED!");
				return;
			}
		}
	}

	void EntityComponentSystem::SetParent(EntityID childID, EntityID parentID)
	{
		Transform& parent = components.GetArray<Transform>().FindByID(parentID);
		Transform& child = components.GetArray<Transform>().FindByID(childID);
		Transform& previousParent = parent;
		for (auto it = previousParent.childrenIDs.begin(); 
			it != previousParent.childrenIDs.end(); ++it)
		{
			if (*it == childID)
			{
				std::swap(*it, previousParent.childrenIDs.back());
				previousParent.childrenIDs.pop_back();
				break;
			}
		}
		child.parentID = parentID;
		parent.childrenIDs.push_back(childID);
	}

	//Delete
	void EntityComponentSystem::DestroyEntity(EntityID idToDelete)
	{
		entities.entities.Delete(idToDelete);
	}

	void EntityComponentSystem::DestroyEntity(Entity* pEntity)
	{
		entities.entities.Delete(pEntity);
	}


	void EntityComponentSystem::Init()
	{
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackInstantiateEntity);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetEntitiesArray);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackSwapEntities);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackSetParent);
		SubscribeComponentCallbacks(AllComponents::Types());
	}

	void EntityComponentSystem::Update()
	{

	}

	void EntityComponentSystem::Exit()
	{

	}
}