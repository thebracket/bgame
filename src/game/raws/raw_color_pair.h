#pragma once

#include <string>
#include "base_raw.h"
#include "../../engine/virtual_terminal.hpp"

using std::string;
using engine::vterm::color_t;

namespace raws {

struct raw_color_pair : public base_raw {
	virtual ~raw_color_pair() {}
    raw_color_pair() { type = COLOR_PAIR; };
    raw_color_pair(const color_t &fg, const color_t &bg) : foreground(fg),background(bg) { type=COLOR_PAIR; }
    color_t foreground;
    color_t background;
};
  
}
