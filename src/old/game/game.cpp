#include "game.h"
#include "loading_screen.h"
#include "components/component_loader.h"
#include "world/universe.hpp"
#include <memory>

using std::make_unique;

my_engine * game_engine;

void run_game()
{
	game_engine = new my_engine(component_factory, load_universe_state, save_universe_state);
	game_engine->init("Black Future", 1024, 768);
	game_engine->main_loop(make_unique<loading_screen>());
	delete game_engine;
}
