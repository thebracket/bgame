#pragma once

#include <vector>

using std::vector;

namespace engine {
namespace ecs {

struct entity {
    int handle;
    vector<int> component_handles;
};

}
}
