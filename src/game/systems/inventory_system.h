#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"
#include "../game.h"
#include <vector>
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::vector;
using std::string;
using engine::base_system;

class inventory_system : public base_system {
public:
     inventory_system() {
	system_name = "Inventory System";
     }

     virtual void tick ( const double &duration_ms );
     
private:
      void store_inventory ( debug_name_component * name, position_component3d * pos );
      void register_inventory_stored( item_storage_component * store );
      void register_inventory_carried ( item_carried_component * carried );
      void register_inventory_ground ( position_component3d * pos );
};
