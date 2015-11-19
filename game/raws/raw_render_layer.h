#pragma once

#include <string>
#include "base_raw.h"

using std::string;

namespace raws {

struct raw_render_layer : public base_raw {
    raw_render_layer() { type = LAYER; };
    raw_render_layer(const int layer) : layer ( layer ) { type = LAYER; }
    int layer;
};
  
}
