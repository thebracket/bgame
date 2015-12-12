#include "game_mode.hpp"
#include "../engine/command_manager.h"
#include <utility>
#include "world/planet.hpp"
#include "world/world_defs.hpp"
#include "../engine/virtual_terminal.h"
#include <sstream>

using std::make_pair;
using namespace engine;
using namespace engine::command;
using engine::vterm::color_t;

void game_mode::init()
{
     finished = false;
     world::planet->load_region ( world::planet->planet_idx ( 1, 1 ) );
     x = REGION_WIDTH/2;
     y = REGION_HEIGHT/2;
     z = 32;
}

void game_mode::done()
{

}

pair< engine::return_mode, unique_ptr< engine::base_mode > > game_mode::tick ( const double time_elapsed )
{
     const uint8_t region_idx = world::planet->planet_idx ( 1, 1 );

     // Very temporary!
     pair<int,int> size = vterm::get_screen_size();
     for ( int Y = 0; Y<size.second; ++Y ) {
          for ( int X = 0; X<size.first; ++X ) {
               uint8_t wx = X - ( size.first/2 ) + x;
               uint8_t wy = Y - ( size.second/2 ) + y;
               const int tile_idx = world::planet->regions[ region_idx  ]->tile_idx ( wx, wy, z );

               tile_t * tile = world::planet->get_tile ( location_t { region_idx, wx, wy, z } );
               vterm::screen_character c;
               if ( tile->solid ) {
                    c.character = 219;
                    c.background_color = color_t {0,0,0};
                    c.foreground_color = color_t {128,128,128};
               } else {
                    c.character = 176;
                    c.background_color = color_t {0,255,255};
                    c.foreground_color = color_t {0,0,0};
               }
               
               if ( tile->water_level > 0 ) {
                    c.background_color = color_t {0,0,255};
                    c.character = 126;
               }
               if ( !tile->solid and tile->base_tile_type == tile_type::FLAT ) {
                    c.foreground_color = color_t {0,255,0};
		    c.background_color = color_t {0,0,0};
                    c.character = 96;
               }
               if ( !tile->solid and tile->base_tile_type == tile_type::RAMP ) {
                    c.foreground_color = color_t {0,255,0};
		    c.background_color = color_t {0,0,0};
                    c.character = 30;
               }
               
               if (tile->water_level > 0) {
		    c.character = 126;
		    c.foreground_color = color_t { 0,0,255 };
		    c.background_color = color_t { 0,0,64 };
               }
               engine::vterm::set_char_xy ( X, Y, c );
          }
     }

     std::stringstream ss;
     ss << "X: " << +x << ", Y: " << +y << ", Z: " << +z << ". Q to quit";
     engine::vterm::print ( 1, 1, ss.str(), engine::vterm::color_t {255,255,255}, engine::vterm::color_t {0,0,0} );

      // Temporary
     if ( is_key_down ( Q ) ) finished = true;
     if ( is_key_down ( UP ) ) y--;
     if ( is_key_down ( DOWN ) ) y++;
     if ( is_key_down ( LEFT ) ) x--;
     if ( is_key_down ( RIGHT ) ) x++;
     if ( is_key_down ( COMMA ) ) z++;
     if ( is_key_down ( PERIOD ) ) z--;

     if ( finished ) {
          return make_pair ( POP, NO_PUSHED_MODE );
     } else {
          return make_pair ( CONTINUE, NO_PUSHED_MODE );
     }
}
