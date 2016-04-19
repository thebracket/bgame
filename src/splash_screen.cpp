#include <rltk.hpp>
#include "splash_screen.hpp"
#include "menu_helper.hpp"
#include <iostream>

constexpr int LOG_LAYER=1;
constexpr int BACKDROP_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void splash_screen::tick(const double duration_ms) {
	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print(2, 2, "Black Future 0.0.1a", WHITE, BLACK);
	term(LOG_LAYER)->print(2, 4, "Loading assets - please wait.", YELLOW, BLACK);

	// TODO: Check the loading thread here
	done_loading = true;
}

void splash_screen::init() {
	register_texture("../assets/background_image.png", "backdrop");
	gui->add_owner_layer(BACKDROP_LAYER, 0, 0, 800, 600, resize_fullscreen, draw_splash_backdrop);
	gui->add_layer(LOG_LAYER, 0, 0, 800, 600, "8x16", resize_fullscreen, false);
	// TODO: Start the loader thread.
}

void splash_screen::destroy() {
	gui->delete_layer(LOG_LAYER);
	gui->delete_layer(BACKDROP_LAYER);
}
