#pragma once

#include <rltk.hpp>
#include "../planet.hpp"

void create_settler(planet_t &planet, const int x, const int y, const int z, rltk::random_number_generator &rng, int shift_id);