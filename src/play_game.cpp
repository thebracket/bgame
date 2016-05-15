#include "play_game.hpp"
#include "game_globals.hpp"
#include "components/loader.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"

#include <rltk.hpp>
#include <iostream>
//#include <Poco/InflatingStream.h>
//#include <Poco/DeflatingStream.h>

using namespace rltk;
using namespace rltk::colors;

constexpr int MAP_LAYER=1;
constexpr int GUI_LAYER=2;

void play_game::tick(const double duration_ms) {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		quitting = true;
	}

	ecs_tick(duration_ms);
}

void play_game::init() {
	// Setup the display
	gui->add_layer(MAP_LAYER, 0, 0, 800, 600, "8x8", resize_fullscreen, true);
	gui->add_layer(GUI_LAYER, 0, 0, 800, 600, "8x8", resize_fullscreen, false);

	// Load the game
	planet = load_planet();

	// Load the ECS
	{
		const std::string save_filename = "world/savegame.dat";
		std::fstream lbfile(save_filename, std::ios::in | std::ios::binary);
		//Poco::InflatingInputStream inflate(lbfile, Poco::InflatingStreamBuf::STREAM_GZIP);
		std::function<void(std::istream&,std::size_t,std::size_t)> helper(component_loader);
		ecs_load(lbfile, helper);
	}

	// Load the current region - check the camera for the world position
	int region_x, region_y;
	each<world_position_t, calendar_t>([&region_x, &region_y] (entity_t &entity, world_position_t &pos, calendar_t &cal) {
		camera_entity = entity.id;
		region_x = pos.world_x;
		region_y = pos.world_y;
		camera_position = &pos;
		calendar = &cal;
	});
	current_region = load_region(region_x, region_y);

	// Setup systems
	add_system<calendar_system>();
	add_system<map_render_system>();
}

void play_game::destroy() {
	gui->delete_layer(MAP_LAYER);
	gui->delete_layer(GUI_LAYER);
}
