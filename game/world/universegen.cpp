#include "universegen.hpp"
#include "universe.hpp"
#include "../game.h"

universe_generator::universe_generator()
{

}

void universe_generator::make_universe()
{
    universe->solar_systems.clear();
    const int numer_of_systems = ( UNIVERSE_WIDTH * UNIVERSE_HEIGHT ) / 6;
    for (int i=0; i < numer_of_systems; ++i) {
	bool good = false;
	
	while (!good) {
	    solar_system_t system;
	    system.universe_x = game_engine->rng.roll_dice(1, UNIVERSE_WIDTH-1 );
	    system.universe_y = game_engine->rng.roll_dice(1, UNIVERSE_HEIGHT-1 );
	    system.universe_idx = universe_idx(system.universe_x,system.universe_y);
	    auto finder = universe->solar_systems.find ( system.universe_idx );
	    if (finder == universe->solar_systems.end() and (system.universe_x != UNIVERSE_WIDTH/2) and (system.universe_y != UNIVERSE_HEIGHT/2)) {
	      good = true;
	      // TODO: Generate system details
	      universe->solar_systems [ system.universe_idx ] = system;
	    }
	}
    }
    // Force Eden to be at the center of the universe
    universe->save();
}
