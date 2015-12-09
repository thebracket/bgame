#pragma once

#include "world_defs.hpp"
#include "region.hpp"
#include "location.hpp"
#include <vector>
#include <memory>

class planet_t {
public:
  planet_t() { regions.resize( WORLD_TILES ); }
  region_t * get_region ( const uint8_t region_index );
  region_t * create_region ( const uint8_t region_index );
  inline uint8_t planet_idx ( const uint8_t &x, const uint8_t &y) { return (y * WORLD_WIDTH) + x; }
  tile_t * get_tile( const location_t location );
  void free_region ( const uint8_t region_index );
private:
  std::vector< std::unique_ptr< region_t >  > regions;
  void load_region ( const uint8_t region_index );
  void save_region ( const uint8_t region_index );
};
