#pragma once

#include "component_types.h"

namespace engine {
namespace ecs {

struct obstruction_component {
    int entity_id;
    int handle;
    component_type type = obstruction;
    
    bool blocks_entry;
    bool blocks_visibility;
};

}
}