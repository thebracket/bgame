#pragma once

#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include <rltk.hpp>
#include "../../region.hpp"
#include "../../planet.hpp"

void build_trees(region_t &region, std::pair<biome_t, biome_type_t> &biome, rltk::random_number_generator &rng);