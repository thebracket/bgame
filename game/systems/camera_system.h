#pragma once

#include "../world/world.h"
#include "../../engine/ecs/ecs.h"
#include "../components/position_component.h"
#include "../../engine/backends/command_manager.h"

namespace engine {
namespace ecs {

class camera_system : public base_system {
    virtual void tick(const double &duration_ms);
};

}
}