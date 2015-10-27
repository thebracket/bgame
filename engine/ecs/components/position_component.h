#pragma once

#include "component_types.h"

namespace engine {
namespace ecs {

struct position_component {
    int entity_id;
    int handle;
    component_type type = position;
    
    position_component() {}
    position_component(const int &nx, const int &ny) : x(nx), y(ny) {}
    
    int x;
    int y;
    bool moved;
};

}
}
