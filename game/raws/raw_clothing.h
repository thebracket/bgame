#pragma once

#include <string>
#include "base_raw.h"
#include "../components/debug_name_component.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_clothing : public base_raw {
    raw_clothing() { type = CLOTHING; };
    raw_clothing(const string &new_name) : item (new_name) { type=CLOTHING; }
    string item;    
};
  
}
