#pragma once

#include "window.h"
#include "../world/world.h"
#include "line_buffer.h"
#include "colors.h"

namespace render {

class build_options_window : public window {
public:
     build_options_window ( sdl2_backend* sdl, const string& title, const bool& decor, const int rx, const int ry ) : window ( sdl,title,decor ), region_x ( rx ), region_y ( ry ) {
          line_buffer lines;
	  
	  const int idx = world::current_region->idx(region_x, region_y);

          lines.add_line ( SDL, "Build options will go here.", sdl_white );

	  const int height = (lines.size()*16)+24;
	  location = { 0, 0, lines.get_width()+32, height };
	  
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

private:
     const int region_x;
     const int region_y;
};

}
