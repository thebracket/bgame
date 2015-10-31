#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include "entity.h"

using std::unordered_map;
using std::function;
using engine::entity;

namespace engine {  
namespace ecs_detail {
  
class entity_storage {
private:
  unordered_map<int, entity> entities;
  int next_handle = 1;
public:
  std::size_t size() { return entities.size(); }
  
  void for_each(std::function<void(entity *)> func) {
    for (auto it=entities.begin(); it != entities.end(); ++it) {
      func(&it->second);
    }
  }
  
  void clear_all() {
      entities.clear();
  }
  
  int get_next_handle() {
    int result = next_handle;
    ++next_handle;
    return result;
  }
  
  void add_entity(const entity &target) {
    entities[target.handle] = target;
  }
  
  entity * find_by_handle(const int &handle) {
    auto finder = entities.find(handle);
    if (finder != entities.end()) {
      return &finder->second;
    }
    return nullptr;
  }  
};
  
}
}