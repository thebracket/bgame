#include "gui_render_system.h"
#include "../game.h"
#include "../world/universe.hpp"
#include "render_ascii.hpp"

void gui_render_system::render_cursor(sdl2_backend * SDL, pair<int, int> &screen_size,	SDL_Rect &viewport, position_component3d * camera_pos)
{
	int mouse_x = engine::command::mouse_x;
	int mouse_y = engine::command::mouse_y;

	if (mouse_x > 0 and mouse_x < screen_size.first and mouse_y > 48
			and mouse_y < screen_size.second)
	{
		const int tile_x = mouse_x / 8;
		const int tile_y = (mouse_y - 48) / 8;

		const int16_t tilespace_x = tile_x + viewport.x;
		const int16_t tilespace_y = tile_y + viewport.y;

		if (tilespace_x == last_mouse_x and tilespace_y == last_mouse_y)
		{
			++mouse_hover_time;
		}
		else
		{
			mouse_hover_time = 0;
			if (tooltip_window)
				tooltip_window.reset();
		}
		last_mouse_x = tilespace_x;
		last_mouse_y = tilespace_y;

		const location_t target	{ camera_pos->pos.region, tilespace_x, tilespace_y,	camera_pos->pos.z };
		const int target_idx = get_tile_index(tilespace_x, tilespace_y,	camera_pos->pos.z);

		engine::vterm::screen_character cursor{ 176, color_t{ 255, 255, 0 }, color_t{ 0, 0, 0 } };
		if (!world::planet->get_region(target.region)->revealed[target_idx])
		{
			cursor.foreground_color = color_t{ 64, 64, 64 };
		}
		else
		{
			tile_t * target_tile = world::planet->get_tile(target);
			if (target_tile->flags.test(TILE_OPTIONS::SOLID))
			{
				cursor.foreground_color = color_t{ 255, 0, 0 };
			}
			if (target_tile->flags.test(TILE_OPTIONS::CAN_STAND_HERE))
			{
				cursor.foreground_color = color_t{ 0, 255, 255 };
			}
		}

		int alpha = 32 + (mouse_hover_time * 8);
		if (alpha > 200)
			alpha = 200;
		SDL->set_alpha_mod("font_s", alpha);
		SDL_Rect dest{ tile_x * 8, (tile_y * 8) + 48, 8, 8 };
		render_ascii(dest, cursor, SDL, 0, make_tuple(1.0,1.0,1.0),true);
		SDL->set_alpha_mod("font_s", 255);

		if (mouse_hover_time > 10 and world::planet->get_region(camera_pos->pos.region)->revealed[target_idx])
		{
			render_tooltip(SDL, target,	std::make_pair(tilespace_x - viewport.x, tilespace_y - viewport.y));
		}
	}
}

void gui_render_system::render_tooltip(sdl2_backend * SDL, const location_t &loc, const std::pair<int, int> mouse)
{
	if (tooltip_window)
	{
		tooltip_window->render(mouse_hover_time);
		return;
	}

	tooltip_window = std::make_unique<render::panel_tooltip>(SDL, loc,
			mouse);
	tooltip_window->render(mouse_hover_time);
}

void gui_render_system::tick(const double& duration_ms) {
	sdl2_backend * SDL = game_engine->get_backend();

	// Calculations - these are duplicated in map_render and should be refactored to be done once.
	position_component3d * camera_pos = get_camera_position();
	region_t * current_region = world::planet->get_region(camera_pos->pos.region);
	pair<int, int> screen_size = SDL->get_screen_size();
	screen_size.second -= 48;
	const int ascii_width = screen_size.first / 8;
	const int ascii_height = screen_size.second / 8;
	if (camera_pos->pos.x < ascii_width / 2) camera_pos->pos.x = ascii_width / 2;
	if (camera_pos->pos.x > REGION_WIDTH - (ascii_width / 2)) camera_pos->pos.x = REGION_WIDTH - (ascii_width / 2);
	if (camera_pos->pos.y < ascii_height / 2) camera_pos->pos.y = ascii_height / 2;
	if (camera_pos->pos.y > REGION_HEIGHT - (ascii_height / 2))	camera_pos->pos.y = REGION_HEIGHT - (ascii_height / 2);
	SDL_Rect viewport{ camera_pos->pos.x - (ascii_width / 2), camera_pos->pos.y	- (ascii_height / 2), ascii_width, ascii_height };

	// Render mode
	if (mode == normal)
	{
		render_cursor(SDL, screen_size, viewport, camera_pos);
	}
	else if (mode == radial)
	{
		if (!universe->globals.paused)
		{
			mode = normal;
		}
		else
		{
			// TODO: Render Radial
		}
	}
}
