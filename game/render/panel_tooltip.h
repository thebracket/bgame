#pragma once

#include "panel.h"
#include "line_buffer.h"
#include <utility>
#include <algorithm>
#include <memory>
#include <sstream>
#include "../game.h"
#include "../world/world.h"
#include "../raws/raws.h"

using std::stringstream;

namespace render {

class panel_tooltip {
public:
     panel_tooltip ( sdl2_backend * sdl, const location_t &loc, const std::pair<int,int> mouse_loc );     
     void render();
     bool render_me = true;
     
private:
     void render_buffer();
     void add_tile_contents ( const int region_x, const int region_y, const int region_z );
     bool add_settler_details ( const int entity_id );
     inline void add_name_details ( const int entity_id );
     inline void add_containers ( const int entity_id );

     int screen_x;
     int screen_y;
     std::unique_ptr<panel> panel_holder;
     line_buffer lines;
     sdl2_backend * SDL;
};

}