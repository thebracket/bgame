#ifdef WIN32
#include <windows.h>
#endif
#include "world_gen.hpp"
#include "../planet/planet_builder.hpp"
#include "game_config.hpp"
#include "game_globals.hpp"
#include "../utils/telemetry.hpp"
#include "menu_helper.hpp"
#include "../systems/gui/imgui_helper.hpp"
#include "../external/imgui-sfml/imgui-SFML.h"
#include "../utils/gl/worldgen_render.hpp"

constexpr int WORLD_LAYER=1;
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
	setup_build_planet();
	call_home("worldgen");
	seed = rng.roll_dice(1, std::numeric_limits<int>::max());
    optional_display_hook = rltk::optional_display_hook;
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

void render_hook();

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
        rltk::optional_display_hook = render_hook;
	}
	ImGui::End();
}

void render_hook() {
    planet_builder_lock.lock();

    worldgen_scene scene;
    scene.layer_texture = WORLD_LAYER;

    for (int y=0; y<WORLD_HEIGHT-1; ++y) {
        for (int x = 0; x < WORLD_WIDTH - 1; ++x) {
            const int idx = planet.idx(x,y);
            const float altitude = (*planet_builder_display.get())[idx].altitude;

            float red = (float)(*planet_builder_display.get())[idx].foreground.r / 255.0f;
            float green = (float)(*planet_builder_display.get())[idx].foreground.g / 255.0f;
            float blue = (float)(*planet_builder_display.get())[idx].foreground.b / 255.0f;

            const float slope = 1.0f - std::abs(altitude - (*planet_builder_display.get())[planet.idx(x+1, y)].altitude) / 6.0f;
            red *= slope;
            green *= slope;
            blue *= slope;

            const float X = ((float)x - WORLD_WIDTH/2.0f);
            const float Y = ((float)y - WORLD_HEIGHT/2.0f);

            worldgen_tile tile;
            tile.x = X;
            tile.y = Y;
            tile.red = red;
            tile.green = green;
            tile.blue = blue;
            tile.glyph = (*planet_builder_display.get())[idx].terrain_glyph;
            tile.alt1 = altitude;
            tile.alt2 = (*planet_builder_display.get())[planet.idx(x, y + 1)].altitude;
            tile.alt3 = (*planet_builder_display.get())[planet.idx(x + 1, y + 1)].altitude;
            tile.alt4 = (*planet_builder_display.get())[planet.idx(x + 1, y)].altitude;
            scene.add_tile(tile);

            if ((*planet_builder_display.get())[planet.idx(x,y+1)].unit_glyph > 0) {
                worldgen_unit unit;
                unit.x = X;
                unit.y = Y;
                unit.altitude = altitude;
                unit.glyph = (*planet_builder_display.get())[planet.idx(x,y+1)].unit_glyph;
                scene.add_unit(unit);
            }
        }
    }
    scene.render();

    planet_builder_lock.unlock();

    // Call ImGui
    ImGui::Render();
}

void world_gen::tick(const double duration_ms) {
	term(WORLD_LAYER)->clear();

	if (mode == WG_MENU) {
		render_menu();
	} else {
        planet_builder_lock.lock();
        ImGui::Begin("World Generation Progress");
        ImGui::Text("%s",planet_builder_status.c_str());
        ImGui::End();
        planet_builder_lock.unlock();

		if (is_planet_build_complete()) {
            done = true;
            rltk::optional_display_hook = optional_display_hook;
        }
	}
}
