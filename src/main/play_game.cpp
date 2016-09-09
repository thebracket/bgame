#include "play_game.hpp"
#include "game_globals.hpp"
#include "../components/loader.hpp"
#include "../components/components.hpp"
#include "../systems/systems.hpp"

#include <rltk.hpp>
#include <iostream>

using namespace rltk;
using namespace rltk::colors;

constexpr int MAP_LAYER=1;
constexpr int GUI_LAYER=2;
constexpr int RIGHT_PANEL = 3;
constexpr int TOOLTIP_LAYER = 4;

void play_game::tick(const double duration_ms) {
	ecs_tick(duration_ms);
}

void resize_game_panel(rltk::layer_t * l, int w, int h) {
	l->w = w;
	l->h = h;
}

void play_game::init() {
	auto window_size = get_window()->getSize();
	const int window_width = window_size.x;
	const int window_height = window_size.y;

	// Setup the display
	gui->add_layer(MAP_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_game_panel, true);
	gui->add_layer(TOOLTIP_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_game_panel, false);
	gui->add_layer(GUI_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_fullscreen, false);
	gui->add_layer(RIGHT_PANEL, 0, 0, window_width, window_height, game_config.gui_font, resize_game_panel, false);

	term(TOOLTIP_LAYER)->clear();

	// Load the game
	std::cout << "Loading the planet\n";
	planet = load_planet();

	// Load the ECS
	std::cout << "Loading game state\n";
	{
		const std::string save_filename = "world/savegame.dat";
		std::fstream lbfile(save_filename, std::ios::in | std::ios::binary);
		//Poco::InflatingInputStream inflate(lbfile, Poco::InflatingStreamBuf::STREAM_GZIP);
		std::function<void(std::istream&,std::size_t,std::size_t)> helper(component_loader);
		ecs_load(lbfile, helper);
	}

	// Load the current region - check the camera for the world position
	std::cout << "Storing important entity handles\n";

	int region_x, region_y;
	each<world_position_t, calendar_t, designations_t>([&region_x, &region_y] (entity_t &entity, world_position_t &pos, calendar_t &cal, designations_t &design) {
		camera_entity = entity.id;
		region_x = pos.world_x;
		region_y = pos.world_y;
		camera_position = &pos;
		calendar = &cal;
		designations = &design;
	});
	std::cout << "Loading the region\n";
	*current_region = load_region(region_x, region_y);

	// Setup systems
	std::cout << "Setting up systems\n";
	add_system<keyboard_system>();
	add_system<calendar_system>();
	add_system<world_system>();
	add_system<wildlife_population_system>();
	add_system<sentient_ai_system>();
	add_system<corpse_system>();
	add_system<particle_system>();
	add_system<renderables_system>();
	add_system<camera_system>();
	add_system<lighting_system>();
	add_system<mining_system>();
	add_system<inventory_system>();
	add_system<power_system>();
	add_system<workflow_system>();
	add_system<settler_ai_system>();
	add_system<mode_rogue_system>();
	add_system<movement_system>();
	add_system<damage_system>();
	add_system<topology_system>();
	add_system<visibility_system>();
	add_system<vegetation_system>();
	add_system<map_render_system>();
	add_system<panel_render_system>();
	add_system<mode_units_system>();
	add_system<mode_rogue_render_system>();

	std::cout << "ECS Config\n";
	ecs_configure();
	std::cout << "Go!\n";	
}

void play_game::destroy() {
	gui->delete_layer(MAP_LAYER);
	gui->delete_layer(TOOLTIP_LAYER);
	gui->delete_layer(GUI_LAYER);
	gui->delete_layer(RIGHT_PANEL);
}
