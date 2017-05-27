#pragma once

#include <rltk.hpp>

void just_add_blight(rltk::random_number_generator &rng);
void build_ant_mound(rltk::random_number_generator &rng, std::vector<std::tuple<int,int,int>> &spawn_points);
