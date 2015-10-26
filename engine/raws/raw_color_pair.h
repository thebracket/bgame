#pragma once

#include <string>
#include "base_raw.h"
#include "../virtual_terminal.h"

using std::string;
using engine::vterm::color_t;

namespace engine {
namespace raws {

struct raw_color_pair : public base_raw {
    raw_color_pair() { type = COLOR_PAIR; };
    raw_color_pair(const color_t &fg, const color_t &bg) : foreground(fg),background(bg) { type=COLOR_PAIR; }
    color_t foreground;
    color_t background;
};
  
}
}