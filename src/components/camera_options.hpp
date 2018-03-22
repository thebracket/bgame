#pragma once

enum game_camera_mode_t { TOP_DOWN, FRONT, DIAGONAL_LOOK_NW, DIAGONAL_LOOK_NE, DIAGONAL_LOOK_SW, DIAGONAL_LOOK_SE };

struct camera_options_t {

	camera_options_t() = default;
	camera_options_t(const game_camera_mode_t mode, const bool ascii, const int zoom) noexcept : camera_mode(mode), ascii_mode(ascii), zoom_level(zoom) {}

    game_camera_mode_t camera_mode = game_camera_mode_t::TOP_DOWN;
    bool ascii_mode = false;
    int zoom_level = 12;
	bool perspective = true;
	int following = 0;
	bool fps = false;
};

