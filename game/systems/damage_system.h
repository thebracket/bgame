#pragma once

#include "../../engine/ecs.hpp"

using engine::base_system;

class damage_system : public base_system {
public:
     damage_system() { system_name = "Damage System"; }
     virtual void tick ( const double &duration_ms );
};
