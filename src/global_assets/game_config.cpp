#include "game_config.hpp"
#include "../bengine/string_utils.hpp"
#include <fstream>

namespace config {

    config_t game_config;

    void read_config() {
        std::ifstream f("world_defs/config.txt");
        std::string line;
        while (getline(f, line))
        {
            auto split_line = split(line, '=');
            if (split_line[0] == "window_width") game_config.window_width = std::stoi(split_line[1]);
            if (split_line[0] == "window_height") game_config.window_height = std::stoi(split_line[1]);
            if (split_line[0] == "game_font") game_config.game_font = split_line[1];
            if (split_line[0] == "game_font_small") game_config.game_font_small = split_line[1];
            if (split_line[0] == "gui_font") game_config.gui_font = split_line[1];
            if (split_line[0] == "tooltip_fadein" && split_line[1] != "yes") game_config.tooltip_fadein = false;
            if (split_line[0] == "tooltip_speed") game_config.tooltip_speed = std::stof(split_line[1]);
            if (split_line[0] == "autosave_minutes") game_config.autosave_minutes = std::stoi(split_line[1]);
            if (split_line[0] == "fullscreen" && split_line[1]=="1") game_config.fullscreen = true;
            if (split_line[0] == "gui_ttf") game_config.gui_ttf = split_line[1];
            if (split_line[0] == "gui_ttf_size") game_config.gui_ttf_size = std::stoi(split_line[1]);
            if (split_line[0] == "scale_factor") game_config.scale_factor = std::stof(split_line[1]);
            if (split_line[0] == "allow_calling_home" && split_line[1] != "yes") game_config.allow_calling_home = false;
            if (split_line[0] == "online_username") game_config.online_username = split_line[1];
            if (split_line[0] == "show_entity_ids" && split_line[1]!="yes") game_config.show_entity_ids = false;
            if (split_line[0] == "build_tech_trees" && split_line[1]=="yes") game_config.build_tech_trees = true;

        }
    }

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
        f << "build_tech_trees=";
        if (build_tech_trees) {
            f << "yes\n";
        } else {
            f << "no\n";
        }
    }

}
