#pragma once

#include <GameObject/Components/transform-component.h>

namespace Copium
{
	#define RegisterComponent(Type)template <> ComponentType GetComponentType<Type>(){return ComponentType::Type;}

	template <typename T> 
	ComponentType GetComponentType()
	{return ComponentType::None;}

	//RegisterComponent(Transform)

	//struct Transforms
	//{
	//	float translation[3] = { 0.0f, 0.0f, 0.0f };
	//	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	//	float scale[3] = { 1.0f, 1.0f, 1.0f };

	//	Transforms() = default;
	//	Transforms(const Transforms&) = default;
	//};
}