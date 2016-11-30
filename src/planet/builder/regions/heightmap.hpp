#pragma once

#include <rltk.hpp>
#include "../../planet.hpp"
#include "../../region.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include <FastNoise/FastNoise.h>
#include <vector>

std::vector<uint8_t> create_empty_heightmap();
void build_heightmap_from_noise(std::pair<int,int> &target, FastNoise &noise, std::vector<uint8_t> &heightmap, planet_t &planet);
std::vector<int> create_subregions(planet_t &planet, region_t &region, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, rltk::random_number_generator &rng, std::vector<uint8_t> &pooled_water, std::vector<std::pair<int, uint8_t>> &water_spawners);
