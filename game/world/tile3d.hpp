#pragma once

#include "tile_types.hpp"

class tile_t {
public:
    bool solid;
    unsigned char base_tile_type;
    unsigned char ground;
    unsigned char climate;
    unsigned char covering;
};
