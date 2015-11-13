#pragma once

#include "../../engine/ecs.hpp"

using engine::base_system;

class obstruction_system : public base_system {
     virtual void tick ( const double &duration_ms );
};
