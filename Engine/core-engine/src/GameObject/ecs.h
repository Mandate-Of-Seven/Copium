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
        EntitiesArray entities;
        //SparseSet<Entity, MAX_ENTITIES> backUpEntities;
        AllComponents components;
        //Parent to Child
        std::unordered_map<EntityID,std::vector<EntityID>> entityRelationships;
        //std::bitset<MAX_ENTITIES> activeBackUpEntities;
    public:
        void Init();
        void Update();
        void Exit();
    private:
        void SetParent(EntityID childId, EntityID parentID);
        void DestroyEntity(EntityID idToDelete);
        void DestroyEntity(Entity* pEntity);
        void SwapEntities(EntityID lhs, EntityID rhs);
        template<typename T>
        T* AddComponent(EntityID id);
        template<typename T>
        void RemoveComponent(EntityID id);
        template<typename T>
        bool HasComponent(EntityID id);

        //Events Callbacks
        void CallbackGetEntitiesArray(GetEntitiesArrayEvent* pEvent);
        void CallbackInstantiateEntity(InstantiateEntityEvent* pEvent);
        void CallbackSwapEntities(SwapEntitiesEvent* pEvent);
        void CallbackSetParent(SetParentEvent* pEvent);
        template <typename T>
        void CallbackGetComponentsArray(GetComponentsArrayEvent<T>* pEvent);
        template <typename T>
        void CallbackRemoveComponent(RemoveComponentEvent<T>* pEvent);
        template <typename T>
        void CallbackHasComponent(HasComponentEvent<T>* pEvent);
        template <typename T>
        void CallbackAddComponent(AddComponentEvent<T>* pEvent);
        void SubscribeComponentCallbacks() {}
        template <typename Component, typename... Components>
        void SubscribeComponentCallbacks(Pack<Component, Components...> components);
    };

    template <typename T>
    T* EntityComponentSystem::AddComponent(EntityID id)
    {
        static_assert(AllComponents::Has<T>());
        COPIUM_ASSERT(!entities.entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        entities.entities.DenseGet(id).componentsBitset.set(GetComponentType<T>::e);
        ComponentsArray<T>& componentArray{ components.GetArray<T>() };
        size_t sparseID = componentArray.components.AddFromDenseIndex(id);
        componentArray.SetEnabled(id, true);
        return &componentArray.components.DenseGet(id);
    }

    template <typename T>
    void EntityComponentSystem::RemoveComponent(EntityID id)
    {
        static_assert(AllComponents::Has<T>());
        COPIUM_ASSERT(!entities.entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        COPIUM_ASSERT(!HasComponent<T>(id), typeid(T).name());
        entities.entities.DenseGet(id).componentsBitset.set(GetComponentType<T>::e, 0);
        ComponentsArray<T>& typedComponents { components.GetArray<T>() };
        typedComponents.components.Delete(&typedComponents.components.DenseGet(id));
    }
    
    template <typename T>
    bool EntityComponentSystem::HasComponent(EntityID id)
    {
        static_assert(AllComponents::Has<T>());
        COPIUM_ASSERT(!entities.entities.DenseExists(id), "ENTITY DOES NOT EXIST");
        return entities.entities.DenseGet(id).componentsBitset.test(GetComponentType<T>::e);
    }

    template <typename T>
    void EntityComponentSystem::CallbackGetComponentsArray(GetComponentsArrayEvent<T>* pEvent)
    {
        pEvent->pComponents = &components.GetArray<T>();
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
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackGetComponentsArray<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackRemoveComponent<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackAddComponent<Component>);
        MyEventSystem.subscribe(this, &EntityComponentSystem::CallbackHasComponent<Component>);
        if constexpr (sizeof...(Components) == 0)
        {
            return;
        }
        else
        {
            SubscribeComponentCallbacks(Pack<Components...>());
        }
    }
}

#endif // !ECS_H
