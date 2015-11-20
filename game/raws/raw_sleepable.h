#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"

using std::vector;

namespace raws {

struct raw_sleepable : public base_raw {
     raw_sleepable() {
          type = SLEEPABLE;
     };
};

}
