#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include "world_defs.hpp"

using heightmap_t = std::vector< uint16_t >;

std::unique_ptr<heightmap_t> get_height_map() { 
    std::unique_ptr<heightmap_t> result = std::make_unique<heightmap_t>(); 
    result->resize( NUMBER_OF_TILES_IN_THE_WORLD );
    return std::move(result);
}
