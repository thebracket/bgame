#pragma once

#include "../../engine/virtual_terminal.h"

using engine::vterm::color_t;

class tile {
public:
  tile();

  bool revealed = false;
  bool visible = false;
  unsigned char display = '.';
  color_t foreground{0,255,0};
  color_t background{0,0,0};
};