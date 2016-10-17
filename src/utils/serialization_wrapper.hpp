#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/optional.hpp>

/**** Forward declarations ****/
template <typename... T> void Serialize(std::ostream &f, T&... args);
template <typename... T> void Deserialize(std::istream &f, T&... args);

/**** Specific implementations of Serialize ****/

/*
 * Vector case for a primitive; save the size and then call Serialize for each element in the container
 * in turn.
 */
template <typename T>
inline void __Serialize(std::ostream &f, std::vector<T> &arg) {
	std::cout << "Serialize: vector case\n";
	std::size_t sz;
	rltk::serialize(f, sz);
	for (T& e : arg) {
		Serialize<T>(f, e);
	}
	std::cout << "Serialize: end vector case\n";
}

/*
 * Vector case for a pair
 */
template <typename K, typename V>
inline void __Serialize(std::ostream &f, std::vector<std::pair<K,V>> &arg) {
	std::cout << "Serialize: vector pair case\n";
	std::size_t sz;
	rltk::serialize(f, sz);
	for (std::pair<K,V>& e : arg) {
		Serialize<K>(f, e.first);
		Serialize<V>(f, e.second);
	}
	std::cout << "Serialize: end vector case\n";
}

/*
 * boost::flat_map case
 */
template <typename K, typename V>
inline void __Serialize(std::ostream &f, boost::container::flat_map<K,V> &arg) {
	std::cout << "Serialize: boost::flat_map\n";
	std::size_t sz;
	rltk::serialize(f, sz);
	for (std::pair<K,V>& e : arg) {
		Serialize<K>(f, e.first);
		Serialize<V>(f, e.second);
	}
	std::cout << "Serialize: end boost::flat_map case\n";
}

/* Support for boost::optional */
template <typename T>
inline void __Serialize(std::ostream &f, boost::optional<T> &arg) {
	std::cout << "Serialize: boost::optional\n";
	bool has_data = false;
	if (arg) has_data = true;
	Serialize(f, has_data);
	if (arg) Serialize<T>(f, arg.get());
}

/* Support for boost::container::flat_set */
template <typename T>
inline void __Serialize(std::ostream &f, boost::container::flat_set<T> &arg) {
	std::cout << "Serialize: boost::flat_set case\n";
	std::size_t sz;
	rltk::serialize(f, sz);
	for (T& e : arg) {
		Serialize<T>(f, e);
	}
	std::cout << "Serialize: end flat_set case\n";
}

/*
 * Base case for a primitive; in this case, send it through to the RLTK serialization code.
 */
template <typename T>
inline void __Serialize(std::ostream &f, T& arg) {
	std::cout << "Serialize: base case\n";
	rltk::serialize(f, arg);
}

/**** Specific implementations of Deserialize ****/

template <typename T>
inline void __Deserialize(std::istream &f, T& arg) {
	std::cout << "Deserialize: base case\n";
	rltk::deserialize(f, arg);
}

template <typename T>
inline void __Deserialize(std::istream &f, std::vector<T> &arg) {
	std::cout << "Deserialize: vector case\n";
	std::size_t sz;
	rltk::deserialize(f, sz);
	arg.clear();
	for (std::size_t i=0; i<sz; ++i) {
		T tmp;
		Deserialize<T>(f, tmp);
		arg.push_back(tmp);
	}
}

template <typename K, typename V>
inline void __Deserialize(std::istream &f, std::vector<std::pair<K,V>> &arg) {
	std::cout << "Deserialize: vector pair case\n";
	std::size_t sz;
	rltk::deserialize(f, sz);
	arg.clear();
	for (std::size_t i=0; i<sz; ++i) {
		std::pair<K,V> tmp;
		Deserialize<K>(f, tmp.first);
		Deserialize<V>(f, tmp.second);
		arg.push_back(tmp);
	}
}

template <typename K, typename V>
inline void __Deserialize(std::istream &f, boost::container::flat_map<K,V> &arg) {
	std::cout << "Deserialize: flat_map case\n";
	std::size_t sz;
	rltk::deserialize(f, sz);
	arg.clear();
	for (std::size_t i=0; i<sz; ++i) {
		std::pair<K,V> tmp;
		Deserialize<K>(f, tmp.first);
		Deserialize<V>(f, tmp.second);
		arg.insert(tmp);
	}
}

