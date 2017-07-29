#pragma once

#include "../planet.hpp"
#include <rltk.hpp>
#include "../../FastNoise/FastNoise.h"

std::pair<int,int> builder_select_starting_region(planet_t &planet);
void build_region(planet_t &planet, std::pair<int,int> &target_region, rltk::random_number_generator &rng, FastNoise &noise);
