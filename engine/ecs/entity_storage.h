#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include "entity.h"

using engine::ecs::entity;
using std::unordered_map;
using std::function;

namespace engine {  
namespace ecs_detail {
  
class entity_storage {
private:
  unordered_map<int, entity> entities;
  int next_handle = 0;
public:
  std::size_t size() { return entities.size(); }
  
  void for_each(std::function<void(entity *)> func) {
    for (auto it=entities.begin(); it != entities.end(); ++it) {
      func(&it->second);
    }
  }
  
  int get_next_handle() {
    int result = next_handle;
    ++next_handle;
    return result;
  }
  
  void add_entity(const ecs::entity &target) {
    entities[target.handle] = target;
  }
  
  ecs::entity * find_by_handle(const int &handle) {
    auto finder = entities.find(handle);
    if (finder != entities.end()) {
      return &finder->second;
    }
    return nullptr;
  }
  
  vector<int> find_by_func ( function<bool ( const entity &e ) > matcher ) {
    vector<int> result;
     for ( auto it = entities.begin(); it != entities.end(); ++it ) {
          const bool match = matcher ( it->second );
          if ( match ) result.push_back ( it->second.handle );
     }
     return result;
  }
  
  vector<int> find_entities_by_bitset ( const int &bit_to_check )
  {
      return find_by_func ( [bit_to_check] ( const entity &e ) {
	    return e.component_types.test ( bit_to_check );
      } );
  }
};
  
}
}