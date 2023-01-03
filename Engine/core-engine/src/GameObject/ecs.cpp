#include <pch.h>
#include <GameObject/ecs.h>

namespace Copium
{
	//bool Entity::IsActive()
	//{
	//	return componentBitset.test(0);
	//}

	EntityID EntityManager::InstantiateEntity()
	{
		size_t sparseIndex = entities.Add();
		size_t denseIndex = entities.GetDenseIndex(sparseIndex);
		activeEntities.set(denseIndex, true);
		return denseIndex;
	}

	//Delete
	void EntityManager::DestroyEntity(EntityID idToDelete)
	{
		entities.Delete(idToDelete);
	}

	void EntityManager::DestroyEntity(Entity* pEntity)
	{
		entities.Delete(pEntity);
	}
}