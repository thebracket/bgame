#pragma once

#include <vector>

using std::vector;

namespace engine {
namespace ecs {

enum component_type{position,name,renderable};

class base_component {
public:
    int entity_id;
    int handle;
    component_type type;
};

}
}
