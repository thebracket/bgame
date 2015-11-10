#pragma once

#include "component_types.h"
#include "../../engine/virtual_terminal.h"
#include <vector>
#include <iostream>

using namespace serialization_generic;

using std::vector;
using engine::vterm::color_t;

struct renderable_component {
    int entity_id;
    int handle;
    component_type type = renderable;    
    
    renderable_component() {}
    renderable_component(const unsigned char &g, const color_t &f, const color_t &b, const int &tile) : glyph(g), foreground(f), background(b), tile_idx(tile) {}
    
    unsigned char glyph;
    engine::vterm::color_t foreground;
    engine::vterm::color_t background; 
    int tile_idx;
    
    void save(fstream &lbfile) {
      save_common_component_properties<renderable_component>(lbfile, *this);
      save_primitive<unsigned char>(lbfile, glyph);
      save_primitive<engine::vterm::color_t>(lbfile, foreground);
      save_primitive<engine::vterm::color_t>(lbfile, background);
      save_primitive<int>( lbfile, tile_idx );
    }
    
    void load(fstream &lbfile) {
      load_common_component_properties<renderable_component>(lbfile, *this);
      load_primitive<unsigned char>(lbfile, glyph);
      load_primitive<engine::vterm::color_t>(lbfile, foreground);
      load_primitive<engine::vterm::color_t>(lbfile, background);
      load_primitive<int>( lbfile, tile_idx );
    }
};
