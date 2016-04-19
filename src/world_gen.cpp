#include "world_gen.hpp"

constexpr int LOG_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void world_gen::init() {
	gui->add_layer(LOG_LAYER, 0, 0, 800, 600, "8x16", resize_fullscreen, false);
}

void world_gen::destroy() {
	gui->delete_layer(LOG_LAYER);
}

void world_gen::tick(const double duration_ms) {
	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print(2,2, "World Gen Goes Here");
}