#include "renderable_system.h"
#include <iostream>
#include <tuple>
#include "../game.h"

void renderable_system::tick ( const double &duration_ms ) {
    bool changes = false;
     vector<entity_moved_message> * change_list = game_engine->messaging->get_messages_by_type<entity_moved_message>();
     for ( entity_moved_message &msg : *change_list ) {
          changes = true;
          msg.deleted = true;
     }
  
    if (changes) {
	// Clear the visible entity system
	std::fill( world::entity_render_list.begin(), world::entity_render_list.end(), world::render_layers_t() );

	// Obtain a list of things that can be rendered
	const vector<renderable_component> * renderables = game_engine->ecs->find_components_by_type<renderable_component> ();
	for ( const renderable_component &render_info : *renderables ) {

	    position_component * pos = game_engine->ecs->find_entity_component<position_component>(render_info.entity_id);

	    if ( pos != nullptr) {
		const int idx = world::current_region->idx ( pos->x, pos->y );
		//world::entity_render_list[idx] = std::make_tuple(render_info.tile_idx, render_info.glyph, render_info.foreground, render_info.background);
		world::render_info_t target{ render_info.tile_idx, render_info.glyph, render_info.foreground, render_info.background, render_info.translucent, render_info.composite, render_info.entity_id, render_info.extra_tall };
		switch (render_info.layer) {
		  case 0 : world::entity_render_list[idx].floor = target; break;
		  case 1 : world::entity_render_list[idx].building = target; break;
		  case 2 : world::entity_render_list[idx].top = target; break;
		}
	    }
	}
    }
}
