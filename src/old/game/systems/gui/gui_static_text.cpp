#include "gui_static_text.hpp"

void gui_static_text::render() {
	sdl2_backend * SDL = game_engine->get_backend();

	std::string line_s = SDL->render_text_to_image("disco10", message, "tmp", color);
	SDL->render_bitmap_simple(line_s, x, y);
}
