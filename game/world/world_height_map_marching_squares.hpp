#pragma once

#include "world_height_map.hpp"
#include <functional>
#include <memory>
#include <vector>

typedef std::vector< uint8_t > marching_squares_map_t;

std::unique_ptr< marching_squares_map_t > marching_squares ( const heightmap_t * height_map, std::function<bool(int)> comparator );
