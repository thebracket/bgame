#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"

using std::vector;

namespace raws {

struct raw_power_drain : public base_raw {
     raw_power_drain() {
          type = POWER_DRAIN;
     };
     
     raw_power_drain(const int &qty) : quantity(qty) {
	  type = POWER_DRAIN;
     }
     
     int quantity;     
};

}
