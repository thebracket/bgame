#pragma once

#include "../../engine/sdl2_backend.h"
#include <utility>

using std::pair;
using engine::sdl2_backend;

namespace render {
 
    void tile_options( sdl2_backend * SDL, const std::pair<int,int> region_loc, const std::pair<int,int> mouse_loc );
  
}