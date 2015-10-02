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
  
  vector<unique_ptr<gui_element_base>> children;
};
  
}