#pragma once

#include <memory>
#include <string>
#include <utility>

#include "../../engine/ecs.hpp"
#include "../../engine/engine.hpp"
#include "../components/position_component3d.hpp"

using engine::base_system;

enum game_mode_t
{
	normal, radial
};

class gui_render_system : public base_system
{
public:
	gui_render_system()
	{
		system_name = "GUI Render System";
	}
	virtual void tick(const double &duration_ms);

private:
	int last_mouse_x = 0;
	int last_mouse_y = 0;
	int mouse_hover_time = 0;
	game_mode_t mode = normal;

	int radial_screen_x = 0;
	int radial_screen_y = 0;
	int radial_tilespace_x = 0;
	int radial_tilespace_y = 0;
	int radial_tilespace_z = 0;

	void render_cursor(engine::sdl2_backend * SDL, std::pair<int, int> &screen_size, SDL_Rect &viewport, position_component3d * camera_pos);
};
