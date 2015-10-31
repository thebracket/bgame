#include "settler_ai_system.h"
#include "../../engine/globals.h"
#include "../world/landblock.h"

namespace engine {
namespace ecs {

void settler_ai_system::tick ( const double &duration_ms ) {
    if (world::paused) return;
    
    // Obtain a link to the calendar
    calendar_component * calendar = engine::globals::ecs->find_entity_component<calendar_component> ( world::cordex_handle );

    vector<settler_ai_component> * settlers = engine::globals::ecs->find_components_by_type<settler_ai_component>();
    for (settler_ai_component &settler : *settlers) {
        if (settler.next_tick <= calendar->system_time) {
            // Time for the settler to do something!
            position_component * pos = engine::globals::ecs->find_entity_component<position_component>(settler.entity_id);

            // For now, they will wander around aimlessly with no purpose or collision-detection.
            int x = pos->x;
            int y = pos->y;
            bool moved = true;

            int direction = engine::roll_dice(1,5);
            switch (direction) {
            case 1 :
                --x;
                break;
            case 2 :
                ++x;
                break;
            case 3 :
                --y;
                break;
            case 4 :
                ++y;
                break;
            default :
                moved = false;
            }
            if (moved) {
		// Cancel movement if it collides
		const int idx = world::current_region->idx(x,y);
		auto finder = world::walk_blocked.find(idx);
		if (finder != world::walk_blocked.end() or world::current_region->tiles[idx].base_tile_type == tile_type::WATER) {
		    moved = false;
		    x = pos->x;
		    y = pos->y;
		}
	    }
            if (moved) {
                if (x < 1) x = 1;
                if (x > landblock_width-1 ) x = landblock_width-1;
                if (y < 1 ) y = 1;
                if (y > landblock_height-1) y = landblock_height-1;
                pos->x = x;
                pos->y = y;
                pos->moved = true;


		/*stringstream ss;
                const int tile_idx = world::current_region->idx(x,y);
                const string climate = world::current_region->tiles[tile_idx].get_climate();
                const string type = world::current_region->tiles[tile_idx].get_description();
                ss << "Settler enters tile: @CYAN@" << climate << " @GREEN@" << type;
                world::log.write(ss.str());*/
            } else {
                pos->moved = false;
            }

            // Random pause
            settler.next_tick = calendar->system_time + 1;
        }
    }
}

}
}