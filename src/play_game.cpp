#include "play_game.hpp"
#include "game_globals.hpp"
#include "components/loader.hpp"
#include "components/components.hpp"

#include <rltk.hpp>
#include <iostream>
#include <Poco/InflatingStream.h>
#include <Poco/DeflatingStream.h>

using namespace rltk;
using namespace rltk::colors;

constexpr int WORLD_LAYER=1;

void play_game::tick(const double duration_ms) {
	term(WORLD_LAYER)->clear();
	term(WORLD_LAYER)->print(2,2,"Press Q to quit");

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		quitting = true;
	}
}

void play_game::init() {
	// Setup the display
	gui->add_layer(WORLD_LAYER, 0, 0, 800, 600, "8x8", resize_fullscreen, true);

	// Load the game
	planet = load_planet();

	// Load the ECS
	{
		const std::string save_filename = "world/savegame.dat";
		std::fstream lbfile(save_filename, std::ios::in | std::ios::binary);
		Poco::InflatingInputStream inflate(lbfile, Poco::InflatingStreamBuf::STREAM_GZIP);
		std::function<void(std::istream&,std::size_t,std::size_t)> helper(component_loader);
		ecs_load(inflate, helper);
	}

	// Load the current region - check the camera for the world position
	int region_x, region_y;
	each<world_position_t, calendar_t>([&region_x, &region_y] (entity_t &entity, world_position_t &pos, calendar_t &cal) {
		camera_entity = entity.id;
		region_x = pos.world_x;
		region_y = pos.world_y;
	});

	// Setup systems
	current_region = load_region(region_x, region_y);
}

void play_game::destroy() {
	gui->delete_layer(WORLD_LAYER);
}
