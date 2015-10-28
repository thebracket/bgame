#pragma once

/* Convenience header - includes the rest of the engine */

#include "rng.h"
#include "virtual_terminal.h"
#include "colors.h"
#include "base_mode.h"
#include "gui/gui.h"
#include "backends/command_manager.h"
#include "ecs/ecs.h"
#include "backends/output_backend.h"

namespace engine {
  
/* Initialize engine components; call at startup */
void init( const engine::backend_mode mode );
void main_loop ( unique_ptr<base_mode> starting_mode );


}
