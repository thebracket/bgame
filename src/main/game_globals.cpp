#include "game_globals.hpp"
#include <fstream>

config_t game_config;
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
game_camera_mode_t camera_mode = TOP_DOWN;
bool ascii_mode = false;

void config_t::save() {
    std::ofstream f("world_defs/config.txt");
    f << "window_width=" << window_width << "\n";
    f << "window_height=" << window_height << "\n";
    f << "game_font=" << game_font << "\n";
    f << "gui_font=" << gui_font << "\n";
    f << "game_font_small=" << game_font_small << "\n";
    if (tooltip_fadein) {
        f << "tooltip_fadein=yes\n";
    } else {
        f << "tooltip_fadein=no\n";
    }
    f << "tooltip_speed=" << tooltip_speed << "\n";
    f << "autosave_minutes=" << autosave_minutes << "\n";
    if (fullscreen) {
        f << "fullscreen=1\n";
    } else {
        f << "fullscreen=0\n";
    }
    f << "gui_ttf=" << gui_ttf << "\n";
    f << "gui_ttf_size=" << gui_ttf_size << "\n";
    f << "scale_factor=" << scale_factor << "\n";
    if (allow_calling_home) {
        f << "allow_calling_home=yes\n";
    } else {
        f << "allow_calling_home=no\n";
    }
    f << "online_username=" << online_username << "\n";
}