#pragma once

#include "../world/world.h"
#include "../../engine/ecs.h"

class input_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
};
