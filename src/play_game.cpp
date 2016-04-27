#include "play_game.hpp"
#include "game_globals.hpp"
#include "components/loader.hpp"

#include <rltk.hpp>
#include <iostream>
#include <Poco/InflatingStream.h>
#include <Poco/DeflatingStream.h>

using namespace rltk;
using namespace rltk::colors;

void play_game::tick(const double duration_ms) {
}

void play_game::init() {
	// Setup the display

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

	// Setup systems
}

void play_game::destroy() {
}
