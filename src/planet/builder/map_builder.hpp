#pragma once

#include "../planet.hpp"
#include "../../bengine/FastNoise.h"
#include "../../bengine/random_number_generator.hpp"

void planet_zero_fill(planet_t &planet) noexcept;
FastNoise planet_noise_map(planet_t &planet, const int &perlin_seed) noexcept;
int planet_determine_proportion(planet_t &planet, int &candidate, int target) noexcept;
void planet_base_type_allocation(planet_t &planet) noexcept;
void planet_mark_coastlines(planet_t &planet) noexcept;
void planet_rainfall(planet_t &planet) noexcept;
void planet_rivers(planet_t &planet, bengine::random_number_generator &rng) noexcept;