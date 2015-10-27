#pragma once

#include "base_system.h"

namespace engine {
namespace ecs {

class power_system : public base_system {
public:
     virtual void tick ( const double &duration_ms );
private:
     long last_tick=0;
};
  
}
}