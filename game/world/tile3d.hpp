#pragma once

#include "tile_types.hpp"
#include <Poco/DeflatingStream.h>
#include <Poco/InflatingStream.h>
#include "../../engine/virtual_terminal.h"
#include <bitset>

enum TILE_OPTIONS {
  WALK_BLOCKED = 0,
  VIEW_BLOCKED = 1,
  CAN_STAND_HERE = 2,
  CAN_GO_NORTH = 3,
  CAN_GO_SOUTH = 4,
  CAN_GO_EAST = 5,
  CAN_GO_WEST = 6,
  CAN_GO_UP = 7,
  CAN_GO_DOWN = 8,
  SOLID = 9
};

constexpr int TILE_OPTIONS_COUNT = 10;

class tile_t {
public:
    tile_t() { flags = 0L; }
  
    uint8_t water_level;
    unsigned char base_tile_type;
    unsigned char ground;
    unsigned char climate;
    unsigned char covering;
    std::bitset<TILE_OPTIONS_COUNT> flags;
    
    // Non-persistent helpers
    engine::vterm::screen_character render_as;
    
    void save ( Poco::DeflatingOutputStream &lbfile );
    void load ( Poco::InflatingInputStream &lbfile );
};

void tile_render_calculation( tile_t * tile );
