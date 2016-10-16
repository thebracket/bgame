#pragma once

#include <rltk.hpp>
#include <tuple>
#include <type_traits>
#include <utility>
#include <boost/tti/has_member_function.hpp>

// for-each for tuples
template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
	using swallow = int[];
	(void)swallow {
		1,
			(f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
	};
}

template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f) {
	constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
	for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
		std::make_index_sequence<N>{});
}

/*
 * Specialized serialize to handle multiple types.
 */

namespace s_detail {
	BOOST_TTI_HAS_MEMBER_FUNCTION(save)
	BOOST_TTI_HAS_MEMBER_FUNCTION(load)
}

template<typename T>
struct _Serializer {

	template<typename T>
	typename std::enable_if< s_detail::has_member_function_save<T, std::ofstream>::value, void>::type
	Serialize(std::ostream &f, T &val) {
		val.save(f);
	}
	template<typename T>
	typename std::enable_if< !s_detail::has_member_function_save<T, std::ofstream>::value, void>::type
	Serialize(std::ostream &f, T &val) {
		rltk::serialize(f, val);
	}

	template<typename T>
	typename std::enable_if< s_detail::has_member_function_save<T, std::ofstream>::value, void>::type
	Deserialize(std::istream &f, T &val) {
		val.load(f);
	}
	template<typename T>
	typename std::enable_if< !s_detail::has_member_function_save<T, std::ofstream>::value, void>::type
	Deserialize(std::istream &f, T &val) {
		rltk::deserialize(f, val);
	}
};

template<typename T>
inline void _Serialize(std::ostream &f, T &val) {
	_Serializer<T> s;
	s.Serialize(f, val);
}

template<typename T>
inline void _Deserialize(std::istream &f, T &val) {
	_Serializer<T> s;
	s.Deserialize(f, val);
}

/*
 * Call Serialize on every element of a variadic pack of parameters. The idea here is to let
 * the caller do Serialize(file, one, two, three) for a very obvious serialization order.
 */
template<typename... T>
inline void _Serialize(std::ostream &f, std::tuple<T...> &vals) {
	for_each(vals, [&f] (auto &t) { _Serialize(f, t); });
}

template<typename ...T>
inline void _Deserialize(std::istream &f, std::tuple<T...> &vals) {
	for_each(vals, [&f] (auto &t) { _Deserialize(f, t); });
}

template<typename ...T>
inline void Serialize(std::ostream &f, T&... vals) {
	_Serialize(f, std::make_tuple<T&...>(vals...));
}

template<typename ...T>
inline void Deserialize(std::istream &f, T&... vals) {
	_Deserialize(f, std::make_tuple<T&...>(vals...));
}
