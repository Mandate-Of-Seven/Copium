#pragma once

#include <GameObject/entity.h>
#include <GameObject/components.h>
#include <Utilities/sparse-set.h>

namespace Copium
{

	struct IEvent
	{
	protected:
		virtual ~IEvent() {};
	};

	struct CollisionEvent : public IEvent 
	{
		CollisionEvent(Entity& a, Entity& b) : gameObjA{ a }, gameObjB{ b } {}
		Entity& gameObjA;
		Entity& gameObjB;
	};

	struct InstantiateEntityEvent : public IEvent
	{
		InstantiateEntityEvent(EntityID* pNewEntityID) : pEntityID{pNewEntityID} {}
		EntityID* pEntityID;
	};

	struct GetEntitiesEvent : public IEvent
	{
		GetEntitiesEvent(SparseSet<Entity,MAX_ENTITIES>*& pEntitiesContainer) : pContainer{ pEntitiesContainer } {}
		SparseSet<Entity, MAX_ENTITIES>*& pContainer;
	};

	struct GetEntityEvent : public IEvent
	{
		GetEntityEvent(EntityID _id, Entity*& _pEntity) : id{_id}, pEntity{_pEntity}{}
		EntityID id;
		Entity*& pEntity;
	};

	struct GetEntityActiveEvent : public IEvent
	{
		GetEntityActiveEvent(EntityID _id, bool& _active) : id{_id}, active{_active}{}
		EntityID id;
		bool& active;
	};

	struct SetEntityActiveEvent : public IEvent
	{
		SetEntityActiveEvent(EntityID _id, bool _active) : id{ _id }, active{ _active }{}
		EntityID id;
		bool active;
	};

	template <typename T>
	struct GetComponentsEvent : public IEvent
	{
		GetComponentsEvent(ComponentsArray<T>*& _pComponents) : pComponents{ _pComponents }{}
		ComponentsArray<T>*& pComponents;
	};

	template <typename T>
	struct GetComponentEvent : public IEvent
	{
		GetComponentEvent(EntityID _id, T*& _pComponent) : id{ _id }, pComponent{ _pComponent }{}
		EntityID id; 
		T*& pComponent;
	};

	struct SetParentEvent : public IEvent
	{
		SetParentEvent(EntityID _childID, EntityID _parentID) : childID{ _childID }, parentID{ _parentID }{}
		EntityID childID;
		EntityID parentID;
	};

	template <typename T>
	struct GetEntityFromComponentEvent : public IEvent
	{
		GetEntityFromComponentEvent(EntityID& _entityId, T& _component) : entityId{ _entityId }, component{ _component }{}
		EntityID& entityId;
		T& component;
	};

	template <typename T>
	struct AddComponentEvent : public IEvent
	{
		AddComponentEvent(EntityID _id, T*& _pComponent) : id{ _id }, pComponent{ _pComponent }{}
		EntityID id;
		T*& pComponent;
	};

	template <typename T>
	struct RemoveComponentEvent : public IEvent
	{
		RemoveComponentEvent(EntityID _id) : id{ _id }{}
		EntityID id;
	};

	template <typename T>
	struct HasComponentEvent : public IEvent
	{
		HasComponentEvent(EntityID _id, bool& _exists) : id{ _id }, exists{_exists} {}
		EntityID id;
		bool& exists;
	};

	template <typename T>
	struct GetComponentEnabledEvent : public IEvent
	{
		GetComponentEnabledEvent(EntityID _id, bool& _enabled) : id{ _id }, enabled{ _enabled }{}
		EntityID id;
		bool& enabled;
	};

	template <typename T>
	struct SetComponentEnabledEvent : public IEvent
	{
		SetComponentEnabledEvent(EntityID _id, bool _enabled) : id{ _id }, enabled{ _enabled }{}
		EntityID id;
		bool enabled;
	};

	struct SwapEntitiesEvent : public IEvent
	{
		SwapEntitiesEvent(EntityID _lhs, EntityID  _rhs) : lhs{ _lhs }, rhs{ _rhs }{}
		EntityID lhs;
		EntityID rhs;
	};

}