#pragma once

#include "../planet.hpp"
#include "../../bengine/random_number_generator.hpp"

void create_settler(planet_t &planet, const int x, const int y, const int z, bengine::random_number_generator &rng, int shift_id) noexcept;