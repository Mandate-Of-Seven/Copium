#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <config.h>
#include <bitset>
#include <Utilities/sparse-set.h>

namespace Copium
{
    struct Entity
    {
        std::string name;
    private:
        friend class EntityComponentSystem;
        std::bitset<MAX_COMPONENTS> componentsBitset{};
    };

	class EntitiesArray
	{
		std::bitset<MAX_ENTITIES> activeEntities{};
		SparseSet<Entity, MAX_ENTITIES> entities;
		friend class EntityComponentSystem;
	public:
		void SetActive(EntityID entityID, bool val)
		{
			activeEntities.set(entityID, val);
		}

		bool GetActive(EntityID entityID)
		{
			return activeEntities.test(entityID);
		}

		Entity& operator[](size_t index)
		{
			return entities[index];
		}

		Entity& FindByID(EntityID entityID)
		{
			return entities.DenseGet(entityID);
		}

		EntityID GetID(const Entity& entity)
		{
			return(&entity - &entities.DenseGet(0));
		}

		size_t GetSize() const
		{
			return entities.GetSize();
		}
	};
};

#endif // !ENTITY_H