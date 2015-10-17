#pragma once

#include <vector>

using std::vector;

namespace engine {
namespace ecs {

struct base_component {
    int entity_id;
    int handle;
};

}
}
