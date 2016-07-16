#pragma once

#include "../planet.hpp"
#include <rltk.hpp>

std::pair<int,int> builder_select_starting_region(planet_t &planet);
void build_region(planet_t &planet, std::pair<int,int> &target_region, rltk::random_number_generator &rng, rltk::perlin_noise &noise);
