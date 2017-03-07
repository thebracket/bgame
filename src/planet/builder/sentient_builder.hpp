#pragma once

#include <rltk.hpp>
#include "../region.hpp"
#include "../planet.hpp"

void create_sentient_unit(planet_t &planet, region_t &region, rltk::random_number_generator &rng, std::size_t civ_id,
                          const std::string &unit_tag,
                          std::vector<std::tuple<int,int,int>> &starting_points, int &spawn_counter,
                          const bool announce=false);
