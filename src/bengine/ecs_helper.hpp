#pragma once

namespace bengine
{
	/*
	 * Returns true if T appears in the Ts parameter pack.
	 */
	template<typename T, typename... Ts>
	constexpr bool contains()
	{
		return std::disjunction_v<std::is_same<T, Ts>...>;
	}

	/*
	 * Builds an index sequencer of sequential integers for a given parameter pack.
	 */
	template<typename ...Ts, size_t... I>
	void setup_index(std::tuple<std::pair<size_t, std::map<int, Ts>>...> &tuple, std::index_sequence<I...>) noexcept
	{
		(void)(std::initializer_list<int> {
			(std::get<I>(tuple).first = I, 0)...
		});
	}

	/*
	 * Given a storage pack, it erases a component belonging to a given entity ID.
	 */
	template<typename ...Ts, size_t... I>
	void delete_components_for_entity(std::tuple<std::pair<size_t, std::map<int, Ts>>...> &tuple, std::index_sequence<I...>, const int &entity_id) noexcept
	{
		(void)(std::initializer_list<int> {
			(std::get<I>(tuple).second.erase(entity_id), 0)...
		});
	}

}