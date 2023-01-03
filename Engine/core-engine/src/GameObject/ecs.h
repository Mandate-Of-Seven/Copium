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
        template<typename T>
        void AddComponent();
        template<typename T>
        void RemoveComponent();
        template<typename T>
        bool HasComponent();
    private:
        friend class EntityManager;
    };

    class EntityManager
    {
        SparseSet<Entity, MAX_ENTITIES> entities;
        std::bitset<MAX_ENTITIES> activeEntities;
    public:
        EntityID InstantiateEntity();
        void SetActive(EntityID id, bool active = true) { activeEntities.set(id, active); }
        bool GetActive(EntityID id) { return activeEntities.test(id); }
        void DestroyEntity(EntityID idToDelete);
        void DestroyEntity(Entity* pEntity);
    };
}

#endif // !ECS_H
