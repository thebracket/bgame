#pragma once

#include <memory>
#include <tuple>
#include <vector>
#include <functional>
#include <utility>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "serialization.h"

using std::tuple;
using std::vector;
using std::function;
using std::fstream;

namespace engine {
namespace ecs_detail {

template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl ( Tuple&& tuple, F&& f, std::index_sequence<Indices...> )
{
     using swallow = int[];
     ( void ) swallow {1,
                       ( f ( std::get<Indices> ( std::forward<Tuple> ( tuple ) ) ), void(), int{} )...
                      };
}

template <typename Tuple, typename F>
void for_each ( Tuple&& tuple, F&& f )
{
     constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
     for_each_impl ( std::forward<Tuple> ( tuple ), std::forward<F> ( f ),
                     std::make_index_sequence<N> {} );
}

template<typename... Ts>
class component_storage {
private:
     int next_handle = 1;

     static constexpr size_t number_of_component_types() {
          return sizeof... ( Ts );
     }
     tuple<vector<Ts>...> component_container;

     template <typename T>
     vector<T> * find_appropriate_bag() {
          return &std::get< vector<T> > ( component_container );
     }

public:
     std::size_t size() {
          std::size_t size = 0;
          for_each ( component_container, [&size] ( auto x ) {
               size += x.size();
          } );
          return size;
     }

     int get_next_handle() {
          const int result = next_handle;
          ++next_handle;
          return result;
     }

     template <typename T>
     int store_component ( T &component ) {
	  component.handle = get_next_handle();
          find_appropriate_bag<T>()->push_back ( component );
	  //std::cout << "Stored a component of type " << component.type << ", with handle " << component.handle << ".\n";
	  return component.handle;
     }

     template <typename T>
     T * find_component_by_handle ( const int &handle_to_find, T ignore ) {
          vector<T> * storage_bag = find_appropriate_bag<T>();
          for ( T &component : *storage_bag ) {
               if ( component.handle == handle_to_find ) {
                    return &component;
               }
          }
          return nullptr;
     }

     template<typename T>
     vector<T *> find_components_by_func ( function<bool ( const T * ) > matcher ) {
          vector<T *> result;
          vector<T> * storage_bag = find_appropriate_bag<T>();
          for ( const T &component : *storage_bag ) {
               if ( matcher ( &component ) ) result.push_back ( &component );
          }
          return result;
     }

     template<typename T>
     vector<T> * find_components_by_type(T ignore) {
          vector<T> * storage_bag = find_appropriate_bag<T>();
          return storage_bag;
     }

     void save ( fstream &lbfile ) {
          for_each ( component_container, [&lbfile] ( auto x ) {
               for ( const auto &c : x ) {
                    engine::ecs::serialization::save_component ( lbfile, c );
               }
          } );
     }
};

}
}
