#include "universegen.hpp"
#include "../game.h"
#include <vector>
#include <utility>

using std::vector;
using std::pair;

universe_generator::universe_generator()
{

}

void universe_generator::make_universe()
{
     universe->solar_systems.clear();
     const int numer_of_systems = ( UNIVERSE_WIDTH * UNIVERSE_HEIGHT ) / 6;
     for ( int i=0; i < numer_of_systems; ++i ) {
          bool good = false;

          while ( !good ) {
               solar_system_t system;
               system.universe_x = game_engine->rng.roll_dice ( 1, UNIVERSE_WIDTH-1 );
               system.universe_y = game_engine->rng.roll_dice ( 1, UNIVERSE_HEIGHT-1 );
               system.universe_idx = universe_idx ( system.universe_x,system.universe_y );
               auto finder = universe->solar_systems.find ( system.universe_idx );
               if ( finder == universe->solar_systems.end() and ( system.universe_x != UNIVERSE_WIDTH/2 ) and ( system.universe_y != UNIVERSE_HEIGHT/2 ) ) {
                    good = true;
                    detail_star_system ( system );
                    universe->solar_systems [ system.universe_idx ] = system;
               }
          }
     }
     // Force Eden to be at the center of the universe
     universe->save();
}

system_class_t random_star_type()
{
     int roll = game_engine->rng.roll_dice ( 1, 3000000 );
     if ( roll < 2280000 ) return M;
     roll -= 2280000;
     if ( roll < 357000 ) return K;
     roll -= 357000;
     if ( roll < 225000 ) return G;
     roll -= 225000;
     if ( roll < 90900 ) return F;
     roll -= 90900;
     if ( roll < 18750 ) return A;
     roll -= 18750;
     if ( roll < 3750 ) return B;
     return O;
}

void universe_generator::detail_star_system ( solar_system_t& system )
{
     if ( game_engine->rng.roll_dice ( 1,20 ) ==1 ) {
          system.binary_system = true;
     } else {
          system.binary_system = false;
     }
     system.system_class = random_star_type();
     add_planetary_bodies ( system );
}

void universe_generator::add_planetary_bodies ( solar_system_t& system )
{
     int n_bodies = game_engine->rng.roll_dice(1,20)-6;
     if (n_bodies < 1) return;
     
     for (int i=0; i<n_bodies; ++i) {
	add_planetary_body ( i, system );
     }
}

void universe_generator::add_planetary_body ( int i, solar_system_t& system )
{
     // TODO: More interesting items!
     solar_system_body_t body;
     body.system_idx = i;
     body.body_type = HABITABLE_PLANET;
     system.bodies.push_back( body );
}

