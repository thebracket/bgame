#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"

using engine::base_system;

class cordex_ai_system : public base_system {
    virtual void tick(const double &duration_ms);
};
