#pragma once

#include <utility>
#include <vector>
#include <memory>
#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include <iostream>

using std::vector;
using std::unique_ptr;

namespace engine {

enum GUI_FRAME_MODE { FILL_REGION };
  
class gui_frame : public gui_element_base {
public:
  gui_frame(const GUI_FRAME_MODE new_mode) : mode(new_mode)
  {}
  
  virtual void render(const screen_region viewport) {
    if (mode == FILL_REGION) {
      vterm::draw_box(viewport.x, viewport.y, viewport.w, viewport.h, grey, black);
      const screen_region inner_viewport{viewport.x+1, viewport.y+1, viewport.w-2, viewport.h-2};
      for (const auto &child : children)
	child->render(inner_viewport);
      
    }
  };
private:
  const GUI_FRAME_MODE mode;
};
  
}