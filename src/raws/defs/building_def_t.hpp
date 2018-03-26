#pragma once

#include "../reaction_input.hpp"
#include "../../bengine/rexspeeder.hpp"

enum provides_t { provides_sleep, provides_food, provides_seating, provides_desk, provides_door,
    provides_wall, provides_floor, provides_stairs_up, provides_stairs_down, provides_stairs_updown,
    provides_ramp, provides_light, provides_cage_trap, provides_stonefall_trap, provides_blades_trap,
    provides_spikes, provides_lever, provides_signal_recipient, provides_storage, provides_pressure_plate };

struct building_provides_t {
    provides_t provides;
    int energy_cost = 0;
    int radius = 0;
	int alternate_vox = 0;
    bengine::color_t color{1.0f, 1.0f, 1.0f};
};

struct building_def_t {
    std::string tag = "";
	std::string description = "";
    std::string name = "";
    std::vector<reaction_input_t> components;
    std::pair<std::string, int> skill;
    std::vector<building_provides_t> provides;
    int width = 1;
    int height = 1;
    std::vector<xp::vchar> glyphs;
    std::vector<xp::vchar> glyphs_ascii;
    bool emits_smoke = false;
    bool structure = false;
    int vox_model = 0;
	std::string blocked = "";
};
