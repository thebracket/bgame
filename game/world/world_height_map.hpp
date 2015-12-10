#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <numeric>
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

inline void smooth_height_map ( heightmap_t * height_map ) {
    std::unique_ptr < heightmap_t > tmp_p = get_height_map();
    heightmap_t tmp = *tmp_p.get();
    heightmap_t altitude_map = *height_map;
    
    for (int y = 1; y < (WORLD_HEIGHT * REGION_HEIGHT)-1; ++y) {
      for (int x = 1; x < (WORLD_WIDTH * REGION_WIDTH)-1; ++x) {
	const int sum = altitude_map[height_map_idx ( x-1, y-1 )] +
                            altitude_map[height_map_idx ( x, y-1 )] +
                            altitude_map[height_map_idx ( x+1, y-1 )] +
                            altitude_map[height_map_idx ( x-1, y )] +
                            altitude_map[height_map_idx ( x, y )] +
                            altitude_map[height_map_idx ( x+1, y )] +
                            altitude_map[height_map_idx ( x-1, y+1 )] +
                            altitude_map[height_map_idx ( x, y+1 )] +
                            altitude_map[height_map_idx ( x+1, y+1 )];
	const uint16_t average = sum / 9;
	tmp [ height_map_idx( x, y ) ] = average;
      }
    }
    
    altitude_map.clear();
    std::copy ( tmp.begin(), tmp.end(), std::back_inserter ( altitude_map ) );
}

inline int average_heightmap_height ( heightmap_t * height_map ) {
    int sum = std::accumulate ( height_map->begin(), height_map->end(), 0 );
    return sum / height_map->size();
}

inline int min_heightmap_height ( heightmap_t * height_map ) {
    return *std::min_element ( height_map->begin(), height_map->end() );
}

inline int max_heightmap_height ( heightmap_t * height_map ) {
    return *std::max_element ( height_map->begin(), height_map->end() );
}
