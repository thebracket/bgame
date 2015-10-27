#pragma once

#include "base_system.h"
#include "../power_generator_component.h"

namespace engine {
namespace ecs {

class power_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
private:
     int calculate_power_gain(const power_generator_component * gen);
     long last_tick=0;
};
  
}
}