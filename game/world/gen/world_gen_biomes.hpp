#pragma once

#include "world_height_map.hpp"
#include "../../../engine/rng.h"
#include "world_gen_hydrology.hpp"
#include <memory>
#include <vector>
#include <utility>

struct biome_t {
    std::vector<std::pair<uint16_t,uint16_t>> points;
    uint8_t climate;
    uint8_t difficulty;
    uint8_t biome_type;
};

struct biome_map_t {
    std::unique_ptr<heightmap_t> biome_map;
    std::vector<biome_t> biomes;
};

biome_map_t make_biome_map ( heightmap_t* heightmap, engine::random_number_generator * rng, water_level_map_t * water );
