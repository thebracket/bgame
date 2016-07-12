#pragma once

#include "../planet.hpp"
#include <rltk.hpp>

void planet_zero_fill(planet_t &planet);
rltk::perlin_noise planet_noise_map(planet_t &planet, const int &perlin_seed);
int planet_determine_proportion(planet_t &planet, int &candidate, int target);
void planet_base_type_allocation(planet_t &planet);
void planet_mark_coastlines(planet_t &planet);
void planet_rainfall(planet_t &planet);