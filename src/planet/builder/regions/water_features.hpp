#pragma once

#include "../../planet.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include "../../../bengine/FastNoise.h"
#include "../../../bengine/random_number_generator.hpp"
#include <vector>
#include <utility>

struct region_water_feature_tile {
	region_water_feature_tile() = default;
	region_water_feature_tile(const int IDX, const bool hasw, const int d, const int a) noexcept : idx(IDX), has_water_already(hasw), depth(d), altitude(a) {}
    int idx;
    bool has_water_already = false;
    int depth = 0;
    int altitude = REGION_DEPTH;
};

void add_dig_target(int X, int Y, int radius, int depth, std::unordered_map<int, region_water_feature_tile> &dig_targets, std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap) noexcept;
void just_add_water(planet_t &planet, std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, bengine::random_number_generator &rng, FastNoise &noise, std::vector<std::pair<int, uint8_t>> &water_spawners) noexcept;
