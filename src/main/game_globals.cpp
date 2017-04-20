#include "game_globals.hpp"

planet_t planet;
std::unique_ptr<region_t> current_region = std::make_unique<region_t>();
std::size_t camera_entity;
world_position_t * camera_position;
calendar_t * calendar;
designations_t * designations;
logger_t * logger;
int clip_left = -1;
int clip_right = -1;
int clip_top = -1;
int clip_bottom = -1;
rltk::random_number_generator rng;
pause_mode_t pause_mode = PAUSED;
game_master_mode_t game_master_mode = PLAY;
game_design_mode_t game_design_mode = DIGGING;
game_mining_mode_t game_mining_mode = DIG;
bool has_build_mode_building = false;
available_building_t build_mode_building;
std::size_t selected_settler;
std::size_t negotiating_civ;
bool quitting = false;
int selected_tile_x;
int selected_tile_y;
int selected_tile_z;
std::size_t current_stockpile=0;
camera_options_t * camera;
