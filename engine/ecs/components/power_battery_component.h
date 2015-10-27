#pragma once

#include "component_types.h"

namespace engine {
namespace ecs {

struct power_battery_component {
    int entity_id;
    int handle;
    component_type type = power_battery;
    
    power_battery_component() {}
    power_battery_component(const int &n) : storage_capacity(n) {}
  
    int storage_capacity;
};

}
}