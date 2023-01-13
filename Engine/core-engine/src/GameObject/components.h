#pragma once

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Physics/collision.h>
#include <imgui.h>
#include <Utilities/sparse-set.h>
#include <config.h>
#include <Graphics/textures.h>


namespace Copium
{
	using ComponentID = size_t;

	enum class ComponentType : size_t      // Types of Components
	{
		Transform = 0,
		Animator,
		AudioSource,
		BoxCollider2D,
		Button,
		Camera,
		Image,
		Rigidbody2D,
		SpriteRenderer,
		Script,
		Text,
		None
	};

	static std::map<ComponentType, std::string> MAP_COMPONENT_TYPE_NAME
	{
		{ComponentType::Animator,"Animator"},
		{ComponentType::AudioSource,"AudioSource"},
		{ComponentType::BoxCollider2D,"BoxCollider2D"},
		{ComponentType::Button,"Button"},
		{ComponentType::Camera,"Camera"},
		{ComponentType::Image,"ImageComponent"},
		{ComponentType::Rigidbody2D,"Rigidbody2D"},
		{ComponentType::SpriteRenderer,"SpriteRenderer"},
		{ComponentType::Script,"Script"},
		{ComponentType::Button,"Button"},
		{ComponentType::Text,"Text"},
	};

	template<typename T, typename... Ts>
	constexpr bool contains()
	{
		return std::disjunction_v<std::is_same<T, Ts>...>;
	}

	template <typename... Ts>
	struct Pack {};

	template <typename Component>
	using ComponentsArray = SparseSet<Component, MAX_ENTITIES>;
	using ComponentsBitset = std::bitset<MAX_ENTITIES>;

	template<typename Component, typename... Components>
	struct ComponentGroup
	{
		using Types = Pack<Component,Components...>;

		ComponentsArray<Component> components;
		ComponentsBitset componentsBitset;
		ComponentGroup<Components...> others;

		template <typename T>
		static constexpr bool Has()
		{
			return contains<T,Component, Components...>();
		}

		template <typename T>
		constexpr ComponentsArray<T>& GetArray()
		{
			if constexpr (std::is_same<T, Component>())
			{
				return components;
			}
			else
			{
				return others.GetArray<T>();
			}
		}

		template <typename T>
		constexpr ComponentsBitset& GetBitset()
		{
			if constexpr (std::is_same<T, Component>())
			{
				return componentsBitset;
			}
			else
			{
				return others.GetBitset<T>();
			}
		}
	};

	template<typename Component>
	struct ComponentGroup <Component>
	{
		ComponentsArray<Component> components;
		ComponentsBitset componentsBitset;

		template <typename T>
		constexpr ComponentsArray<T>& GetArray()
		{
			return components;
		}

		template <typename T>
		constexpr ComponentsBitset& GetBitset()
		{
			static_assert(std::is_same<T, Component>());
			return componentsBitset;
		}
	};


	struct Transform
	{
	public:
		Transform() = default;

		EntityID parentID {MAX_ENTITIES};
		std::vector<EntityID> childrenIDs;
		Math::Vec3 position{};
		Math::Vec3 rotation{};
		Math::Vec3 scale{1,1,1};
		bool HasParent() const{ return parentID < MAX_ENTITIES;}
	};

	struct BoxCollider2D
	{
		BoxCollider2D() = default;
		AABB bounds{};
	};

	struct SpriteRenderer
	{
		SpriteRenderer() = default;
		glm::bvec2 flip{ 0 };
		glm::vec3 position{ 0 };	// Temporary variable to access the position
		glm::vec3 rotation{ 0 };	// Temporary variable for the rotation of the sprite
		glm::vec2 size{ 0 };		// The size of the sprite in pixels ( Bean: different from the scale of the gameobject )
		glm::vec4 color{ 1 };		// The blended color of this sprite
		unsigned int spriteID = 0; // The index of the sprite
		const Texture* refTexture; // Pointer to the texture
		bool isAddingSprite;
	};

	using AllComponents = ComponentGroup<Transform, BoxCollider2D, SpriteRenderer>;

	#define RegisterComponent(Type)template <> struct GetComponentType<Type>{static constexpr size_t e{ (size_t)ComponentType::Type }; };

	template <typename T>
	struct GetComponentType{};

	RegisterComponent(Transform);
	RegisterComponent(BoxCollider2D);
	RegisterComponent(SpriteRenderer);
}


#endif // !COMPONENTS_H