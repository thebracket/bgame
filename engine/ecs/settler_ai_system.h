#pragma once

#include "base_system.h"
#include "../../game/world/world.h"
#include "ecs.h"
#include "calendar_component.h"
#include "settler_ai_component.h"
#include "position_component.h"
#include "../rng.h"

namespace engine {
namespace ecs {


class settler_ai_system : public base_system {
     virtual void tick ( const double &duration_ms ) {
	  // Obtain a link to the calendar
	  entity * cordex = get_entity_by_handle ( world::cordex_handle );
          int calendar_handle = cordex->find_component_by_type ( calendar );
          calendar_component * calendar = get_component_by_handle<calendar_component> ( calendar_handle );	  
	  
	  vector<settler_ai_component *> settlers = find_components_by_type<settler_ai_component>(settler_ai);
	  for (settler_ai_component * settler : settlers) {
	      if (settler->next_tick <= calendar->system_time) {
		  // Time for the settler to do something!
		  entity * parent = get_entity_by_handle( settler->entity_id );
		  const int position_handle = parent->find_component_by_type(position);
		  position_component * pos = get_component_by_handle<position_component>(position_handle);
		
		  // For now, they will wander around aimlessly with no purpose or collision-detection.
		  int x = pos->x;
		  int y = pos->y;
		  bool moved = true;
		  
		  int direction = engine::roll_dice(1,5);
		  switch (direction) {
		    case 1 : --x; break;
		    case 2 : ++x; break;
		    case 3 : --y; break;
		    case 4 : ++y; break;
		    default : moved = false;
		  }
		  if (moved) {
		    if (x < 1) x = 1;
		    if (x > 255 ) x = 255;
		    if (y < 1 ) y = 1;
		    if (y > 255) y = 255;
		    pos->x = x;
		    pos->y = y;
		    pos->moved = true;
		  } else {
		    pos->moved = false;
		  }
		
		  // Random pause
		  settler->next_tick = calendar->system_time + 1;
	      }
	  }
     }

};

}
}
