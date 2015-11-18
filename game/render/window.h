#pragma once

#include "../../engine/sdl2_backend.h"
#include "../raws/raws.h"
#include <string>
#include <memory>
#include <iostream>

using std::string;
using engine::sdl2_backend;

namespace render {

class window {
public:
     window ( sdl2_backend * sdl, const string &title, const bool &decor ) : window_title ( title ), render_decor ( decor ), SDL ( sdl ) {}
     ~window();
     virtual void render();
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
};

}