#pragma once

/* Convenience header - includes the rest of the engine */

#include "rng.h"

namespace engine {

/* Initialize engine components; call at startup */
void init() {
    init_rng();
}


}
