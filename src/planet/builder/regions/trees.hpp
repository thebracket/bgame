#pragma once

#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include "../../../raws/defs/biome_type_t.hpp"
#include "../../planet.hpp"
#include "../../../bengine/random_number_generator.hpp"

void build_trees(std::pair<biome_t, biome_type_t> &biome, bengine::random_number_generator &rng);
