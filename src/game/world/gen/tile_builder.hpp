#pragma once

#include "../tile3d.hpp"
#include "world_gen_biomes.hpp"
#include "../../../engine/rng.hpp"

void build_tile( const biome_t &biome, tile_t * tile, engine::random_number_generator &rng );
