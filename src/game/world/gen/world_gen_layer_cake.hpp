#pragma once

#include "world_height_map.hpp"
#include "world_gen_hydrology.hpp"
#include "../../../engine/rng.hpp"
#include <memory>
#include "../planet.hpp"
#include "world_gen_biomes.hpp"

std::unique_ptr < planet_t > make_world_layers ( heightmap_t* base_map, engine::random_number_generator& rng, water_level_map_t* water, biome_map_t * biomes );
