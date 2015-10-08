#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include <string>
#include <iostream>
#include "../../game/world/world.h"

using std::string;

namespace engine {
class gui_console : public gui_element_base {
public:  
     gui_console ( const string name ) {
          tag = name;
     };;

     virtual void render ( const screen_region viewport ) {
          const int n_lines = viewport.h;
          const int width = viewport.w - viewport.x;
          int y = viewport.y;
          for ( auto it = world::log.last_n ( n_lines ); it != world::log.end(); ++it ) {
               log_entry entry = *it;
               int x = viewport.x;
               for ( console_character chr : entry.line ) {
                    const vterm::screen_character schr {
                         chr.character,chr.color, black
                    };
                    vterm::set_char_xy ( x,y,schr );
                    ++x;
               }
               ++y;
          }
     }

private:
};
}
