#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include "world_defs.hpp"

using heightmap_t = std::vector< uint16_t >;

inline std::unique_ptr<heightmap_t> get_height_map() { 
    std::unique_ptr<heightmap_t> result = std::make_unique<heightmap_t>(); 
    result->resize( NUMBER_OF_TILES_IN_THE_WORLD );
    return std::move(result);
}

inline int height_map_idx ( const int x, const int y ) { return ( y * WORLD_WIDTH * REGION_WIDTH) + x; }

inline void fill_height_map ( heightmap_t * height_map, const uint16_t value ) {
    std::fill ( height_map->begin(), height_map->end(), value );
}

inline void zero_height_map( heightmap_t * height_map ) {
    fill_height_map ( height_map, 0 );
}
