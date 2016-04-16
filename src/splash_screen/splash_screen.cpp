#include <rltk.hpp>
#include "splash_screen.hpp"
#include <iostream>

constexpr int LOG_LAYER=1;
using namespace rltk;
using namespace rltk::colors;

void resize_splash_log(layer_t * l, int w, int h) {
	// Use the whole window
	l->w = w;
	l->h = h;
}

void splash_screen::tick(const double duration_ms) {
	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print(2, 2, "Hello World", WHITE, BLACK);
}

void splash_screen::init() {
	gui->add_layer(LOG_LAYER, 0, 0, 800, 600, "8x16", resize_splash_log);
}

void splash_screen::destroy() {
	gui->delete_layer(LOG_LAYER);
}
