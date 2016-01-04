#pragma once

#include "world_height_map.hpp"
#include "../../../engine/rng.hpp"

void make_noisy_heightmap(heightmap_t * base_map, engine::random_number_generator * rng);
