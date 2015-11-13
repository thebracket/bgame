#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"
#include "../components/position_component.h"

using engine::base_system;

class camera_system : public base_system {
    virtual void tick(const double &duration_ms);
};
