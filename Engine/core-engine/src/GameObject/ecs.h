#ifndef ECS_H
#define ECS_H

#include <string>
#include <bitset>
#include <CopiumCore/system-interface.h>
#include <Debugging/logging-system.h>
#include <CopiumCore/system-interface.h>

#include <GameObject/components.h>
#include <Utilities/sparse-set.h>
#include <GameObject/entity.h>
#include <Events/events-system.h>

#define MyECS Copium::EntityComponentSystem::Instance()

namespace Copium
{
    CLASS_SYSTEM(EntityComponentSystem)
    {
        SparseSet<Entity, MAX_ENTITIES> entities;
        //SparseSet<Entity, MAX_ENTITIES> backUpEntities;
        std::bitset<MAX_ENTITIES> activeEntities;
        //std::bitset<MAX_ENTITIES> activeBackUpEntities;
    public:
        void Init();
        void Update();
        void Exit();
    private:
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

        //Events Callbacks
        void CallbackGetEntitiesPtr(GetEntitiesEvent* pEvent);
        void CallbackInstantiateEntity(InstantiateEntityEvent* pEvent);
        void CallbackGetEntityByID(GetEntityEvent* pEvent);
        void CallbackGetEntityActive(GetEntityActiveEvent* pEvent);
        void CallbackSetEntityActive(SetEntityActiveEvent* pEvent);
        template <typename T>
        void CallbackGetComponent(GetComponentEvent<T>* pEvent);
        template <typename T>
        void CallbackRemoveComponent(RemoveComponentEvent<T>* pEvent);
        template <typename T>
        void CallbackHasComponent(HasComponentEvent<T>* pEvent);
        template <typename T>
        void CallbackAddComponent(AddComponentEvent<T>* pEvent);
        template <typename T>
        void CallbackGetComponentEnabled(GetComponentEnabledEvent<T>* pEvent);
        template <typename T>
        void CallbackSetComponentEnabled(SetComponentEnabledEvent<T>* pEvent);

        void SubscribeComponentCallbacks() {}
        template <typename Component, typename... Components>
        void SubscribeComponentCallbacks(Pack<Component, Components...> components);
    };

    template <typename T>
    T* EntityComponentSystem::AddComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(!entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        entities.DenseGet(id).componentsBitset.set(GetComponentType<T>::e);
        return &MainComponents::GetArray<T>()[id];
    }

    template <typename T>
    T* EntityComponentSystem::GetComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(!entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        if (entities.DenseGet(id).componentsBitset.test(GetComponentType<T>::e))
        {
            //PRINT("Found Component " << typeid(T).name() << "of ID: " << id);
            return &MainComponents::GetArray<T>()[id];
        }
        return nullptr;
    }

    template <typename T>
    void EntityComponentSystem::RemoveComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(!entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        COPIUM_ASSERT(!HasComponent<T>(id), typeid(T).name());
        entities.DenseGet(id).componentsBitset.set(GetComponentType<T>::e, 0);
    }
    
    template <typename T>
    bool EntityComponentSystem::HasComponent(EntityID id)
    {
        static_assert(MainComponents::Has<T>());
        COPIUM_ASSERT(!entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        return entities.DenseGet(id).componentsBitset.test(GetComponentType<T>::e);
    }

    template <typename T>
    void EntityComponentSystem::CallbackGetComponent(GetComponentEvent<T>* pEvent)
    {
        pEvent->pComponent = GetComponent<T>(pEvent->id);
    }

    template <typename T>
    void EntityComponentSystem::CallbackRemoveComponent(RemoveComponentEvent<T>* pEvent)
    {
        RemoveComponent<T>(pEvent->id);
    }

    template <typename T>
    void EntityComponentSystem::CallbackHasComponent(HasComponentEvent<T>* pEvent)
    {
        pEvent->exists = HasComponent<T>(pEvent->id);
    }

    template <typename T>
    void EntityComponentSystem::CallbackAddComponent(AddComponentEvent<T>* pEvent)
    {
        pEvent->pComponent = AddComponent<T>(pEvent->id);
    }

    template <typename Component, typename... Components>
    void EntityComponentSystem::SubscribeComponentCallbacks(Pack<Component, Components...> components)
    {
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetComponent<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackRemoveComponent<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackAddComponent<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackHasComponent<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetComponentEnabled<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackSetComponentEnabled<Component>);
        if constexpr (sizeof...(Components) == 0)
        {
            return;
        }
        else
        {
            SubscribeComponentCallbacks(Pack<Components...>());
        }
    }

    template <typename T>
    void EntityComponentSystem::CallbackGetComponentEnabled(GetComponentEnabledEvent<T>* pEvent)
    {
        MainComponents::ComponentEnabled<T>(pEvent->id, &pEvent->enabled, false);
    }

    template <typename T>
    void EntityComponentSystem::CallbackSetComponentEnabled(SetComponentEnabledEvent<T>* pEvent)
    {
        bool val = pEvent->enabled;
        MainComponents::ComponentEnabled<T>(pEvent->id, &val, true);
    }
}

#endif // !ECS_H
