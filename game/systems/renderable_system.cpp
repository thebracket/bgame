#include "renderable_system.h"
#include <iostream>
#include "../game.h"

void renderable_system::tick ( const double &duration_ms ) {
    // Clear the visible entity system
    world::entity_render_list.clear();

    // Obtain a list of things that can be rendered
    const vector<renderable_component> * renderables = game_engine->ecs->find_components_by_type<renderable_component> ();
    for ( const renderable_component &render_info : *renderables ) {

        position_component * pos = game_engine->ecs->find_entity_component<position_component>(render_info.entity_id);

	const int idx = world::current_region->idx ( pos->x, pos->y );
	world::entity_render_list[idx] = {render_info.glyph, render_info.foreground, render_info.background};
    }
}
