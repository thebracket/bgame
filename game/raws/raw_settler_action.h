#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/renderable_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"
#include "../../engine/globals.h"

using std::vector;

namespace raws {

struct raw_settler_action : public base_raw {
     raw_settler_action() {
          type = SETTLER_ACTION;
     };
     
     std::string action_name;
     std::string provides;
     int provides_qty;
     int power_drain;

     virtual void build_components ( entity &parent, const int &x, const int &y ) const {
     }
};

}
