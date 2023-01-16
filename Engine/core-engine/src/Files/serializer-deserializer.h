#pragma once

#include <CopiumCore/system-interface.h>
#include <Events/events-system.h>
#include <GameObject/components.h>
#include <GameObject/entity.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

namespace Copium
{
	class Serializer
	{
		template <typename T>
		void Serialize(const char* name,T& value)
		{
			PRINT(typeid(T).name() << " does not have a Serialize function for it!");
		}

		void SerializeScene(const char* sceneName)
		{
			SparseSet<Entity, MAX_ENTITIES>* pEntitiesArray;
			MyEventSystem.publish(new GetEntitiesEvent{ pEntitiesArray });
			for (size_t i = 0; i < pEntitiesArray->GetSize(); ++i)
			{
				Entity& entity{ (*pEntitiesArray)[i] };
				EntityID entityID{ &entity - &pEntitiesArray->DenseGet(0)};
				SerializeComponents<AllComponents>(entityID);
			}
		}

		template <typename Component, typename... Components>
		void SerializeComponents(EntityID entityID)
		{
			bool hasComponent{ false };
			MyEventSystem.publish(new HasComponentEvent<Component>{entityID,hasComponent});
			Serialize(typeid(Component).name() + strlen("struct Copium::"));
			if constexpr (sizeof...(Components) != 0)
			{
				SerializeComponents<Components...>(entity);
			}
			else
			{

			}
		}

		template<>
		void Serialize(const char* name,Math::Vec3& value)
		{

		}
		
		template<>
		void Serialize(const char* name, Transform& value)
		{
			Serialize("Pos:", value.position);
		}

		template<>
		void Serialize(const char* name, BoxCollider2D& value)
		{

		}
	private:
		rapidjson::Document* _doc{};
	};

	class Deserializer
	{
		void init() {}
		void update() {}
		void exit() {}
		rapidjson::Document* doc{};
	};
}