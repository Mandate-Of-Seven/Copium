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
		InstantiateEntityEvent(EntityID* pNewEntityID, EntityID _parentID = MAX_ENTITIES) 
			: pEntityID{ pNewEntityID }, parentID{ _parentID } {}
		EntityID* pEntityID;
		EntityID parentID;
	};

	struct GetEntitiesArrayEvent : public IEvent
	{
		GetEntitiesArrayEvent(EntitiesArray*& pEntitiesContainer) : pContainer{ pEntitiesContainer } {}
		EntitiesArray*& pContainer;
	};

	template <typename T>
	struct GetComponentsArrayEvent : public IEvent
	{
		GetComponentsArrayEvent(ComponentsArray<T>*& _pComponents) : pComponents{ _pComponents }{}
		ComponentsArray<T>*& pComponents;
	};

	struct SetParentEvent : public IEvent
	{
		SetParentEvent(EntityID _childID, EntityID _parentID) : childID{ _childID }, parentID{ _parentID }{}
		EntityID childID;
		EntityID parentID;
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

	struct SwapEntitiesEvent : public IEvent
	{
		SwapEntitiesEvent(EntityID _lhs, EntityID  _rhs) : lhs{ _lhs }, rhs{ _rhs }{}
		EntityID lhs;
		EntityID rhs;
	};

	struct GetDeltaTimeEvent : public IEvent
	{
		GetDeltaTimeEvent(float& _dt) : dt{_dt} {}
		float& dt;
	};

	struct GetFixedDeltaTimeEvent : public IEvent
	{
		GetFixedDeltaTimeEvent(float& _fdt) : fdt{ _fdt } {}
		float& fdt;
	};

	struct GetGameCameraEvent : public IEvent
	{
		GetGameCameraEvent(Camera*& _pCamera): pCamera{ _pCamera }{}
		Camera*& pCamera;
	};
}