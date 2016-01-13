#pragma once

#include <memory>
#include <string>
#include <utility>

#include "../../engine/ecs.hpp"
#include "../render/panel_tooltip.h"

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
	std::unique_ptr<render::panel_tooltip> tooltip_window;

	void render_tooltip(sdl2_backend * SDL, const location_t &loc, const std::pair<int, int> mouse);
	void render_cursor(sdl2_backend * SDL, pair<int, int> &screen_size,	SDL_Rect &viewport, position_component3d * camera_pos);
};
