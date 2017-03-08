#include "world_gen.hpp"
#include "../planet/planet_builder.hpp"
#include "game_globals.hpp"
#include "../utils/telemetry.hpp"
#include "menu_helper.hpp"
#include "../systems/gui/imgui_helper.hpp"
#include "../external/imgui-sfml/imgui-SFML.h"
#include <iostream>

constexpr int WORLD_LAYER=1;
constexpr int LOG_LAYER=2;
constexpr int BACKDROP_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void world_gen::init() {
	auto window_size = get_window()->getSize();
	const int window_width = window_size.x;
	const int window_height = window_size.y;

    gui->add_owner_layer(BACKDROP_LAYER, 0, 0, window_width, window_height, resize_fullscreen, draw_splash_backdrop);
	gui->add_layer(WORLD_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_fullscreen, true);
	done = false;
	setup_build_planet(term(WORLD_LAYER)->term_width, term(WORLD_LAYER)->term_height);
	call_home("worldgen");
	seed = rng.roll_dice(1, std::numeric_limits<int>::max());
}

void world_gen::destroy() {
	gui->delete_layer(WORLD_LAYER);
	world_thread->join();
	world_thread.reset();
	planet_builder_display.reset();
    call_home("worldgen_done");
}

void world_gen::start_thread() {
	world_thread = std::make_unique<std::thread>(build_planet, seed, water, plains, starting_settlers, strict_beamdown);
}

void world_gen::render_menu() {
	ImGui::Begin("World Generation");

	ImGui::InputInt("World Seed", &seed);
	ImGui::SliderInt("Water Level", &water, 1, 4);
	ImGui::SliderInt("Plains Level", &plains, 1, 4);
	ImGui::SliderInt("Starting Settlers", &starting_settlers, 1, 20);
	ImGui::Checkbox("Require Teleport Beacon for Beamdown", &strict_beamdown);
	if (ImGui::Button("Create World")) {
		mode = WG_RUNNING;
		start_thread();
        gui->delete_layer(BACKDROP_LAYER);
	}
	ImGui::End();
}

void world_gen::tick(const double duration_ms) {
	term(WORLD_LAYER)->clear();

	if (mode == WG_MENU) {
		render_menu();
	} else {
		planet_builder_lock.lock();
		for (int y = 0; y < term(WORLD_LAYER)->term_height; ++y) {
			for (int x = 0; x < term(WORLD_LAYER)->term_width; ++x) {
				const auto idx = term(WORLD_LAYER)->at(x, y);
				term(WORLD_LAYER)->set_char(idx, (*planet_builder_display.get())[idx]);
			}
		}
		term(WORLD_LAYER)->print_center(1, " " + planet_builder_status + " ", YELLOW, BLACK);
		planet_builder_lock.unlock();

		if (is_planet_build_complete()) done = true;
	}
}
