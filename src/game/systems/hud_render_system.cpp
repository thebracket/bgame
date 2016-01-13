#include "hud_render_system.h"
#include "../game.h"
#include "../world/universe.hpp"
#include "../render/colors.h"

void hud_render_system::tick(const double& duration_ms) {
	render_hud_base();
	render_power_bar();
	render_date_time();
	render_paused();
}

void hud_render_system::render_hud_base() {
	sdl2_backend * SDL = game_engine->get_backend();
	SDL_Rect all{ 0, 0, 1024, 48 };
	SDL->render_bitmap("header", all, all);
}

void hud_render_system::render_power_bar() {
	sdl2_backend * SDL = game_engine->get_backend();

	const float power_percent = static_cast<float>(universe->globals.stored_power) / static_cast<float>(universe->globals.max_power);
	const int power_tenths = (power_percent * 10.0) - 1;
	SDL_Rect src
	{ power_tenths * 46, 0, 46, 48 };
	SDL_Rect dest
	{ 4, 0, 48, 48 };
	SDL->render_bitmap("cordex", src, dest);

	std::stringstream ss;
	ss << "Power: " << universe->globals.stored_power << " I " << universe->globals.max_power;
	string emote_text = SDL->render_text_to_image("lcd10", ss.str(), "tmp",
			render::sdl_dark_grey);
	SDL->render_bitmap_simple(emote_text, 68, 22);
}

void hud_render_system::render_date_time() {
	sdl2_backend * SDL = game_engine->get_backend();

	calendar_component * calendar = ECS->find_entity_component<calendar_component>(universe->globals.cordex_handle);

	if (calendar->display_day_month.empty())
	{
		calendar->display_day_month = " ";
	}
	if (calendar->display_time.empty())
	{
		calendar->display_time = " ";
	}
	const std::string the_date = game_engine->render_text_to_image("lcd10",
			calendar->display_day_month, "btn_playgame",
			render::sdl_dark_grey);
	const std::string the_time = game_engine->render_text_to_image("lcd10",
			calendar->display_time, "btn_playgame", render::sdl_dark_grey);
	SDL->render_bitmap_simple(the_date, 68, 8);
	SDL->render_bitmap_simple(the_time, 178, 8);
}

void hud_render_system::render_paused() {
	sdl2_backend * SDL = game_engine->get_backend();

	if (universe->globals.paused)
	{
		const std::string paused = game_engine->render_text_to_image(
				"lcd10", "PAUSED", "btn_playgame", render::sdl_dark_grey);
		SDL->render_bitmap_simple(paused, 310, 8);
	}
}
