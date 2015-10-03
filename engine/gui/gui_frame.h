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

enum GUI_FRAME_MODE { FILL_REGION, TRI_MAIN, TRI_CONSOLE, TRI_PANEL };
  
class gui_frame : public gui_element_base {
public:
  gui_frame(const string name, const GUI_FRAME_MODE new_mode) : mode(new_mode)
  {
    tag = name;
  }
  
  virtual void render(const screen_region viewport) {
    if (mode == FILL_REGION) {
      vterm::draw_box(viewport.x, viewport.y, viewport.w, viewport.h, grey, black);
      const screen_region inner_viewport{viewport.x+1, viewport.y+1, viewport.w-2, viewport.h-2};
      for (const auto &child : children) {
	child->render(inner_viewport);    
      }
      return;
    }
    
    if (mode == TRI_MAIN) {
      const int main_width = viewport.w - 20;
      const int main_height = viewport.h - 10;
      vterm::draw_box(viewport.x, viewport.y, main_width, main_height, grey, black);
      const screen_region inner_viewport{viewport.x+1, viewport.y+1, main_width-2, main_height-2};
      for (const auto &child : children) {
	child->render(inner_viewport);    
      }
      return;
    }

    if (mode == TRI_CONSOLE) {
      const int main_width = viewport.w - 20;
      const int main_top = viewport.y+viewport.h-10;
      vterm::draw_box(viewport.x, main_top, main_width, 10, grey, black);
      const screen_region inner_viewport{viewport.x+1, main_top+1, main_width-2, 8};
      for (const auto &child : children) {
	child->render(inner_viewport);    
      }
      return;
    }

    if (mode == TRI_PANEL) {
      const int main_left = viewport.x+viewport.w-20;
      
      vterm::draw_box(main_left, viewport.y, 20, viewport.h, grey, black);
      const screen_region inner_viewport{main_left+1, viewport.y+1, 18, viewport.h-2};
      for (const auto &child : children) {
	child->render(inner_viewport);    
      }
      return;
    }
    
  };
private:
  const GUI_FRAME_MODE mode;
};
  
}