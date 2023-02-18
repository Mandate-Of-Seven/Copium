#ifndef CONFIG_H
#define CONFIG_H

template <typename... Ts>
struct TemplatePack 
{};


template<typename T, typename... Ts>
static constexpr bool contains()
{
	return std::disjunction_v<std::is_same<T, Ts>...>;
}

#define MAX_COMPONENTS 100000
#define MAX_GAMEOBJECTS 100000
#define TEXT_BUFFER_SIZE 2048

static const char* FILESYSTEM_MUTEX = "FILESYSTEM_MUTEX";

#endif // !CONFIG_H