#pragma once

#include "window.h"
#include "../world/world.h"
#include "line_buffer.h"
#include "colors.h"
#include "../raws/raws.h"
#include <sstream>

namespace render {

class build_options_window : public window {
public:
     build_options_window ( sdl2_backend* sdl, const string& title, const bool& decor, const int rx, const int ry ) : window ( sdl,title,decor ), region_x ( rx ), region_y ( ry ) {
          line_buffer lines;

          const int idx = world::current_region->idx ( region_x, region_y );

          determine_buildables ( idx, lines );

          const int height = ( lines.size() *16 ) +24;
          location = { 0, 0, lines.get_width() +32, height };

          allocate();
          render_decorations();

          SDL->texture_target ( texture_id );

          const int x = 16;
          int y = 16;
          for ( const std::pair<std::string,SDL_Color> &panel_line : lines.lines ) {
               std::string line_s = SDL->render_text_to_image ( "disco14", panel_line.first, "tmp", panel_line.second );
               SDL->render_bitmap_simple ( line_s, x, y );
               y += 16;
          }

          SDL->reset_texture_target();
     }

     void determine_buildables ( const int &idx, line_buffer &lines ) {
          // Bail out if not flat
          if ( world::current_region->tiles[idx].base_tile_type != tile_type::FLAT ) {
               lines.add_line ( SDL, "You can only build on flat terrain.", sdl_red );
               return;
          }

          // Bail out if there is already something here
          bool occupied = false;
          vector<position_component *> positions = game_engine->ecs->find_components_by_func<position_component> (
          [this] ( const position_component &c ) {
               return ( c.x == this->region_x and c.y == this->region_y );
          } );

          for ( const position_component * pos : positions ) {
               settler_ai_component * settler = game_engine->ecs->find_entity_component<settler_ai_component> ( pos->entity_id );
               if ( settler == nullptr ) {
                    occupied = true;
               }
          }
          if ( occupied ) {
               lines.add_line ( SDL, "There is already a structure here: demolish it before building a new one.", sdl_red );
               return;
          }

          // Iterate the raws and look for something buildable!
          vector< pair<string,vector<string>>> buildables = raws::get_buildables();
          for ( const pair<string,vector<string>> &buildable : buildables ) {
	       bool have_components = true;
	       int available = 0;
	       
	       for ( const string &component : buildable.second ) {
		  auto finder = world::inventory.find( component );
		  if (finder == world::inventory.end()) {
		    have_components = false;
		  } else {
		    available = finder->second.size();
		  }
	       }
	       
	       if (have_components) {
		  lines.add_line ( SDL, buildable.first, sdl_yellow );
		  
		  for ( const string &component : buildable.second ) {
			std::stringstream ss;
			ss << "  Requires: " << component << " (" << available << " available)";
			lines.add_line ( SDL, ss.str(), sdl_white );
		  }
	       }
          }
          if ( buildables.empty() ) {
               // Default
               lines.add_line ( SDL, "Nothing can be built here.", sdl_red );
          }
     }

private:
     const int region_x;
     const int region_y;
};

}
