#include <pch.h>
#include <GameObject/ecs.h>

namespace Copium
{
	void EntityComponentSystem::CallbackInstantiateEntity(InstantiateEntityEvent* pEvent)
	{
		size_t sparseIndex = entities.Add();
		PRINT("SPARSE INDEX: " << sparseIndex);
		size_t denseIndex = entities.GetDenseIndex(sparseIndex);
		std::stringstream name;
		name << "New GameObject(" << denseIndex << ")";
		entities[sparseIndex].name = name.str();
		activeEntities.set(denseIndex, true);
		if (pEvent->pEntityID)
		{
			*pEvent->pEntityID = denseIndex;
			AddComponent<Transform>(sparseIndex);
		}
	}

	void EntityComponentSystem::CallbackGetEntityByID(GetEntityEvent* pEvent)
	{
		pEvent->pEntity = &entities.DenseGet(pEvent->id);
	}

	void EntityComponentSystem::CallbackGetEntityActive(GetEntityActiveEvent* pEvent)
	{
		pEvent->active = GetActive(pEvent->id);
	}

	void EntityComponentSystem::CallbackSetEntityActive(SetEntityActiveEvent* pEvent)
	{
		SetActive(pEvent->id, pEvent->active);
	}

	void EntityComponentSystem::CallbackGetEntitiesPtr(GetEntitiesEvent* pEvent)
	{
		pEvent->pContainer = &entities;
	}

	void EntityComponentSystem::CallbackSetParent(SetParentEvent* pEvent)
	{
		SetParent(pEvent->childID,pEvent->parentID);
	}


	void EntityComponentSystem::SetParent(EntityID childID, EntityID parentID)
	{
		Transform* parent = GetComponent<Transform>(parentID);
		Transform* child = GetComponent<Transform>(childID);
		Transform* previousParent = GetComponent<Transform>(parentID);
		for (auto it = previousParent->childrenIDs.begin(); 
			it != previousParent->childrenIDs.end(); ++it)
		{
			if (*it == childID)
			{
				std::swap(*it, previousParent->childrenIDs.back());
				previousParent->childrenIDs.pop_back();
				break;
			}
		}
		child->parentID = parentID;
		if (parent)
		{
			parent->childrenIDs.push_back(childID);
		}
	}

	//Delete
	void EntityComponentSystem::DestroyEntity(EntityID idToDelete)
	{
		entities.Delete(idToDelete);
	}

	void EntityComponentSystem::DestroyEntity(Entity* pEntity)
	{
		entities.Delete(pEntity);
	}


	void EntityComponentSystem::Init()
	{
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackInstantiateEntity);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetEntitiesPtr);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackSetParent);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetEntityByID);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackSetEntityActive);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetEntityActive);
		SubscribeComponentCallbacks(AllComponents::Types());
	}

	void EntityComponentSystem::Update()
	{

	}

	void EntityComponentSystem::Exit()
	{

	}
}