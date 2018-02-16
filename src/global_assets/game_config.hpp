#pragma once

#include <string>

namespace config {

	struct config_t {
		int window_width = 1400;
		int window_height = 900;
		std::string game_font = "24x24";
		std::string gui_font = "8x16";
		std::string game_font_small = "8x8";
		bool tooltip_fadein = true;
		float tooltip_speed = 100.0;
		int autosave_minutes = 5;
		bool fullscreen = true;
		std::string gui_ttf = "yanonekaffeesatz.ttf";
		int gui_ttf_size = 18;
		float scale_factor = 1.0f;
		bool allow_calling_home = true;
		std::string online_username = "anonymouse";
		bool show_entity_ids = true;
		bool build_tech_trees = false;
		int texture_size = 512;
		int mip_levels = 0;
		bool render_ascii_light = true;
		int num_ascii_levels_below = 2;
		int shadow_map_size = 128;
		bool always_update_shadows = false;
		int shadow_divisor = 20;
		bool disable_lighting = false;
		bool disable_hdr = false;
		bool disable_ssao = false;
		float ticks_per_ms = 33.0f;
		bool vsync = true;
		bool parallax = true;

		void save() const noexcept;
	};

	extern config_t game_config;
	void read_config() noexcept;

}