#pragma once

#include <rltk.hpp>
#include "../../planet.hpp"
#include "../../region.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include <FastNoise/FastNoise.h>
#include <vector>
#include <utility>
#include <boost/container/flat_map.hpp>

struct region_water_feature_tile {
	region_water_feature_tile() {}
	region_water_feature_tile(int IDX, bool hasw, int d, int a) : idx(IDX), has_water_already(hasw), depth(d), altitude(a) {}
    int idx;
    bool has_water_already = false;
    int depth = 0;
    int altitude = REGION_DEPTH;
};

void add_dig_target(int X, int Y, int radius, int depth, boost::container::flat_map<int, region_water_feature_tile> &dig_targets, std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap);
void just_add_water(planet_t &planet, region_t &region, std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, rltk::random_number_generator &rng, FastNoise &noise, std::vector<std::pair<int, uint8_t>> &water_spawners);
