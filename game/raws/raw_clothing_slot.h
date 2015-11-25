#pragma once

#include <string>
#include "base_raw.h"
#include "../components/debug_name_component.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_clothing_slot : public base_raw {
    raw_clothing_slot() { type = CLOTHING_SLOT; };
    raw_clothing_slot(const string &new_name) : slot (new_name) { type=NAME; }
    string slot;    
};
  
}
