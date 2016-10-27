#pragma once

#include <rltk.hpp>
#include "../../planet.hpp"
#include "../../region.hpp"

void add_building(std::string tag, const int x, const int y, const int z);
void add_construction(region_t &region, const int x, const int y, const int z, const std::string type, bool solid);
void build_escape_pod(region_t &region, const int crash_x, const int crash_y, const int crash_z);
void build_game_components(region_t &region, const int crash_x, const int crash_y, const int crash_z);
int build_building(xp::rex_sprite &sprite, const int x, const int y, const int z, region_t &region, std::vector<std::tuple<int,int,int>> &spawn_points, const bool active);
xp::rex_sprite get_building_template(const std::size_t civ_id, planet_t &planet, rltk::random_number_generator &rng);
void build_buildings(region_t &region, rltk::random_number_generator &rng, const int n_buildings, const bool active, 
    std::vector<std::tuple<int,int,int>> &spawn_points, const std::size_t &civ_id, planet_t &planet);