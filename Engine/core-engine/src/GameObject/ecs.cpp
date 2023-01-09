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
		//If entity was active
		if (activeEntities.test(pEvent->id))
		{
			pEvent->pEntity = &entities.DenseGet(pEvent->id);
		}
		else
		{
			pEvent->pEntity = nullptr;
		}
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
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetEntityByID);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackSetEntityActive);
		MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetEntityActive);
		SubscribeComponentCallbacks(MainComponents::Types());
	}

	void EntityComponentSystem::Update()
	{

	}

	void EntityComponentSystem::Exit()
	{

	}
}