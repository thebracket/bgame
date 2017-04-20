#pragma once

#include "../planet/region.hpp"
#include "../raws/reaction_input.hpp"
#include "../components/calendar.hpp"
#include "../components/designations.hpp"
#include "../components/logger.hpp"
#include "../components/world_position.hpp"
#include "../components/camera_options.hpp"
#include <rltk.hpp>
#include <memory>

struct available_building_t {
	available_building_t() {}
	available_building_t(const std::string &n, const std::string &t) : name(n), tag(t) {}

	std::string name = "";
	std::string tag = "";
	std::vector<reaction_input_t> components;
    int width = 0;
    int height = 0;
    std::vector<rltk::vchar> glyphs;
	std::vector<rltk::vchar> glyphs_ascii;
	int n_existing = 0;
	std::string get_name() const noexcept {
		return name + std::string(" (") + std::to_string(n_existing) + std::string(")");
	}
    bool structure = false;
};

enum game_master_mode_t { PLAY, DESIGN, UNITS, SETTLER, WORKFLOW, ROGUE, CIVS, CIV_NEGOTIATE, 
	STANDING_ORDERS, TILEMENU, SENTIENT_INFO, GRAZER_INFO, WISHMODE, TRIGGER_MANAGEMENT };
enum game_design_mode_t { DIGGING, BUILDING, CHOPPING, GUARDPOINTS, STOCKPILES, HARVEST, ARCHITECTURE };
enum game_mining_mode_t { DIG, CHANNEL, RAMP, UP, DOWN, UPDOWN, MINING_DELETE };

extern std::unique_ptr<region_t> current_region;
extern std::size_t camera_entity;
extern world_position_t * camera_position;
extern calendar_t * calendar;
extern designations_t * designations;
extern logger_t * logger;
extern camera_options_t * camera;
extern rltk::random_number_generator rng;
extern game_master_mode_t game_master_mode;
extern game_design_mode_t game_design_mode;
extern game_mining_mode_t game_mining_mode;
extern bool has_build_mode_building;
extern available_building_t build_mode_building;
extern std::size_t selected_settler;
extern std::size_t current_stockpile;
extern bool quitting;
extern std::size_t negotiating_civ;
extern int selected_tile_x;
extern int selected_tile_y;
extern int selected_tile_z;

