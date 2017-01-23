#include "threat_scanner.hpp"

using namespace rltk;

namespace tasks {

spotted_hostile_t can_see_hostile(const rltk::entity_t &e, const position_t &pos, const viewshed_t &view, const std::function<bool(rltk::entity_t&)> &test) {
    spotted_hostile_t result;

    for (const std::size_t other_entity : view.visible_entities) {
        const auto other = entity(other_entity);
        if (!other) {
            // We can't access the other entity - so skip it
            break;
        }

        if (other && test(*other)) {
            result.terrified = true;
            const auto other_pos = other->component<position_t>();
            if (other_pos) {
                const float d = distance3d_squared(pos.x, pos.y, pos.z, other_pos->x, other_pos->y, other_pos->z);
                if (d < result.terror_distance) {
                    result.terror_distance = d;
                    result.closest_fear = other_entity;
                }
            }
        }
    }

    return result;
}

}