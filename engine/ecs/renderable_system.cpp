#include "renderable_system.h"

namespace engine {
namespace ecs {

void renderable_system::tick ( const double &duration_ms ) {
    // Clear the visible entity system
    world::entity_render_list.clear();

    // Obtain a list of things that can be rendered
    const vector<renderable_component *> renderables = find_components_by_type<renderable_component> ( renderable );
    for ( const renderable_component * render_info : renderables ) {

        const entity * parent = get_entity_by_handle ( render_info->entity_id );

        const int entity_handle = parent->find_component_by_type ( ecs::position );
        position_component * pos = get_component_by_handle<position_component> ( entity_handle );

        if ( !render_info->multi_tile ) {
            const int idx = world::current_region->idx ( pos->x, pos->y );
            world::entity_render_list[idx] = {render_info->glyph, render_info->foreground, render_info->background};
        } else {
            int i=0;
            int y = pos->y - ( render_info->height/2 );
            for ( int Y=0; Y<render_info->height; ++Y ) {
                int x = pos->x - ( render_info->width/2 );
                for ( int X=0; X<render_info->width; ++X ) {
                    const int idx = world::current_region->idx ( x,y );
                    world::entity_render_list[idx] = {render_info->glyphs[i], render_info->foreground, render_info->background};
                    ++x;
                    ++i;
                }
                ++y;
            }
        }
    }
}

}
}