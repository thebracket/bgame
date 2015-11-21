#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"
#include <vector>

using std::vector;
using engine::base_system;

namespace flowmaps {
  
extern vector<short> food_flow_map;
extern vector<short> water_flow_map;
extern vector<short> sleep_flow_map;
extern vector<int> food_flow_map_entity_id;
extern vector<int> water_flow_map_entity_id;
extern vector<int> sleep_flow_map_entity_id;

}

class flowmap_system : public base_system {
public:
     flowmap_system() {
	  flowmaps::food_flow_map.resize( landblock_height * landblock_width );
	  flowmaps::water_flow_map.resize( landblock_height * landblock_width );
	  flowmaps::food_flow_map_entity_id.resize( landblock_height * landblock_width );
	  flowmaps::water_flow_map_entity_id.resize( landblock_height * landblock_width );
	  flowmaps::sleep_flow_map.resize ( landblock_height * landblock_width );
	  flowmaps::sleep_flow_map_entity_id.resize ( landblock_height * landblock_width );
     }
     
     virtual void tick ( const double &duration_ms );
};
