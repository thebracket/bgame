#pragma once

#include "../planet.hpp"
#include <rltk.hpp>
#include "../../FastNoise/FastNoise.h"

void planet_zero_fill(planet_t &planet);
FastNoise planet_noise_map(planet_t &planet, const int &perlin_seed);
int planet_determine_proportion(planet_t &planet, int &candidate, int target);
void planet_base_type_allocation(planet_t &planet);
void planet_mark_coastlines(planet_t &planet);
void planet_rainfall(planet_t &planet);
void planet_rivers(planet_t &planet, rltk::random_number_generator &rng);