template <typename T>
inline void __Deserialize(std::istream &f, boost::optional<T> &arg) {
	std::cout << "Serialize: boost::optional\n";
	bool has_data = false;
	Deserialize(f, has_data);
	if (arg) {
		T real_data;
		Deserialize<T>(f, real_data);
		arg = real_data;
	}
}

template <typename T>
inline void __Deserialize(std::istream &f, boost::container::flat_set<T> &arg) {
	std::cout << "Deserialize: boost::container::flat_set case\n";
	std::size_t sz;
	rltk::deserialize(f, sz);
	arg.clear();
	for (std::size_t i=0; i<sz; ++i) {
		T tmp;
		Deserialize<T>(f, tmp);
		arg.insert(tmp);
	}
}

/* Helper functions. These use SFINAE to detect whether a struct/class has a load or save method. */

template< class T >
struct has_save_method
{
    typedef char(&YesType)[1];
    typedef char(&NoType)[2];
    template< class, class > struct Sfinae;

    template< class T2 > static YesType Test( Sfinae<T2, decltype(std::declval<T2>().save( std::declval<std::ostream&>() ))> * );
    template< class T2 > static NoType  Test( ... );
    static const bool value = sizeof(Test<T>(0))==sizeof(YesType);
};

template< class T >
struct has_load_method
{
    typedef char(&YesType)[1];
    typedef char(&NoType)[2];
    template< class, class > struct Sfinae;

    template< class T2 > static YesType Test( Sfinae<T2, decltype(std::declval<T2>().load( std::declval<std::istream&>() ))> * );
    template< class T2 > static NoType  Test( ... );
    static const bool value = sizeof(Test<T>(0))==sizeof(YesType);
};

/* Uses enable_if to determine if it should call a struct's save method, or just pass through to
 * a more specific Serialize */
template<class T>
struct _Serialize_check_for_save
{
	template<class Q = T>
	typename std::enable_if< has_save_method<Q>::value, void >::type
    test(std::ostream &f, Q &arg)
    {
        std::cout << "Serialize: struct has a save method\n";
		arg.save(f);
		std::cout << "END save method\n";
    }

    template<class Q = T>
	typename std::enable_if< !has_save_method<Q>::value, void >::type
    test(std::ostream &f, Q &arg)
    {
		std::cout << "Serialize: no save method\n";
        __Serialize(f, arg);
    }
};

/* Uses enable_if to determine if it should call a struct's load method, or just pass through to
 * a more specific Deserialize */
template<class T>
struct _Deserialize_check_for_load
{
    template<class Q = T>
	typename std::enable_if< has_load_method<Q>::value, void >::type
    test(std::istream &f, Q &arg)
    {
        std::cout << "Deserialize: struct has a load method\n";
		arg.load(f);
		std::cout << "END load method\n";
    }

    template<class Q = T>
	typename std::enable_if< !has_load_method<Q>::value, void >::type
    test(std::istream &f, Q &arg)
    {
        __Deserialize(f, arg);
    }
};

/**** Parameter pack expansion templates; recursively call Serialize(f, element) on each element in the parameter pack with no copy. ***/

template<typename T>
void _Serialize(std::ostream &f, T &arg) {
	_Serialize_check_for_save<T> temp;
	temp.test(f, arg);
}

template <typename First, typename... Rest>
void _Serialize(std::ostream &f, First& arg, Rest&... args) {
	_Serialize(f, arg);
	_Serialize(f, args...);
}

template <typename T>
void _Deserialize(std::istream &f, T &arg) {
	_Deserialize_check_for_load<T> temp;
	temp.test(f, arg);
}

template <typename First, typename... Rest>
void _Deserialize(std::istream &f, First& arg, Rest&... args) {
	_Deserialize(f, arg);
	_Deserialize(f, args...);
}

/*
 * Base Serialize template: use as Serialize(file, as, many, args, as, you, want)
 * The args are passed in as a parameter pack of references, so it runs without a copy.
 */
template <typename... T>
inline void Serialize(std::ostream &f, T&... args) {
	_Serialize(f, args...);
}

/*
 * Base Deserialize template: use as Serialize(file, as, many, args, as, you, want)
 * The args are passed in as a parameter pack of references, so it loads in-place.
 */
template <typename... T>
inline void Deserialize(std::istream &f, T&... args) {
	_Deserialize(f, args...);
}