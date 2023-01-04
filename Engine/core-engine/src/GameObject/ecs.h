#ifndef ECS_H
#define ECS_H

#include <string>
#include <bitset>
#include <map>
#include <CopiumCore/system-interface.h>
#include <Debugging/logging-system.h>

#include <GameObject/components.h>
//#include <GameObject/Components/transform-component.h>
#include <Utilities/sparse-set.h>



#define MAX_COMPONENTS 16
#define MAX_ENTITIES 100000

namespace Copium
{
    template<typename T, typename... Ts>
    constexpr bool contains()
    {
        return std::disjunction_v<std::is_same<T, Ts>...>;
    }

    template<size_t ID,typename... Component>
    struct ComponentGroup
    {
        template <typename T>
        static constexpr bool Has()
        {
            return contains<T, Component...>();
        }

        template <typename T>
        static SparseSet<T,MAX_ENTITIES>& GetArray()
        {
            static_assert(contains<T, Component...>());
            static SparseSet<T, MAX_ENTITIES> components;
            return components;
        }
    };

    using MainComponents    =   ComponentGroup<0, Transform>;
    using BackupComponents  =   ComponentGroup<1, Transform>;

    using EntityID = size_t;

    struct Entity
    {
        std::string name;
    private:
        friend class EntityManager;
        std::bitset<MAX_COMPONENTS> componentsBitset;
    };

    class EntityComponentSystem
    {
        SparseSet<Entity, MAX_ENTITIES> entities;
        std::bitset<MAX_ENTITIES> activeEntities;
    public:
        EntityID InstantiateEntity();
        void SetActive(EntityID id, bool active = true) { activeEntities.set(id, active); }
        bool GetActive(EntityID id) { return activeEntities.test(id); }
        void DestroyEntity(EntityID idToDelete);
        void DestroyEntity(Entity* pEntity);
        template<typename T>
        T* AddComponent(EntityID id);
        template<typename T>
        T* GetComponent(EntityID id);
        template<typename T>
        void RemoveComponent(EntityID id);
        template<typename T>
        bool HasComponent(EntityID id);
    };

    template <typename T>
    T* EntityComponentSystem::AddComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        entities.DenseGet(id).componentsBitset.set(GetComponentType<T>());
        return &MainComponents::GetArray<T>()[id];
    }

    template <typename T>
    T* EntityComponentSystem::GetComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        if (entities.DenseGet(id).componentsBitset.test(GetComponentType<T>()))
        {
            return &MainComponents::GetArray<T>()[id];
        }
        return nullptr;
    }

    template <typename T>
    void EntityComponentSystem::RemoveComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        COPIUM_ASSERT(HasComponent<T>(id), typeid(T).name());
        entities.DenseGet(id).componentsBitset.set(GetComponentType<T>(), 0);
    }
    
    template <typename T>
    bool EntityComponentSystem::HasComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        return entities.DenseGet(id).componentsBitset.test(GetComponentType<T>());
    }
}

#endif // !ECS_H
