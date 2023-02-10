#pragma once

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <GameObject/Components/component.h>
#include <config.h>
#include <vector>

#define MAX_ANIMATION_COUNT 5


namespace Copium
{

	enum class ComponentType : int      // Types of Components
	{
		Animator,
		AudioSource,
		BoxCollider2D,
		Button,
		Camera,
		Image,
		Rigidbody2D,
		SpriteRenderer,
		Script,
		Transform,
		Text,
		SortingGroup,
		None
	};

	template <typename T>
	using ComponentsArray = std::vector<T>;
	template <typename T>
	using ComponentsPtrArray = std::vector<T*>;

	template<typename Component, typename... Components>
	struct ComponentGroup
	{
		using Types = TemplatePack<Component,Components...>;

		ComponentsArray<Component> components;
		ComponentGroup<Components...> others;

		constexpr ComponentGroup(TemplatePack<Component,Components...> pack){}

		template <typename T>
		static constexpr bool Has(){return contains<T,Component, Components...>();}

		template <typename T>
		constexpr ComponentsArray<T>& GetArray()
		{
			if constexpr (std::is_same<T, Component>())
				return components;
			else
				return others.GetArray<T>();
		}
	};

	template<typename Component>
	struct ComponentGroup <Component>
	{
		ComponentsArray<Component> components;
		template <typename T>
		constexpr ComponentsArray<T>& GetArray()
		{
			return components;
		}
	};

	template<typename Component, typename... Components>
	struct ComponentPtrGroup
	{
		using Types = TemplatePack<Component, Components...>;

		ComponentsPtrArray<Component> components;
		ComponentPtrGroup<Components...> others;

		constexpr ComponentPtrGroup(TemplatePack<Component, Components...> pack) {}

		template <typename T>
		static constexpr bool Has() { return contains<T, Component, Components...>(); }

		template <typename T>
		constexpr ComponentsPtrArray<T>& GetArray()
		{
			if constexpr (std::is_same<T, Component>())
				return components;
			else
				return others.GetArray<T>();
		}
	};

	template<typename Component>
	struct ComponentPtrGroup <Component>
	{
		ComponentsPtrArray<Component> components;
		template <typename T>
		constexpr ComponentsPtrArray<T>& GetArray()
		{
			return components;
		}
	};

#define RegisterComponent(Type) class Type;template <> struct GetComponentType<Type>{static constexpr size_t e{ (size_t)ComponentType::Type }; static constexpr const char* name = #Type;};


	template <typename T>
	struct GetComponentType{};

	RegisterComponent(Transform);
	RegisterComponent(BoxCollider2D);
	RegisterComponent(SpriteRenderer);
	RegisterComponent(Rigidbody2D);
	RegisterComponent(Camera);
	RegisterComponent(Animator);
	RegisterComponent(SortingGroup);
	RegisterComponent(AudioSource);
	RegisterComponent(Script);
	RegisterComponent(Image);
	RegisterComponent(Text);
	RegisterComponent(Button);


	using ComponentTypes = TemplatePack<Transform, BoxCollider2D, Rigidbody2D, SpriteRenderer, Camera, Animator, SortingGroup, AudioSource, Script, Image, Text, Button>;
	using ComponentsArrays = decltype(ComponentGroup<ComponentTypes>());
	using ComponentsPtrArrays = decltype(ComponentPtrGroup<ComponentTypes>());
}


#endif // !COMPONENTS_H