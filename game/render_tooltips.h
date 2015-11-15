#pragma once

#include "../engine/sdl2_backend.h"
#include <utility>

using namespace engine;

namespace render {
  
void tooltip( sdl2_backend * SDL, const std::pair<int,int> region_loc, const std::pair<int,int> mouse_loc );
  
}