#include "gui_console.h"

namespace engine {

void gui_console::render ( const screen_region viewport ) {
    const int n_lines = viewport.h;
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

}
