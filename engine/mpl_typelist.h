#pragma once

namespace engine {
namespace ecs_detail {
 
template <typename... Ts>
struct type_list {
    static constexpr std::size_t size{sizeof...(Ts)};
};

// Renamer:
  
template <template <typename...> class TNewName, typename T>
struct rename_helper;

// "Renames" `TOldName<Ts...>` to `TNewName<Ts...>`.
template <template <typename...> class TNewName,
    template <typename...> class TOldName, typename... Ts>
struct rename_helper<TNewName, TOldName<Ts...>>
{
    using type = TNewName<Ts...>;
};

template <template <typename...> class TNewName, typename T>
using rename = typename rename_helper<TNewName, T>::type;

}
}