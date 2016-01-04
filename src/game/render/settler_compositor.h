#pragma once

#include "../../engine/sdl2_backend.hpp"

using engine::sdl2_backend;

namespace render {
  
void render_settler_composite ( sdl2_backend * SDL, const int &entity_id, const int &x, const int &y );
  
}
