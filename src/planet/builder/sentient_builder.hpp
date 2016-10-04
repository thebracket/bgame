#pragma once

#include <rltk.hpp>
#include "../region.hpp"
#include "../planet.hpp"

void create_sentient(const int x, const int y, const int z, rltk::random_number_generator &rng, 
        planet_t &planet, region_t &region, const std::size_t person_id, const bool announce=false);
