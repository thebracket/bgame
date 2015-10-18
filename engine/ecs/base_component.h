#pragma once

#include <vector>

using std::vector;

namespace engine {
namespace ecs {

enum component_type{position,name};

struct base_component {
    int entity_id;
    int handle;
    component_type type;
};

}
}
