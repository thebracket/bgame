#pragma once

#include <string>
#include "base_raw.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_workshop : public base_raw {
    raw_workshop() { type = WORKSHOP; };
    raw_workshop(const string &new_workshop) : workshop ( new_workshop ) { type=WORKSHOP; }
    string workshop;    
};
  
}
