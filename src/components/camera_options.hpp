#pragma once

enum game_camera_mode_t { TOP_DOWN, FRONT, DIAGONAL };

struct camera_options_t {

	camera_options_t() = default;
	camera_options_t(const game_camera_mode_t mode, const bool ascii, const int zoom) noexcept : camera_mode(mode), ascii_mode(ascii), zoom_level(zoom) {}

    game_camera_mode_t camera_mode = TOP_DOWN;
    bool ascii_mode = false;
    int zoom_level = 12;    
};

