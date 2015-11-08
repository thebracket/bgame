#include "settler_ai_system.h"
#include "../game.h"
#include "../world/landblock.h"
#include "../world/tables.h"
#include "flowmap_system.h"
#include "../messages/power_consumed_message.h"
#include "../messages/chat_emote_message.h"
#include <map>

namespace settler_ai_detail {

void consume_power(const int &quantity) {
    game_engine->messaging->add_message<power_consumed_message>(power_consumed_message(quantity));
}

void emote(const string &message, const position_component * pos, const chat_emote_color_t &color) {
    game_engine->messaging->add_message<chat_emote_message>(chat_emote_message(string(" ")+message, pos->x+2, pos->y, color));
}
  
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
    facing_t new_facing = OMNI;
    if (pos->x < nx) new_facing = EAST;
    if (pos->x > nx) new_facing = WEST;
    if (pos->y < ny) new_facing = SOUTH;
    if (pos->y > ny) new_facing = NORTH;
  
    pos->moved=true;
    pos->x = nx;
    pos->y = ny;
    pos->facing = new_facing;
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

void sleepy_time(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos) {
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
	  settler_ai_detail::emote("YAWN", pos, YELLOW);
	  settler.state_major = IDLE;
	  viewshed_component * vision = game_engine->ecs->find_entity_component<viewshed_component>(settler.entity_id);
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

void drinking_time(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos) {
    const int idx = world::current_region->idx( pos->x, pos->y );
    const short distance_to_drink = flowmaps::water_flow_map [ idx ];
    //std::cout << "Distance to drink from [" << pos->x << "," << pos->y << "]: " << distance_to_drink << "\n";
    renderable->foreground = color_t{0,255,255};
    if (distance_to_drink < 2) {
	const int drink_source_id = flowmaps::water_flow_map_entity_id[idx];
	provisions_component * drink_source = game_engine->ecs->find_entity_component<provisions_component>(drink_source_id);
	settler.thirst = drink_source->provides_quantity;
	settler.state_major = IDLE;
	world::log.write(settler_ai_detail::announce(drink_source->action_name, settler));
	settler_ai_detail::emote(drink_source->action_name, pos, CYAN);
	settler_ai_detail::consume_power(drink_source->power_drain);
    } else {
	settler_ai_detail::follow_flow_map(pos, flowmaps::water_flow_map);
    }
}

void eating_time(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos) {
    const int idx = world::current_region->idx( pos->x, pos->y );
    const short distance_to_food = flowmaps::food_flow_map [ idx ];
    renderable->foreground = color_t{255,0,255};
    //std::cout << "Distance to drink from [" << pos->x << "," << pos->y << "]: " << distance_to_drink << "\n";
    if (distance_to_food < 2) {
	const int food_source_id = flowmaps::food_flow_map_entity_id[idx];
	provisions_component * food_source = game_engine->ecs->find_entity_component<provisions_component>( food_source_id );
	settler.calories += food_source->provides_quantity;
	settler.state_major = IDLE;
	world::log.write(settler_ai_detail::announce(food_source->action_name, settler));
	settler_ai_detail::emote(food_source->action_name, pos, MAGENTA);
	settler_ai_detail::consume_power(food_source->power_drain);
    } else {
	settler_ai_detail::follow_flow_map(pos, flowmaps::food_flow_map);
    }
}

}

void settler_ai_system::tick ( const double &duration_ms ) {
    if (world::paused) return;
    
    // Obtain a link to the calendar
    calendar_component * calendar = game_engine->ecs->find_entity_component<calendar_component> ( world::cordex_handle );

    vector<settler_ai_component> * settlers = game_engine->ecs->find_components_by_type<settler_ai_component>();
    for (settler_ai_component &settler : *settlers) {
	settler_ai_detail::settler_needs needs = settler_ai_detail::needs_clocks(settler);
	position_component * pos = game_engine->ecs->find_entity_component<position_component>(settler.entity_id);
	
	// Needs will override current action!
	if (needs.needs_sleep and settler.state_major != SLEEPING and settler.state_major != DRINKING and settler.state_major != EATING ) {
	    // TODO: Look for a bed, when we support such things!
	    settler.state_major = SLEEPING;
	    settler.state_timer = 360;
	    world::log.write(settler_ai_detail::announce("falls asleep.", settler));
	    settler_ai_detail::emote("Zzzz", pos, BLUE);
	    viewshed_component * vision = game_engine->ecs->find_entity_component<viewshed_component>(settler.entity_id);
	    vision->scanner_range = 2;
	    vision->last_visibility.clear();
	} else if (needs.needs_drink and settler.state_major != SLEEPING and settler.state_major != DRINKING and settler.state_major != EATING ) {
	    settler.state_major = DRINKING;
	    world::log.write(settler_ai_detail::announce("wants a drink.", settler));
	    //settler_ai_detail::emote("Thirsty", pos, BLUE);
	} else if (needs.needs_food and settler.state_major != SLEEPING and settler.state_major != DRINKING and settler.state_major != EATING ) {
	    settler.state_major = EATING;
	    world::log.write(settler_ai_detail::announce("wants some food.", settler));
	    //settler_ai_detail::emote("Hungry", pos, BLUE);
	}
	
	// Perform actions
	renderable_component * renderable = game_engine->ecs->find_entity_component<renderable_component>(settler.entity_id);
	game_stats_component * stats = game_engine->ecs->find_entity_component<game_stats_component>(settler.entity_id); 

	if (settler.next_tick <= calendar->system_time) {
            // Time for the settler to do something!
	    if (settler.state_major == IDLE) {
		renderable->glyph = '@';
		renderable->foreground = color_t{255,255,0};
		settler_ai_detail::wander_randomly(settler, pos);		
	    } else if (settler.state_major == SLEEPING) {
		settler_ai_detail::sleepy_time(settler, stats, renderable, pos);
	    } else if (settler.state_major == DRINKING) {
		settler_ai_detail::drinking_time(settler, stats, renderable, pos);
	    } else if (settler.state_major == EATING) {
		settler_ai_detail::eating_time(settler, stats, renderable, pos);
	    }
	    
            // Random pause
            settler.next_tick = calendar->system_time + engine::roll_dice(1,6) - stat_modifier(stats->dexterity);
        }
    }
}
