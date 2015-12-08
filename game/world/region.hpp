#pragma once

#include <vector>
#include <cstdint>
#include "world_defs.hpp"
#include "tile3d.hpp"

class region_t {
public:
  inline uint16_t tile_idx ( const uint8_t x, const uint8_t y ) { return ( y * REGION_WIDTH ) + x; }
private:
  std::vector< tile_t > tiles;
};