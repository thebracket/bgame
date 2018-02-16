#include "game_config.hpp"
#include "../bengine/string_utils.hpp"
#include "../bengine/filesystem.hpp"
#include <fstream>

namespace config {

    config_t game_config;

    void read_config() noexcept {
		const auto config_filename = get_save_path() + "/config.txt";
		if (!exists(config_filename))
		{
			game_config.save();
		}

        std::ifstream f(config_filename);
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
            if (split_line[0] == "fullscreen" && split_line[1]=="yes") game_config.fullscreen = true;
			if (split_line[0] == "fullscreen" && split_line[1] == "no") game_config.fullscreen = false;
			if (split_line[0] == "gui_ttf") game_config.gui_ttf = split_line[1];
            if (split_line[0] == "gui_ttf_size") game_config.gui_ttf_size = std::stoi(split_line[1]);
            if (split_line[0] == "scale_factor") game_config.scale_factor = std::stof(split_line[1]);
            if (split_line[0] == "allow_calling_home" && split_line[1] != "yes") game_config.allow_calling_home = false;
            if (split_line[0] == "online_username") game_config.online_username = split_line[1];
            if (split_line[0] == "show_entity_ids" && split_line[1]!="yes") game_config.show_entity_ids = false;
            if (split_line[0] == "build_tech_trees" && split_line[1]=="yes") game_config.build_tech_trees = true;
			if (split_line[0] == "texture_size") game_config.texture_size = std::stoi(split_line[1]);
			if (split_line[0] == "mip_levels") game_config.mip_levels = std::stoi(split_line[1]);
			if (split_line[0] == "render_ascii_light" && split_line[1] == "yes") game_config.render_ascii_light = true;
			if (split_line[0] == "render_ascii_light" && split_line[1] == "no") game_config.render_ascii_light = false;
			if (split_line[0] == "num_ascii_levels_below") game_config.num_ascii_levels_below = std::stoi(split_line[1]);
			if (split_line[0] == "shadow_map_size") game_config.shadow_map_size = std::stoi(split_line[1]);
			if (split_line[0] == "always_update_shadows" && split_line[1] == "yes") game_config.always_update_shadows = true;
			if (split_line[0] == "always_update_shadows" && split_line[1] == "no") game_config.always_update_shadows = false;
			if (split_line[0] == "shadow_divisor") game_config.shadow_divisor = std::stoi(split_line[1]);
			if (split_line[0] == "disable_lighting" && split_line[1] == "yes") game_config.disable_lighting = true;
			if (split_line[0] == "disable_lighting" && split_line[1] == "no") game_config.disable_lighting = false;
			if (split_line[0] == "disable_hdr" && split_line[1] == "yes") game_config.disable_hdr = true;
			if (split_line[0] == "disable_hdr" && split_line[1] == "no") game_config.disable_hdr = false;
			if (split_line[0] == "disable_ssao" && split_line[1] == "yes") game_config.disable_ssao = true;
			if (split_line[0] == "disable_ssao" && split_line[1] == "no") game_config.disable_ssao = false;
			if (split_line[0] == "ticks_per_ms") game_config.ticks_per_ms = std::stof(split_line[1]);
			if (split_line[0] == "vsync" && split_line[1] == "yes") game_config.vsync = true;
			if (split_line[0] == "vsync" && split_line[1] == "no") game_config.vsync = false;
			if (split_line[0] == "parallax" && split_line[1] == "yes") game_config.parallax = true;
			if (split_line[0] == "parallax" && split_line[1] == "no") game_config.parallax = false;
		}
		if (game_config.texture_size == 0 || (game_config.texture_size & (game_config.texture_size - 1)) != 0)
		{
			game_config.texture_size = 512; // Restore to default because it isn't a power of 2!
		}
    }

    void config_t::save() const noexcept {
		const auto config_filename = get_save_path() + "/config.txt";
        std::ofstream f(config_filename);
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
            f << "fullscreen=yes\n";
        } else {
            f << "fullscreen=no\n";
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
		f << "texture_size=" << texture_size << "\n";
		f << "mip_levels=" << mip_levels << "\n";
		f << "render_ascii_light=";
		if (render_ascii_light)
		{
			f << "yes\n";
		} else
		{
			f << "no\n";
		}
		f << "num_ascii_levels_below=" << num_ascii_levels_below << "\n";
		f << "shadow_map_size=" << shadow_map_size << "\n";
		f << "always_update_shadows=";
		if (always_update_shadows)
		{
			f << "yes\n";
		}
		else
		{
			f << "no\n";
		}
		f << "shadow_divisor=" << shadow_divisor << "\n";
		f << "disable_lighting=";
		if (disable_lighting)
		{
			f << "yes\n";
		}
		else
		{
			f << "no\n";
		}
		f << "disable_hdr=";
		if (disable_hdr)
		{
			f << "yes\n";
		}
		else
		{
			f << "no\n";
		}
		f << "disable_ssao=";
		if (disable_ssao)
		{
			f << "yes\n";
		}
		else
		{
			f << "no\n";
		}
		f << "ticks_per_ms=" << ticks_per_ms << "\n";
		f << "vsync=";
		if (vsync)
		{
			f << "yes\n";
		}
		else
		{
			f << "no\n";
		}
		f << "parallax=";
		if (parallax)
		{
			f << "yes\n";
		}
		else
		{
			f << "no\n";
		}
    }

}
