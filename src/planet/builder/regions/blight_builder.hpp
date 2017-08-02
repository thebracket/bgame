#pragma once

#include "../../../bengine/random_number_generator.hpp"

void just_add_blight(bengine::random_number_generator &rng);
void build_ant_mound(bengine::random_number_generator &rng, std::vector<std::tuple<int,int,int>> &spawn_points);
