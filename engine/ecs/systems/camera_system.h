#pragma once

#include "base_system.h"
#include "../../../game/world/world.h"
#include "../ecs.h"
#include "../position_component.h"
#include "../../command_manager.h"

namespace engine {
namespace ecs {

class camera_system : public base_system {
    virtual void tick(const double &duration_ms);
};

}
}