#pragma once

#include "../planet.hpp"
#include "../../bengine/FastNoise.h"
#include "../../bengine/random_number_generator.hpp"

std::pair<int,int> builder_select_starting_region(planet_t &planet);
void build_region(planet_t &planet, std::pair<int,int> &target_region, bengine::random_number_generator &rng, FastNoise &noise, const bool &ascii_mode);
