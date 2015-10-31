#pragma once

#include "../world/world.h"
#include "../../engine/ecs.h"
#include "../components/position_component.h"
#include "../components/renderable_component.h"

class renderable_system : public base_system {
     virtual void tick ( const double &duration_ms );
};
