#include "world_gen.hpp"
#include "planet/planet_builder.hpp"
#include <iostream>

constexpr int WORLD_LAYER=1;
constexpr int LOG_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void world_gen::init() {
	gui->add_layer(WORLD_LAYER, 0, 0, 800, 600, "8x8", resize_fullscreen, true);
	done = false;
	setup_build_planet(term(WORLD_LAYER)->term_width, term(WORLD_LAYER)->term_height);
	world_thread = std::make_unique<std::thread>(build_planet);
}

void world_gen::destroy() {
	gui->delete_layer(WORLD_LAYER);
	world_thread->join();
	world_thread.reset();
	planet_builder_display.reset();
}

void world_gen::tick(const double duration_ms) {
	term(WORLD_LAYER)->clear();

	planet_builder_lock.lock();
	for (int y=0; y<term(WORLD_LAYER)->term_height; ++y) {
		for (int x=0; x<term(WORLD_LAYER)->term_width; ++x) {
			const int idx = term(WORLD_LAYER)->at(x, y);
			term(WORLD_LAYER)->set_char(idx, (*planet_builder_display.get())[idx]);
		}
	}
	term(WORLD_LAYER)->print_center(1, " " + planet_builder_status + " ", YELLOW, BLACK);
	planet_builder_lock.unlock();

	if (is_planet_build_complete()) done = true;
}