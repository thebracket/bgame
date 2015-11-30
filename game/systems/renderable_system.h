#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"
#include "../components/position_component.h"
#include "../components/renderable_component.h"

using engine::base_system;

class renderable_system : public base_system {
public:
     renderable_system() { system_name = "Renderable System"; }
     virtual void tick ( const double &duration_ms );
};
