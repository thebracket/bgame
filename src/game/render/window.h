#pragma once

#include "../../engine/sdl2_backend.hpp"
#include "../raws/raws.h"
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <functional>

using std::string;
using std::vector;
using std::function;
using engine::sdl2_backend;

namespace render {

struct hotspot {
  SDL_Rect area;
  function<bool()> on_click;
  bool close_window = false;
};
  
class window {
public:
     window ( sdl2_backend * sdl, const string &title, const bool &decor ) : window_title ( title ), render_decor ( decor ), SDL ( sdl ) {}
     ~window();
     virtual bool render ( const int &mouse_x, const int &mouse_y, const bool clicked );
     void allocate();
     void render_decorations();

     string window_title = "";
     bool render_decor = true;
     sdl2_backend * SDL;

     string texture_id = "";
     SDL_Rect location;

     bool in_window ( const int &x, const int &y ) const {
          return ( x >= location.x and x < ( location.x + location.w ) and y>= location.y and y< ( location.y + location.h ) );
     }
     
     vector<hotspot> hotspots;
};

}
