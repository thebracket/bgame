#pragma once

#include "../../region.hpp"
#include <rltk.hpp>

void just_add_blight(region_t &region, random_number_generator &rng);
void build_ant_mound(region_t &region, random_number_generator &rng, std::vector<std::tuple<int,int,int>> &spawn_points);
