#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"

using engine::base_system;

class input_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
};
