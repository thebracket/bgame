#pragma once

#include "component_types.h"
#include "../../virtual_terminal.h"
#include <vector>
#include <iostream>

using namespace engine::ecs::serialization_generic;

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
    
    void save(fstream &lbfile) {
      save_common_component_properties<renderable_component>(lbfile, *this);
      save_primitive<unsigned char>(lbfile, glyph);
      save_primitive<engine::vterm::color_t>(lbfile, foreground);
      save_primitive<engine::vterm::color_t>(lbfile, background);
    }
    
    void load(fstream &lbfile) {
      load_common_component_properties<renderable_component>(lbfile, *this);
      load_primitive<unsigned char>(lbfile, glyph);
      load_primitive<engine::vterm::color_t>(lbfile, foreground);
      load_primitive<engine::vterm::color_t>(lbfile, background);
    }
};

}
}
