#pragma once

#include "../virtual_terminal.h"
#include "gui_element_base.h"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

/* Master header to include the rest */

namespace engine {

class gui {
public:
  gui();
  std::pair<int,int> get_screen_size() { return std::make_pair(screen_width, screen_height); };
  void add_child(unique_ptr<gui_element_base> child) { children.push_back(std::move(child)); };
  void render();
private:
  int screen_width;
  int screen_height;
  vector<unique_ptr<gui_element_base>> children;
};
  
}
