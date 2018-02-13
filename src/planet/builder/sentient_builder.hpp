#pragma once

#include "../planet.hpp"
#include "../../bengine/random_number_generator.hpp"

void create_sentient_unit(planet_t &planet, bengine::random_number_generator &rng, std::size_t civ_id,
                          const std::string &unit_tag,
                          std::vector<std::tuple<int,int,int>> &starting_points, int &spawn_counter,
                          const bool announce=false, int crash_x=0, int crash_y=0, int crash_z=0) noexcept;
