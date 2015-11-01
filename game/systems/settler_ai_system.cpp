#include "settler_ai_system.h"
#include "../../engine/globals.h"
#include "../world/landblock.h"
#include "../world/tables.h"

namespace settler_ai_detail {
  
void append_name(stringstream &ss, const settler_ai_component &settler) {
    ss << "@CYAN@" << settler.first_name << " " << settler.last_name << "@WHITE@ (" << settler.profession_tag << ") ";
}

string announce(const string &state, const settler_ai_component &settler) {
    stringstream announce;
    append_name(announce, settler);
    announce << state;
    return announce.str();
}

struct settler_needs {
    bool needs_food;
    bool needs_drink;
    bool needs_sleep;
};

settler_needs needs_clocks(settler_ai_component &settler) {
	settler.calories -= settler.calorie_burn_at_rest;
	settler.thirst--;
	if (settler.state_major != SLEEPING) settler.wakefulness--;
	
	settler_needs result{false,false,false};
	
	if (settler.calories < 300) {
	    result.needs_food = true;
	}
	if (settler.thirst < 10) {
	    result.needs_drink = true;
	}
	if (settler.wakefulness < 10) {
	    result.needs_sleep = true;
	}
	return result;
}

void wander_randomly(settler_ai_component &settler) {
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
    } else {
	pos->moved = false;
    }
}

void sleepy_time(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable) {
    // No movement
    if (engine::roll_dice(1,6)<4) {
	renderable->glyph = 'Z';
	renderable->foreground = color_t{64,64,64};
    } else {
	renderable->glyph = '@';
	renderable->foreground = color_t{64,64,64};
    }
    const int wakeful_gain = stat_modifier(stats->constitution) + 2 + engine::roll_dice(1,6);
    settler.wakefulness += wakeful_gain;
    --settler.state_timer;
    //std::cout << settler.first_name << " sleep time: " << settler.state_timer << ", wakefulness: " << settler.wakefulness << "\n";
    if (settler.state_timer < 1 or settler.wakefulness > 2000) {
	  world::log.write(settler_ai_detail::announce("wakes up with a yawn.", settler));
	  settler.state_major = IDLE;
    }
}

}

void settler_ai_system::tick ( const double &duration_ms ) {
    if (world::paused) return;
    
    // Obtain a link to the calendar
    calendar_component * calendar = engine::globals::ecs->find_entity_component<calendar_component> ( world::cordex_handle );

    vector<settler_ai_component> * settlers = engine::globals::ecs->find_components_by_type<settler_ai_component>();
    for (settler_ai_component &settler : *settlers) {
	settler_ai_detail::settler_needs needs = settler_ai_detail::needs_clocks(settler);
	
	// Needs will override current action!
	if (needs.needs_sleep and settler.state_major != SLEEPING) {
	    // TODO: Look for a bed, when we support such things!
	    settler.state_major = SLEEPING;
	    settler.state_timer = 360;
	    world::log.write(settler_ai_detail::announce("falls asleep.", settler));
	}
	
	// Perform actions
	renderable_component * renderable = engine::globals::ecs->find_entity_component<renderable_component>(settler.entity_id);
	game_stats_component * stats = engine::globals::ecs->find_entity_component<game_stats_component>(settler.entity_id); 

	if (settler.next_tick <= calendar->system_time) {
            // Time for the settler to do something!
	    if (settler.state_major == IDLE) {
		renderable->glyph = '@';
		renderable->foreground = color_t{255,255,0};
		settler_ai_detail::wander_randomly(settler);		
	    } else if (settler.state_major == SLEEPING) {
		settler_ai_detail::sleepy_time(settler, stats, renderable);
	    }
	    
            // Random pause
            settler.next_tick = calendar->system_time + engine::roll_dice(1,6) - stat_modifier(stats->dexterity);
        }
    }
}
