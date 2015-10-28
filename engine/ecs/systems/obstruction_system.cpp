#include "obstruction_system.h"
#include "../components/obstruction_component.h"
#include "../components/position_component.h"
#include "../../../game/world/world.h"
#include "../ecs.h"
#include "../../globals.h"

void engine::ecs::obstruction_system::tick(const double& duration_ms)
{
    world::walk_blocked.clear();

    const vector<obstruction_component> * blockages = engine::globals::ecs->find_components_by_type<obstruction_component>();
    for (const obstruction_component &block : *blockages) {
      
        const entity * parent = engine::globals::ecs->get_entity_by_handle ( block.entity_id );
        const int entity_handle = parent->find_component_by_type ( ecs::position );
        position_component * pos = engine::globals::ecs->get_component_by_handle<position_component> ( entity_handle );
        if (pos != nullptr) {
            const int idx = world::current_region->idx ( pos->x, pos->y );
            world::walk_blocked[idx] = true;
        }
    }
}
