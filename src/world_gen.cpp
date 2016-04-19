#include "world_gen.hpp"
#include "planet/planet_builder.hpp"
#include <iostream>

constexpr int WORLD_LAYER=1;
constexpr int LOG_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void world_gen::init() {
	gui->add_layer(WORLD_LAYER, 0, 0, 800, 600, "8x8", resize_fullscreen, false);
	gui->add_layer(LOG_LAYER, 0, 0, 800, 600, "8x16", resize_fullscreen, false);
	done = false;
	world_thread = std::make_unique<std::thread>(build_planet);
}

void world_gen::destroy() {
	gui->delete_layer(LOG_LAYER);
	gui->delete_layer(WORLD_LAYER);
	world_thread->join();
	world_thread.reset();
}

void world_gen::tick(const double duration_ms) {
	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print(2,2, "World Gen Goes Here");
	if (is_planet_build_complete()) done = true;
}