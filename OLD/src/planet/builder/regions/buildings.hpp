#pragma once

#include <rltk.hpp>
#include "../../planet.hpp"

void add_building(std::string tag, const int x, const int y, const int z, const std::size_t &civ_owner);
void add_construction(const int x, const int y, const int z, const std::string type, bool solid, const std::size_t &civ_owner);
void build_escape_pod(const int crash_x, const int crash_y, const int crash_z);
void build_game_components(const int crash_x, const int crash_y, const int crash_z);
int build_building(xp::rex_sprite &sprite, const int x, const int y, const int z, std::vector<std::tuple<int,int,int>> &spawn_points, const bool active, const std::size_t &civ_id);
xp::rex_sprite get_building_template(const std::size_t civ_id, planet_t &planet, rltk::random_number_generator &rng);
void build_buildings(rltk::random_number_generator &rng, const int n_buildings, const bool active,
    std::vector<std::tuple<int,int,int>> &spawn_points, const std::size_t &civ_id, planet_t &planet);