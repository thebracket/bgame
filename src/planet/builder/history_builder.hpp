#pragma once
#include "../planet.hpp"
#include <rltk.hpp>

void planet_build_initial_civs(planet_t &planet, rltk::random_number_generator &rng);
void planet_build_initial_history(planet_t &planet, rltk::random_number_generator &rng);
