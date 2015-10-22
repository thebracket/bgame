#include "calendar_system.h"

namespace engine {
namespace ecs {
 
void calendar_system::tick ( const double &duration_ms ) {
          entity * cordex = get_entity_by_handle ( world::cordex_handle );
          int calendar_handle = cordex->find_component_by_type ( calendar );
          calendar_component * calendar = get_component_by_handle<calendar_component> ( calendar_handle );

          calendar->duration_buffer += duration_ms;
          if ( calendar->duration_buffer > TICK_LENGTH ) {
               calendar->duration_buffer = 0.0;
               ++calendar->system_time;
	       world::sun_angle += 1.0F;
	       if (world::sun_angle > 180.0F) world::sun_angle = 0.0F;
          }
     }
  
}
}