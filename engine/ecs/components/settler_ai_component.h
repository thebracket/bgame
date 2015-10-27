#pragma once

#include "component_types.h"

namespace engine {
namespace ecs {

struct settler_ai_component {
    int entity_id;
    int handle;
    component_type type = settler_ai; 
    
    int next_tick = 0;    
};

}
}
