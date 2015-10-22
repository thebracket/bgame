#pragma once

#include "base_system.h"
#include "../../game/world/world.h"
#include "ecs.h"
#include "calendar_component.h"

namespace engine {
namespace ecs {

const double TICK_LENGTH = 100.0;

class calendar_system : public base_system {
     virtual void tick ( const double &duration_ms );
};

}
}
