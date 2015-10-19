#pragma once

#include "base_system.h"
#include "../../game/world/world.h"
#include "ecs.h"
#include "calendar_component.h"

namespace engine {
namespace ecs {

const double TICK_LENGTH = 100.0;

class calendar_system : public base_system {
     virtual void tick ( const double &duration_ms ) {
          entity * cordex = get_entity_by_handle ( world::cordex_handle );
          int calendar_handle = cordex->find_component_by_type ( calendar );
          calendar_component * calendar = get_component_by_handle<calendar_component> ( calendar_handle );

          calendar->duration_buffer += duration_ms;
          if ( calendar->duration_buffer > TICK_LENGTH ) {
               calendar->duration_buffer = 0.0;
               ++calendar->system_time;
          }
     }

};

}
}
