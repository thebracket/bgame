#pragma once

#include "../../engine/ecs.h"

class power_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
private:
     long last_tick=0;
};
