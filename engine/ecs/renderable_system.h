#pragma once

#include "base_system.h"
#include "../../game/world/world.h"
#include "ecs.h"
#include "position_component.h"
#include "renderable_component.h"

namespace engine {
namespace ecs {

class renderable_system : public base_system {
    virtual void tick(const double &duration_ms) {
        // Clear the visible entity system
        world::entity_render_list.clear();

        // Obtain a list of things that can be rendered
        vector<base_component *> renderables = find_components_by_type(renderable);
        for (const base_component * renderable : renderables) {

            const renderable_component * render_info = static_cast<const renderable_component *>(renderable);
            const entity * parent = get_entity_by_handle(renderable->entity_id);

            for (const std::pair<component_type, int> &entity_handle : parent->component_handles) {
                if (entity_handle.first == position) {
                    position_component * pos = static_cast<position_component *>(get_component_by_handle(entity_handle.second));
                    const int idx = world::current_region->idx(pos->x, pos->y);
                    world::entity_render_list[idx] = {render_info->glyph, render_info->foreground, render_info->background};
                    break;
                }
            }
        }
    }
};

}
}