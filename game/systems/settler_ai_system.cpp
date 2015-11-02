#include "settler_ai_system.h"
#include "../../engine/globals.h"
#include "../world/landblock.h"
#include "../world/tables.h"
#include "flowmap_system.h"
#include "power_system.h"
#include <map>

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
	if (settler.state_major != SLEEPING) settler.calories -= settler.calorie_burn_at_rest;
	if (settler.state_major != SLEEPING) settler.thirst--;
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

bool is_move_possible(const position_component * pos, const int &delta_x, const int &delta_y) {
    const int nx = pos->x + delta_x;
    const int ny = pos->y + delta_y;    
    const int idx = world::current_region->idx(nx,ny);
    
    if (nx < 0) return false;
    if (nx > landblock_width-1) return false;
    if (ny < 0) return false;
    if (ny > landblock_height-1) return false;
    if (world::walk_blocked.find(idx) != world::walk_blocked.end()) return false;
    if (world::current_region->tiles[idx].base_tile_type == tile_type::WATER) return false;
    return true;
}

inline void move_to(position_component * pos, const int &nx, const int &ny) {
    pos->moved=true;
    pos->x = nx;
    pos->y = ny;
}

void wander_randomly(settler_ai_component &settler, position_component * pos) {    
    // For now, they will wander around aimlessly with no purpose or collision-detection.
    int x = pos->x;
    int y = pos->y;

    int direction = engine::roll_dice(1,5);
    
    switch (direction) {
      case 1 : if (is_move_possible(pos, -1, 0)) move_to(pos, x-1, y); break;
      case 2 : if (is_move_possible(pos, 1, 0)) move_to(pos, x+1, y); break;
      case 3 : if (is_move_possible(pos, 0, -1)) move_to(pos, x, y-1); break;
      case 4 : if (is_move_possible(pos, 0, 1)) move_to(pos, x, y+1); break;
    }
}

void sleepy_time(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable) {
    // No movement
    if (engine::roll_dice(1,6)<4) {
	renderable->glyph = 'Z';
	renderable->foreground = color_t{128,128,255};
    } else {
	renderable->glyph = '@';
	renderable->foreground = color_t{128,128,255};
    }
    const int wakeful_gain = stat_modifier(stats->constitution) + 2 + engine::roll_dice(1,6);
    settler.wakefulness += wakeful_gain;
    --settler.state_timer;
    //std::cout << settler.first_name << " sleep time: " << settler.state_timer << ", wakefulness: " << settler.wakefulness << "\n";
    if (settler.state_timer < 1 or settler.wakefulness > 2000) {
	  world::log.write(settler_ai_detail::announce("wakes up with a yawn.", settler));
	  settler.state_major = IDLE;
	  viewshed_component * vision = engine::globals::ecs->find_entity_component<viewshed_component>(settler.entity_id);
	  vision->scanner_range = 12;
	  vision->last_visibility.clear();
    }
}

