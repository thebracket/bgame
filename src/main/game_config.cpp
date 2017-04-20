#include "game_config.hpp"
#include <fstream>

config_t game_config;

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
    if (show_entity_ids) {
        f << "show_entity_ids=yes\n";
    } else {
        f << "show_entity_ids=no\n";
    }
}