#pragma once

struct config_t {
	int window_width = 800;
	int window_height = 600;
	std::string game_font = "8x8";
	std::string gui_font = "8x16";
	std::string game_font_small = "8x8";
	bool tooltip_fadein = true;
	float tooltip_speed = 100.0;
	int autosave_minutes = 5;
	bool fullscreen = false;
	std::string gui_ttf = "DroidSans.ttf";
	int gui_ttf_size = 24;
	float scale_factor = 1.0f;
	bool allow_calling_home = true;
    std::string online_username = "anonymouse";
    bool show_entity_ids = true;

    void save();
};

extern config_t game_config;
