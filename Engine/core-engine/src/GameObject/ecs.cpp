#include <pch.h>
#include <GameObject/ecs.h>

namespace Copium
{

	EntityID EntityComponentSystem::InstantiateEntity()
	{
		size_t sparseIndex = entities.Add();
		size_t denseIndex = entities.GetDenseIndex(sparseIndex);
		activeEntities.set(denseIndex, true);
		return denseIndex;
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
}