#pragma once

#include <vector>
#include <functional>
#include "components/component_types.h"

using std::vector;
using std::function;

namespace engine {
namespace ecs {

template<typename T>
class component_bag {
public:
  void add(T &component) {
    components.push_back(component);
  }
  
  vector<T *> find_by_func(function<bool(const T*)> matcher) const {
      vector<T *> result;
      for (const T &component : components) {
	  if (matcher(component)) result.push_back(&component);
      }
      return result;
  }
  
  T * find_by_handle(const int &handle) {
      for (T &component : components) {
	  if (component.handle == handle) return &component;
      }
      return nullptr;
  }
  
  vector<T *> find_by_type(const component_type &t) {
      vector<T *> result;
      for (T &component : components) {
	  if (component.type == t) result.push_back(&component);
      }
      return result;
  }
  
  void clear() {
      components.clear();
  }
  
  void map(function<void(const T *)> func) {
      for (const T &component : components) {
	  func(&component);
      }
  }
  
  int size() {
    return components.size();
  }
  
private:
  vector<T> components;
};
  
}
}