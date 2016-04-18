#include <rltk.hpp>
#include "splash_screen.hpp"
#include <iostream>

constexpr int LOG_LAYER=1;
constexpr int BACKDROP_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void resize_splash_log(layer_t * l, int w, int h) {
	// Use the whole window
	l->w = w;
	l->h = h;
}

void draw_bg(layer_t * l, sf::RenderTexture &window) {
        sf::Texture * bg = get_texture("backdrop");
        sf::Sprite backdrop(*bg);
        window.draw(backdrop);
	// TODO: Add scaling here
}

void splash_screen::tick(const double duration_ms) {
	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print(2, 2, "Black Future 0.0.1a", WHITE, BLACK);
	term(LOG_LAYER)->print(2, 4, "Loading assets - please wait.", YELLOW, BLACK);
}

void splash_screen::init() {
	register_texture("../assets/background_image.png", "backdrop");
	gui->add_owner_layer(BACKDROP_LAYER, 0, 0, 800, 600, resize_splash_log, draw_bg);
	gui->add_layer(LOG_LAYER, 0, 0, 800, 600, "8x16", resize_splash_log, false);
}

void splash_screen::destroy() {
	gui->delete_layer(LOG_LAYER);
	gui->delete_layer(BACKDROP_LAYER);
}
