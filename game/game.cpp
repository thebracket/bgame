#include "game.h"
#include "loading_screen.h"
#include "components/component_loader.h"
#include "world/world.h"
#include <memory>

using std::make_unique;

my_engine * game_engine;

void run_game() {
    game_engine = new my_engine( component_factory, world::load_world_constants, world::save_world_constants );
    game_engine->init();
    game_engine->main_loop( make_unique<loading_screen>() );
    delete game_engine;
}
