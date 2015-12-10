#pragma once

#include "tile_types.hpp"
#include <fstream>

class tile_t {
public:
    bool solid;
    uint8_t water_level;
    unsigned char base_tile_type;
    unsigned char ground;
    unsigned char climate;
    unsigned char covering;
    
    void save ( std::fstream &lbfile );
    void load ( std::fstream &lbfile );
};
