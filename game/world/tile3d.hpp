#pragma once

#include "tile_types.hpp"
#include <Poco/DeflatingStream.h>
#include <Poco/InflatingStream.h>
#include "../../engine/virtual_terminal.h"

class tile_t {
public:
    bool solid;
    uint8_t water_level;
    unsigned char base_tile_type;
    unsigned char ground;
    unsigned char climate;
    unsigned char covering;
    
    // Non-persistent helpers
    engine::vterm::screen_character render_as;
    
    void save ( Poco::DeflatingOutputStream &lbfile );
    void load ( Poco::InflatingInputStream &lbfile );
};

void tile_render_calculation( tile_t * tile );
