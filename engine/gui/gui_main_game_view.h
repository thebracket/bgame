#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include "../../game/world/world.h"
#include <string>
#include <iostream>
#include<algorithm>

using std::string;

namespace engine {
class gui_main_game_view : public gui_element_base {
public:
     gui_main_game_view ( const string name ) {
          tag = name;
     };;

     virtual void render ( const screen_region viewport ) {
          const int left_x = std::max ( 0, world::player_x - viewport.w/2 );
          const int top_y = std::max ( 0, world::player_y - viewport.h/2 );
          const int right_x = std::min ( landblock_width-1, world::player_x + viewport.w/2 );
          const int bottom_y = std::min ( landblock_height-1, world::player_y + viewport.h/2 );

          int screen_y = viewport.y;
          for ( int y=top_y; y<=bottom_y; ++y ) {
               int screen_x = viewport.x;
               for ( int x=left_x; x<=right_x; ++x ) {
                    tile t = world::current_region.tiles[world::current_region.idx ( x,y )];
                    if ( t.revealed ) {
                         if ( t.visible ) {
                              vterm::set_char_xy ( screen_x, screen_y, { t.display, t.foreground, t.background } );
                         } else {
                              vterm::set_char_xy ( screen_x, screen_y, { t.display, {64,64,64}, t.background } );
                         }
                    }
                    if ( x == world::player_x && y == world::player_y ) {
                         vterm::set_char_xy ( screen_x, screen_y, { '@', {255,255,255}, {0,0,0} } );
                    }
                    ++screen_x;
               }
               ++screen_y;
          }
     }

private:
};
}
