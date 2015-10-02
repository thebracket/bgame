#pragma once

/* Convenience header - includes the rest of the engine */

#include "rng.h"
#include "virtual_terminal.h"

namespace engine {

/* Initialize engine components; call at startup */
void init();
void main_loop();


}
