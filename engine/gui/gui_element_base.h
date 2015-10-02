#pragma once

#include <utility>
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

namespace engine {
  
class gui_element_base {
public:
  virtual void render()=0;
  
  virtual void add_child(unique_ptr<gui_element_base> child) {
    children.push_back(std::move(child));
  }
  
  vector<unique_ptr<gui_element_base>> children;
};
  
}