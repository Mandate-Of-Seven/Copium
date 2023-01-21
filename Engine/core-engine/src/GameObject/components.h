#pragma once

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Physics/collision.h>
#include <imgui.h>
#include <Utilities/sparse-set.h>
#include <config.h>
#include <Graphics/textures.h>
#include <Animation/animation-struct.h>


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

	using ComponentsBitset = std::bitset<MAX_ENTITIES>;

	template <typename Component>
	class ComponentsArray
	{
		ComponentsBitset componentsBitset{};
		SparseSet<Component, MAX_ENTITIES> components;
		friend class EntityComponentSystem;
	public:
		void SetEnabled(EntityID entityID, bool val)
		{
			componentsBitset.set(entityID, val);
		}

		bool GetEnabled(EntityID entityID) const
		{
			return componentsBitset.test(entityID);
		}

		EntityID GetID(const Component& component)
		{
			return(&component - &components.DenseGet(0));
		}

		size_t GetSize() const
		{
			return components.GetSize();
		}

		Component& FindByID(EntityID entityID)
		{
			return components.DenseGet(entityID);
		}

		Component& operator[](size_t index)
		{
			return components[index];
		}
	};

	template<typename Component, typename... Components>
	struct ComponentGroup
	{
		using Types = Pack<Component,Components...>;

		ComponentsArray<Component> components;
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

	struct Rigidbody2D
	{
		Rigidbody2D() = default;
		Math::Vec2 velocity;					//velocity of object
		Math::Vec2 acceleration;				//acceleration of object
		Math::Vec2 force;						//forces acting on object
		float mass;								//mass of object
		bool active;							//is object active?
		bool useGravity;					//is object affected by gravity?
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

<<<<<<< Updated upstream
	using AllComponents = ComponentGroup<Transform, BoxCollider2D, Rigidbody2D, SpriteRenderer>;
=======
	struct Animator
	{
		enum class AnimatorStatus : char
		{
			idle = 0,
			playing
		};

		Animator() : loop{true}, status{AnimatorStatus::idle} {}

		std::vector<Animation> animations;		// The indices of the animations inside the assets-system
		int currentAnimationIndex;				// Current playing animation
		int startingAnimationIndex;				// The first animation that is playing
		unsigned int animationCount;
		bool loop;
		AnimatorStatus status;

	};

	using AllComponents = ComponentGroup<Transform, BoxCollider2D, SpriteRenderer, Animator>;
>>>>>>> Stashed changes

	#define RegisterComponent(Type)template <> struct GetComponentType<Type>{static constexpr size_t e{ (size_t)ComponentType::Type }; };

	template <typename T>
	struct GetComponentType{};

	RegisterComponent(Transform);
	RegisterComponent(BoxCollider2D);
	RegisterComponent(SpriteRenderer);
<<<<<<< Updated upstream
	RegisterComponent(Rigidbody2D);
=======
	RegisterComponent(Animator);
>>>>>>> Stashed changes
}


#endif // !COMPONENTS_H