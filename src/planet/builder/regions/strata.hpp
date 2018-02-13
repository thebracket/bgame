#pragma once

#include "../../../bengine/FastNoise.h"
#include "../../planet.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include "../../../raws/defs/biome_type_t.hpp"
#include "../../../bengine/random_number_generator.hpp"

struct strata_t {
    std::vector<int> strata_map;
    std::vector<std::size_t> material_idx;
    std::vector<std::tuple<int,int,int,int>> counts;
};

strata_t build_strata(std::vector<uint8_t> &heightmap, bengine::random_number_generator &rng, std::pair<biome_t, biome_type_t> &biome, planet_t &planet) noexcept;
void lay_strata(std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, strata_t &strata, bengine::random_number_generator &rng, std::vector<uint8_t> &pools, std::vector<std::pair<int, uint8_t>> &water_spawners) noexcept;
