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
  
  virtual void render(const screen_region viewport);
    
  
private:
  const GUI_FRAME_MODE mode;
};
  
}