#pragma once
#include "../planet.hpp"
#include "../../bengine/random_number_generator.hpp"

void planet_build_initial_civs(planet_t &planet, bengine::random_number_generator &rng) noexcept;
void planet_build_initial_history(planet_t &planet, bengine::random_number_generator &rng) noexcept;
