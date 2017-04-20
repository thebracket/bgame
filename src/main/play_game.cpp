#include "play_game.hpp"
#include "game_globals.hpp"
#include "game_config.hpp"
#include "game_planet.hpp"
#include "../systems/systems.hpp"
#include "../components/calendar.hpp"
#include "../components/designations.hpp"
#include "../utils/telemetry.hpp"
#include "../systems/gui/imgui_helper.hpp"
#include "../external/imgui-sfml/imgui-SFML.h"
#include "../utils/gl/map_render.hpp"
#include "../utils/filesystem.hpp"

#include <rltk.hpp>
#include <iostream>
#include <thread>
#include <atomic>

using namespace rltk;
using namespace rltk::colors;

constexpr int MAP_LAYER=1;
constexpr int GUI_LAYER=2;
constexpr int RIGHT_PANEL = 3;
constexpr int TOOLTIP_LAYER = 4;
constexpr int RENDERABLES_LAYER = 5;
constexpr int VEG_LAYER=6;

std::atomic<bool> loaded(false);
std::thread * loader_thread = nullptr;

void render_hook_gameplay() {
    if (loaded) {
        map_render_t map3d;
        map3d.render();
    }

    // Call ImGui
    ImGui::Render();
}

void loader_tick() {
    term(GUI_LAYER)->clear();
    term(MAP_LAYER)->clear();
    term(RIGHT_PANEL)->clear();
    term(TOOLTIP_LAYER)->clear();
    //term(GUI_LAYER)->print_center(4, "Loading game - please wait.", YELLOW, BLACK);
    ImGui::Begin("Loading, please wait...");
    ImGui::Text("The game is loading, please wait.");
    ImGui::End();
}

void play_game::tick(const double duration_ms) {
    if (loaded) {
        ecs_tick(duration_ms);
    } else {
        loader_tick();
        if (loaded) {
            loader_thread->join();
            delete loader_thread;
            loader_thread = nullptr;
        }

    }
}

void resize_game_panel(rltk::layer_t * l, int w, int h) {
	l->w = w;
	l->h = h;
}

void do_load_game() {
    call_home("playgame");
    // Load the game
    std::cout << "Loading the planet\n";
    planet = load_planet();

    // Load the ECS
    std::cout << "Loading game state\n";
    {
        const std::string save_filename = get_save_path() + std::string("/savegame.dat");
        std::unique_ptr<std::ifstream> lbfile = std::make_unique<std::ifstream>(save_filename, std::ios::in | std::ios::binary);
        ecs_load(lbfile);
    }

    // Load the current region - check the camera for the world position
    std::cout << "Storing important entity handles\n";

    int region_x, region_y;
    each<world_position_t, calendar_t, designations_t, logger_t, camera_options_t>([&region_x, &region_y]
                           (entity_t &entity, world_position_t &pos, calendar_t &cal, designations_t &design,
                            logger_t &log, camera_options_t &camera_prefs) {
        camera_entity = entity.id;
        region_x = pos.world_x;
        region_y = pos.world_y;
        camera_position = &pos;
        calendar = &cal;
        designations = &design;
        logger = &log;
        camera = &camera_prefs;
    });
    std::cout << "Loading the region\n";
    *current_region = load_region(region_x, region_y);

    // Setup systems
    std::cout << "Setting up systems\n";
    add_systems_to_ecs();

    std::cout << "ECS Config\n";
    ecs_configure();
    std::cout << "Go!\n";

    loaded = true;

    optional_display_hook = rltk::optional_display_hook;
    rltk::optional_display_hook = render_hook_gameplay;
}

void play_game::init() {
	auto window_size = get_window()->getSize();
	const int window_width = window_size.x;
	const int window_height = window_size.y;

	// Setup the display
	gui->add_layer(MAP_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_fullscreen, true);
    gui->add_sparse_layer(VEG_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_fullscreen);
    gui->add_sparse_layer(RENDERABLES_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_fullscreen);
	gui->add_layer(TOOLTIP_LAYER, 0, 0, window_width, window_height, game_config.game_font_small, resize_fullscreen, true);
	gui->add_layer(GUI_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_fullscreen, true);
	gui->add_layer(RIGHT_PANEL, 0, 0, window_width, window_height, game_config.gui_font, resize_fullscreen, true);
	term(RIGHT_PANEL)->set_alpha(220);

	term(TOOLTIP_LAYER)->clear();

    loader_thread = new std::thread(do_load_game);
}

void play_game::destroy() {
    rltk::optional_display_hook = optional_display_hook;
    call_home("stopgame");
	gui->delete_layer(MAP_LAYER);
	gui->delete_layer(TOOLTIP_LAYER);
	gui->delete_layer(GUI_LAYER);
	gui->delete_layer(RIGHT_PANEL);
    gui->delete_layer(RENDERABLES_LAYER);
    gui->delete_layer(VEG_LAYER);
}
