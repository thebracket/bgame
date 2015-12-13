#pragma once

#include <vector>
#include <cstdint>
#include <fstream>
#include <Poco/InflatingStream.h>
#include <Poco/DeflatingStream.h>
#include "world_defs.hpp"
#include "tile3d.hpp"

class region_t {
public:
  region_t() { tiles.resize( REGION_TILES ); }
  inline uint32_t tile_idx ( const uint8_t x, const uint8_t y, const uint8_t z ) const { return (z * REGION_HEIGHT*REGION_WIDTH) + ( y * REGION_WIDTH ) + x; }
  tile_t * get_tile ( const uint8_t x, const uint8_t y, const uint8_t z ) {     
    return &(tiles[ tile_idx ( x, y, z ) ]);
  }
  void save( Poco::DeflatingOutputStream &lbfile );
  void load( Poco::InflatingInputStream &lbfile );

  std::vector< tile_t > tiles;
};