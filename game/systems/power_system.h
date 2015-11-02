#pragma once

#include "../../engine/ecs.h"

namespace power_system_detail {
  extern int power_drain_this_round;
}

class power_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
private:
     long last_tick=0;
};
