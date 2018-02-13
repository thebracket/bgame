#pragma once

#include "../../planet.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include "../../../bengine/FastNoise.h"
#include "../../../bengine/random_number_generator.hpp"
#include <vector>

std::vector<uint8_t> create_empty_heightmap() noexcept;
void build_heightmap_from_noise(std::pair<int,int> &target, FastNoise &noise, std::vector<uint8_t> &heightmap, planet_t &planet) noexcept;
std::vector<int> create_subregions(planet_t &planet, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, bengine::random_number_generator &rng, std::vector<uint8_t> &pooled_water, std::vector<std::pair<int, uint8_t>> &water_spawners) noexcept;
