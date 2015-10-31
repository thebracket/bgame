#pragma once

#include "../world/world.h"
#include "../../engine/ecs.h"
#include "../components/position_component.h"
#include "../../engine/backends/command_manager.h"


class camera_system : public base_system {
    virtual void tick(const double &duration_ms);
};
