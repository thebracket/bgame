#pragma once

/* Convenience header - includes the rest of the engine */

#include "rng.h"
#include "virtual_terminal.h"
#include "colors.h"
#include "base_mode.h"
#include "gui/gui.h"
#include "command_manager.h"
#include "ecs/ecs.h"

namespace engine {

enum backend_mode { NCURSES, SDL2, ANSI };
  
/* Initialize engine components; call at startup */
void init(backend_mode mode);
void main_loop ( unique_ptr<base_mode> starting_mode );


}
