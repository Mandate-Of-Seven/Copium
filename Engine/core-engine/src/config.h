#pragma once

#ifndef CONFIG_H
#define CONFIG_H

using GameObjectID = uint64_t;
using ComponentID = uint64_t;

template <typename... Ts>
struct TemplatePack 
{
	template<typename T>
	static constexpr bool contains()
	{return std::disjunction_v<std::is_same<T, Ts>...>;}
};

static const char* FILESYSTEM_MUTEX = "FILESYSTEM_MUTEX";

#endif // !CONFIG_H