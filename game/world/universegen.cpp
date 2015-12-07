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
               if ( finder == universe->solar_systems.end() and (( system.universe_x != UNIVERSE_WIDTH/2 ) or ( system.universe_y != UNIVERSE_HEIGHT/2 )) ) {
                    good = true;
                    detail_star_system ( system );
                    universe->solar_systems [ system.universe_idx ] = system;
               }
          }
     }
     // Force Eden to be at the center of the universe
     solar_system_t eden;
     eden.binary_system = false;
     eden.system_class = M;
     solar_system_body_t homeworld;
     homeworld.body_type = HABITABLE_PLANET;
     homeworld.system_idx = 0;
     eden.bodies.push_back( homeworld );
     eden.universe_x = UNIVERSE_WIDTH / 2;
     eden.universe_y = UNIVERSE_HEIGHT / 2;
     eden.universe_idx = universe_idx( eden.universe_x, eden.universe_y );
     eden.inhabited_by_man = true;
     universe->solar_systems [ eden.universe_idx ] = eden;
     
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
     int n_bodies = game_engine->rng.roll_dice(1,8)+2;
     if (n_bodies < 1) n_bodies = 1;
     
     for (int i=0; i<n_bodies; ++i) {
	add_planetary_body ( i, system );
     }
     bool habitable = false;
     for (int i=0; i<n_bodies; ++i) {
	if ( system.bodies[i].body_type == HABITABLE_PLANET or system.bodies[i].body_type == SPACE_STATION ) habitable = true;
     }
     if (!habitable) {
	solar_system_body_t body;
	body.system_idx = n_bodies+1;
	body.body_type = HABITABLE_PLANET;
	system.bodies.push_back( body );
     }
}

void universe_generator::add_planetary_body ( int i, solar_system_t& system )
{
     // TODO: More interesting items!
     solar_system_body_t body;
     body.system_idx = i;
     body.body_type = HABITABLE_PLANET;
     const int roll = game_engine->rng.roll_dice(1,7);
     switch (roll) {
       case 1 : body.body_type = ICE_WORLD; break;
       case 2 : body.body_type = VOLCANIC_WORLD; break;
       case 3 : body.body_type = GAS_CLOUD; break;
       case 4 : body.body_type = ASTEROIDS; break;
       case 5 : body.body_type = WARP_HUSK; break;
       case 6 : body.body_type = HABITABLE_PLANET; break;
       case 7 : body.body_type = SPACE_STATION; break;
     }
     system.bodies.push_back( body );
}

void universe_generator::spread_humanity()
{
    for (int y=0; y<UNIVERSE_HEIGHT; ++y) {
	  for (int x=0; x<UNIVERSE_WIDTH; ++x) {
	      const int uidx = universe_idx(x,y);
	      auto finder = universe->solar_systems.find ( uidx );
	      
	      const int dx = std::abs( x - (UNIVERSE_WIDTH/2) );
	      const int dy = std::abs( y - (UNIVERSE_HEIGHT/2) );
	      const int distance = std::sqrt( (dx*dx) + (dy*dy) );	
	      if (distance < 15 and finder != universe->solar_systems.end()) finder->second.inhabited_by_man = true;
	  }
    }
}

void universe_generator::spread_ownership()
{
    for (int y=0; y<UNIVERSE_HEIGHT; ++y) {
	  for (int x=0; x<UNIVERSE_WIDTH; ++x) {
	      const int uidx = universe_idx(x,y);
	      auto finder = universe->solar_systems.find ( uidx );
	      
	      const int dx = std::abs( x - (UNIVERSE_WIDTH/2) );
	      const int dy = std::abs( y - (UNIVERSE_HEIGHT/2) );
	      const int distance = std::sqrt( (dx*dx) + (dy*dy) );	
	      if (distance < 15 and finder != universe->solar_systems.end()) {
		  int ownership = game_engine->rng.roll_dice(1, 6);
		  switch (ownership) {
		    case 1 : finder->second.owner = ARSAK; break;
		    case 2 : finder->second.owner = CARVAZ; break;
		    case 3 : finder->second.owner = HERITEZ; break;
		    case 4 : finder->second.owner = LAMINTREZ; break;
		    case 5 : finder->second.owner = ERENTAR; break;
		    case 6 : finder->second.owner = SYL; break;
		  }
	      }
	  }
    }
}

