#pragma once

#include "component_types.h"
#include "../../raws/raw_power_generator_condition.h"

namespace engine {
namespace ecs {

struct power_generator_component {
    int entity_id;
    int handle;
    component_type type = power_generator;
    
    power_generator_component() {}
    power_generator_component(const int &n, const engine::raws::power_generator_condition &c) : amount(n), generator_mode(c) {}
    
    int amount;
    engine::raws::power_generator_condition generator_mode;
};

}
}