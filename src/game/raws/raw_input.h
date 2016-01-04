#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"

using std::vector;

namespace raws {

struct raw_input : public base_raw {
     raw_input() {
          type = INPUT;
     };
     
     raw_input(const string &item, const int &qty) : required_item(item), quantity(qty) {
	  type = INPUT;
     }
     
     string required_item;
     int quantity;     
};

}
