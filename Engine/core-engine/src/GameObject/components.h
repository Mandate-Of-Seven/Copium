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
	struct Pack{};

	template<size_t ID, typename... Components>
	struct ComponentGroup
	{
		using Types = Pack<Components...>;

		template <typename T>
		static constexpr bool Has()
		{
			return contains<T, Components...>();
		}

		template <typename T>
		static SparseSet<T, MAX_ENTITIES>& GetArray()
		{
			static_assert(contains<T, Components...>());
			static SparseSet<T, MAX_ENTITIES> components;
			return components;
		}

		template <typename T>
		static void ComponentEnabled(EntityID entityID, bool* resultOrVal, bool setting = true)
		{
			static_assert(contains<T, Components...>());
			static std::bitset<MAX_ENTITIES> componentsBitset;
			if (setting)
			{
				PRINT("Setting " << typeid(T).name() << " to " << *resultOrVal);
				componentsBitset.set(entityID, *resultOrVal);
			}
			{
				*resultOrVal = componentsBitset.test(entityID);
				PRINT("Getting " << typeid(T).name() << " = " << *resultOrVal);
			}
		}
	};


	struct Transform
	{
	public:
		Transform() = default;

		Transform* parent = nullptr;
		std::list<Transform*> children;
		Math::Vec3 position;
		Math::Vec3 rotation;
		Math::Vec3 scale;

		bool hasParent() const
		{
			return parent;
		}

		void setParent(Transform* _parent)
		{
			//Previously had a parent
			if (parent)
				parent->children.remove(this);
			parent = _parent;
			//_parent might be nullptr
			if (_parent)
				_parent->children.push_back(this);
		}

		/***************************************************************************/
		/*!
		\brief
			Constructor that takes in position, rotations,scale to 1, and initializes
			parent to nullptr
		*/
		/**************************************************************************/
		Transform(
			ComponentID _entityID,
			Math::Vec3 _position = Math::Vec3(),
			Math::Vec3 _rotation = Math::Vec3(),
			Math::Vec3 _scale = { 1,1,1 });
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
		Texture* refTexture; // Pointer to the texture
		bool isAddingSprite;
	};

	using MainComponents = ComponentGroup<0, Transform, BoxCollider2D, SpriteRenderer>;
	using BackupComponents = ComponentGroup<1, Transform>;

	#define RegisterComponent(Type)template <> struct GetComponentType<Type>{static constexpr size_t e{ (size_t)ComponentType::Type }; };

	template <typename T>
	struct GetComponentType{};

	RegisterComponent(Transform);
	RegisterComponent(BoxCollider2D);
	RegisterComponent(SpriteRenderer);
}


#endif // !COMPONENTS_H