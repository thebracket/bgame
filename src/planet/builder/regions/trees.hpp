#pragma once

#include "../../../raws/raws.hpp"
#include "../../../raws/biomes.hpp"
#include "../../../raws/defs/biome_type_t.hpp"
#include <rltk.hpp>
#include "../../planet.hpp"

void build_trees(std::pair<biome_t, biome_type_t> &biome, rltk::random_number_generator &rng);