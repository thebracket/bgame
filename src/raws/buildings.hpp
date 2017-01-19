#pragma once

#include <rltk.hpp>
#include <string>
#include <vector>
#include <boost/container/flat_map.hpp>
#include "reaction_input.hpp"

enum provides_t { provides_sleep, provides_food, provides_seating, provides_desk, provides_door,
    provides_wall, provides_floor, provides_stairs_up, provides_stairs_down, provides_stairs_updown,
    provides_ramp, provides_light, provides_cage_trap, provides_stonefall_trap, provides_blades_trap };

struct building_provides_t {
    provides_t provides;
    int energy_cost = 0;
    int radius = 0;
    rltk::color_t color = rltk::colors::WHITE;
};

struct building_def_t {
    std::string tag = "";
    std::string name = "";
    std::vector<reaction_input_t> components;
    std::pair<std::string, int> skill;
    std::vector<building_provides_t> provides;
    int width = 1;
    int height = 1;
    std::vector<rltk::vchar> glyphs;
    bool emits_smoke = false;
    bool structure = false;
};

extern boost::container::flat_map<std::string, building_def_t> building_defs;

void read_buildings(std::ofstream &tech_tree_file) noexcept;
void sanity_check_buildings() noexcept;