void follow_flow_map(position_component * pos, const vector<short> &flow_map) {
    // Make a sorted list of options
    // Check each option for viability & move that way if it is good
    std::map<short,char> candidates;
    if (settler_ai_detail::is_move_possible(pos, -1, 0)) {
	const int idx = world::current_region->idx( pos->x-1, pos->y );
	candidates[flow_map[idx]] = 1;
    }
    if (settler_ai_detail::is_move_possible(pos, 1, 0)) {
	const int idx = world::current_region->idx( pos->x+1, pos->y );
	candidates[flow_map[idx]] = 2;
    }
    if (settler_ai_detail::is_move_possible(pos, 0, -1)) {
	const int idx = world::current_region->idx( pos->x, pos->y-1 );
	candidates[flow_map[idx]] = 3;
    }
    if (settler_ai_detail::is_move_possible(pos, 0, 1)) {
	const int idx = world::current_region->idx( pos->x, pos->y+1 );
	candidates[flow_map[idx]] = 4;
    }
    
    //std::cout << "Potential drink candidates: " << candidates.size() << "\n";
    
    if (!candidates.empty()) {
	auto it = candidates.begin();
	char direction = it->second;
	switch (direction) {
	  case 1 : settler_ai_detail::move_to(pos, pos->x-1, pos->y); break;
	  case 2 : settler_ai_detail::move_to(pos, pos->x+1, pos->y); break;
	  case 3 : settler_ai_detail::move_to(pos, pos->x, pos->y-1); break;
	  case 4 : settler_ai_detail::move_to(pos, pos->x, pos->y+1); break;
	}
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
	position_component * pos = engine::globals::ecs->find_entity_component<position_component>(settler.entity_id);
	
	// Needs will override current action!
	if (needs.needs_sleep and settler.state_major != SLEEPING and settler.state_major != DRINKING and settler.state_major != EATING ) {
	    // TODO: Look for a bed, when we support such things!
	    settler.state_major = SLEEPING;
	    settler.state_timer = 360;
	    world::log.write(settler_ai_detail::announce("falls asleep.", settler));
	    viewshed_component * vision = engine::globals::ecs->find_entity_component<viewshed_component>(settler.entity_id);
	    vision->scanner_range = 2;
	    vision->last_visibility.clear();
	} else if (needs.needs_drink and settler.state_major != SLEEPING and settler.state_major != DRINKING and settler.state_major != EATING ) {
	    settler.state_major = DRINKING;
	    world::log.write(settler_ai_detail::announce("wants a drink.", settler));
	} else if (needs.needs_food and settler.state_major != SLEEPING and settler.state_major != DRINKING and settler.state_major != EATING ) {
	    settler.state_major = EATING;
	    world::log.write(settler_ai_detail::announce("wants some food.", settler));
	}
	
	// Perform actions
	renderable_component * renderable = engine::globals::ecs->find_entity_component<renderable_component>(settler.entity_id);
	game_stats_component * stats = engine::globals::ecs->find_entity_component<game_stats_component>(settler.entity_id); 

	if (settler.next_tick <= calendar->system_time) {
            // Time for the settler to do something!
	    if (settler.state_major == IDLE) {
		renderable->glyph = '@';
		renderable->foreground = color_t{255,255,0};
		settler_ai_detail::wander_randomly(settler, pos);		
	    } else if (settler.state_major == SLEEPING) {
		settler_ai_detail::sleepy_time(settler, stats, renderable);
	    } else if (settler.state_major == DRINKING) {
		const int idx = world::current_region->idx( pos->x, pos->y );
		const short distance_to_drink = flowmaps::water_flow_map [ idx ];
		//std::cout << "Distance to drink from [" << pos->x << "," << pos->y << "]: " << distance_to_drink << "\n";
		renderable->foreground = color_t{0,255,255};
		if (distance_to_drink < 2) {
		    settler.thirst = 1000;
		    settler.state_major = IDLE;
		    world::log.write(settler_ai_detail::announce("enjoys a drink.", settler));
		    power_system_detail::power_drain_this_round += 10;
		} else {
		    settler_ai_detail::follow_flow_map(pos, flowmaps::water_flow_map);
		}
	    } else if (settler.state_major == EATING) {
		const int idx = world::current_region->idx( pos->x, pos->y );
		const short distance_to_food = flowmaps::food_flow_map [ idx ];
		renderable->foreground = color_t{255,0,255};
		//std::cout << "Distance to drink from [" << pos->x << "," << pos->y << "]: " << distance_to_drink << "\n";
		if (distance_to_food < 2) {
		    settler.calories += 2000;
		    settler.state_major = IDLE;
		    world::log.write(settler_ai_detail::announce("enjoys some food.", settler));
		    power_system_detail::power_drain_this_round += 10;
		} else {
		    settler_ai_detail::follow_flow_map(pos, flowmaps::food_flow_map);
		}
	    }
	    
            // Random pause
            settler.next_tick = calendar->system_time + engine::roll_dice(1,6) - stat_modifier(stats->dexterity);
        }
    }
}
