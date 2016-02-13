#include "gui_popup_menu.hpp"
#include "../../render/colors.h"
#include "../../raws/raws.h"

void gui_popup_menu::add_option(const gui_menu_option &option) {
	options.push_back(option);
}

void gui_popup_menu::render() {
	sdl2_backend * SDL = game_engine->get_backend();
	const int height = (options.size()+1) * 12;

	int x = screen_x + 8;
	int y = screen_y - (height / 2);

	// Background
	int width = SDL->text_size("disco10", title).first;
	for (const gui_menu_option &option : options) {
		int w = SDL->text_size("disco10", option.title).first;
		if (w > width) width = w;
	}

	int mouse_x = engine::command::mouse_x;
	int mouse_y = engine::command::mouse_y;
	if (engine::command::left_click and (mouse_x < x or mouse_x > x+width or mouse_y < y or mouse_y > y+height)) {
		request_close = true;
	}

	SDL_Rect r{ x,y,width,height };
	SDL_Rect src = raws::get_tile_source_by_name("WINDOWMASK");
	SDL->render_bitmap("spritesheet", src, r);

	// Lines

	std::string line_s = SDL->render_text_to_image("disco10", title, "tmp", render::sdl_cyan);
	SDL->render_bitmap_simple(line_s, x, y);
	y += 12;
	for (const gui_menu_option &option : options) {
		if (mouse_x > x and mouse_x < x+width and mouse_y>y and mouse_y<y+12) {
			line_s = SDL->render_text_to_image("disco10", option.title, "tmp", render::sdl_yellow);
			if (engine::command::left_click) {
				request_close = true;
				option.action();
			}
		} else {
			line_s = SDL->render_text_to_image("disco10", option.title, "tmp", render::sdl_white);
		}
		SDL->render_bitmap_simple(line_s, x, y);
		y += 12;
	}
}
