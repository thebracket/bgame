#pragma once

#include <string>
#include "base_raw.h"
#include "../components/debug_name_component.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_clothing_color : public base_raw {
    raw_clothing_color() { type = CLOTHING_COLOR; };
    vector<string> colors;
  
};
  
}
