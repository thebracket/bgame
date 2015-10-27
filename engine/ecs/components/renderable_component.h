#pragma once

#include "component_types.h"
#include "../../virtual_terminal.h"
#include <vector>
#include <iostream>

using std::vector;
using engine::vterm::color_t;

namespace engine {
namespace ecs {

struct renderable_component {
    int entity_id;
    int handle;
    component_type type = renderable;    
    
    renderable_component() {}
    renderable_component(const unsigned char &g, const color_t &f, const color_t &b) : glyph(g), foreground(f), background(b) {}
    
    unsigned char glyph;
    engine::vterm::color_t foreground;
    engine::vterm::color_t background;    
};

}
}
