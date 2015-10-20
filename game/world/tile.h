#pragma once

#include "../../engine/virtual_terminal.h"
#include "tile_material.h"
#include <fstream>

using engine::vterm::color_t;
using std::fstream;

enum tile_type { water,flat,ramp,beach };
const unsigned char TILE_REVEALED = 1;
const unsigned char TILE_VISIBLE = 2;

class tile {
public:
    tile();

    // About the tile itself
    tile_type base_tile_type;
    tile_material base_tile_material;
    short altitude;

    void calculate_display_properties();

    // Cached: not persistent
    unsigned char display;
    color_t foreground {0,255,0};
    color_t background {0,0,0};

    void save(fstream &lbfile);
    void load(fstream &lbfile);
};

