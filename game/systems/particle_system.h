#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"

using engine::base_system;

class particle_system : public base_system {
public:
    particle_system() { system_name = "Particle System"; }
    virtual void tick(const double &duration_ms);
};
