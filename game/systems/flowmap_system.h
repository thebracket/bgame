#pragma once

#include "../world/world.h"
#include "../../engine/ecs.h"
#include <vector>

using std::vector;

namespace flowmaps {
  
extern vector<short> food_flow_map;
extern vector<short> water_flow_map;
  
}

class flowmap_system : public base_system {
public:
     flowmap_system() {
	  flowmaps::food_flow_map.resize( landblock_height * landblock_width );
	  flowmaps::water_flow_map.resize( landblock_height * landblock_width );
     }
     
     virtual void tick ( const double &duration_ms );
};
