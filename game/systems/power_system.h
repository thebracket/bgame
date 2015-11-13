#pragma once

#include "../../engine/ecs.hpp"

using engine::base_system;

class power_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
private:
     long last_tick=0;
};
