#include "entity.h"

namespace engine {
namespace ecs {

int entity::find_component_by_type(const component_type &type) const {
    int flag = component_flag(type);
    if (!component_types.test(flag)) return -1;
    for (const auto &c : component_handles) {
        if (c.first == type) return c.second;
    }
    return -1;
}

}
}