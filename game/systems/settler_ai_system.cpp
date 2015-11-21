#include "settler_ai_system.h"
#include "../game.h"
#include "../world/landblock.h"
#include "../world/tables.h"
#include "flowmap_system.h"
#include "../messages/power_consumed_message.h"
#include "../messages/chat_emote_message.h"
#include "../tasks/help_wanted.h"
#include "../raws/raws.h"
#include <map>

namespace settler_ai_detail {

void consume_power(const int &quantity) {
    game_engine->messaging->add_message<power_consumed_message>(power_consumed_message(quantity));
}

void emote(const string &message, const position_component * pos, const chat_emote_color_t &color) {
    game_engine->messaging->add_message<chat_emote_message>(chat_emote_message(string(" ")+message, pos->x+1, pos->y, color));
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
	if (settler.state_major != SLEEPING) settler.wakefulness-=3;
	
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
    
    int direction = game_engine->rng.roll_dice(1,5);
    switch (direction) {
      case 1 : if (is_move_possible(pos, -1, 0)) move_to(pos, x-1, y); break;
      case 2 : if (is_move_possible(pos, 1, 0)) move_to(pos, x+1, y); break;
      case 3 : if (is_move_possible(pos, 0, -1)) move_to(pos, x, y-1); break;
      case 4 : if (is_move_possible(pos, 0, 1)) move_to(pos, x, y+1); break;
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

void sleepy_time(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos) {
    const int idx = world::current_region->idx ( pos->x, pos->y );
    const short distance_to_bed = flowmaps::sleep_flow_map [ idx ];
  
    if (distance_to_bed < 3 or distance_to_bed > 20000) {
	if (distance_to_bed < 3) {
	    position_component * bed_pos = game_engine->ecs->find_entity_component<position_component>( flowmaps::sleep_flow_map_entity_id[idx] );
	    pos->x = bed_pos->x;
	    pos->y = bed_pos->y;
	    provisions_component * bed = game_engine->ecs->find_entity_component<provisions_component>( bed_pos->entity_id );
	    bed->provides_quantity = 1;
	    settler.job_id = bed->entity_id;
	}      
      
	// Reduced visibility while asleep
	viewshed_component * vision = game_engine->ecs->find_entity_component<viewshed_component>(settler.entity_id);
	vision->scanner_range = 2;
	vision->last_visibility.clear();

	// No movement
	if (game_engine->rng.roll_dice(1,6)<4) {
	    renderable->glyph = 'Z';
	    renderable->foreground = color_t{128,128,255};
	} else {
	    renderable->glyph = '@';
	    renderable->foreground = color_t{128,128,255};
	}
	const int wakeful_gain = stat_modifier(stats->constitution) + 2 + game_engine->rng.roll_dice(1,6);
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
	      if (settler.job_id > 0) {
		  provisions_component * bed = game_engine->ecs->find_entity_component<provisions_component>( settler.job_id );
		  bed->provides_quantity = 0;
	      }
	}
    } else {
	settler_ai_detail::follow_flow_map(pos, flowmaps::sleep_flow_map);
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

void idle(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos) {
    bool claimed_job = false;
    for (auto it = ai::jobs_board.begin(); it != ai::jobs_board.end(); ++it) {
	if (it->second.assigned_to == 0 and !claimed_job) {
	    settler.state_major = JOB;
	    it->second.assigned_to = settler.entity_id;
	    settler.job_id = it->second.job_id;
	    claimed_job = true;
	}
    }
  
    if (settler.state_major == IDLE) {
	renderable->glyph = '@';
	renderable->foreground = color_t{255,255,0};
	settler_ai_detail::wander_randomly(settler, pos);  
    }
}

void do_your_job(settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos) {
    auto job = ai::jobs_board.find( settler.job_id );
    if ( job == ai::jobs_board.end()) {
	settler.state_major = IDLE;
	return;
    }
    if ( job->second.current_step > job->second.steps.size()-1 ) {
	// Job complete
	ai::jobs_board.erase ( settler.job_id );
	settler.job_id = 0;
	settler.state_major = IDLE;
	return;
    }
    
    ai::job_step_t step = job->second.steps[ job->second.current_step ];
    switch (step.type) {
      case ai::MOVE_TO : {
	  // Are we there yet?
	  const int distance = std::sqrt((std::abs(pos->x - step.target_x)*std::abs(pos->x - step.target_x)) + ( std::abs(pos->y - step.target_y)*( std::abs(pos->y - step.target_y))));
	  if ( distance <= 1 ) {
	      // We've reached our destination
	      ++job->second.current_step;
	      settler.current_path.reset();
	      return;
	  }
	
	  // No - so we better consult the map
	  if (settler.current_path == nullptr ) {
	      // If there is no path - request one to the destination and exit (no moving and pathing!)
	      settler.current_path = world::current_region->find_path( std::make_pair( pos->x, pos->y ), std::make_pair( step.target_x, step.target_y ), world::walk_blocked );
	      return;
	  } else {
	      // There is a path...
	      if (settler.current_path->destination.first != step.target_x or settler.current_path->destination.second != step.target_y) {
		  // Does it go to the right place? If not, then make a new one and exit
		  settler.current_path.reset();
		  return;
	      }
	      // ... and it goes to the right place!
	      if (settler.current_path->steps.empty()) {
		  settler.current_path.reset();
		  return;
	      }
	      std::pair<int,int> next_step = settler.current_path->steps.front();
	      settler.current_path->steps.pop();
	      const int delta_x = next_step.first - pos->x;
	      const int delta_y = next_step.second - pos->y;
	      if (is_move_possible( pos, delta_x, delta_y )) {
		  move_to( pos, next_step.first, next_step.second );
		  return;
	      } else {
		  // Reset the path - something went wrong
		  settler.current_path.reset();
		  return;
	      }
	  }
      } break;
      case ai::PICK_UP_COMPONENT : {
	  // We've reached the component, so we pick it up
	  engine::entity * item = game_engine->ecs->get_entity_by_handle( step.component_id );
	  item_storage_component * storage = game_engine->ecs->find_entity_component<item_storage_component>( step.component_id );
	  storage->deleted = true; // It's not stored anymore, so delete the component
	  game_engine->ecs->add_component<item_carried_component>( *item, item_carried_component( settler.entity_id, 0 ) );
	  ++job->second.current_step;
      } break;
      case ai::DROP_OFF_COMPONENT : {
	  engine::entity * item = game_engine->ecs->get_entity_by_handle( step.component_id );
	  item_carried_component * carried = game_engine->ecs->find_entity_component<item_carried_component>( step.component_id );
	  carried->deleted = true; // It's not carried anymore, so delete the component
	  game_engine->ecs->add_component<position_component>( *item, position_component( step.target_x, step.target_y ) );
	  ++job->second.current_step;
      } break;
      case ai::CONSTRUCT_WITH_SKILL : {
	  // TODO: Skills aren't implemented yet, so move on.
	  ++job->second.current_step;
      } break;
      case ai::CONVERT_PLACEHOLDER_STRUCTURE : { 
	  debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component>( step.placeholder_structure_id );
	  game_engine->ecs->delete_entity( step.placeholder_structure_id );
	  raws::create_structure_from_raws( name->debug_name, step.target_x, step.target_y );	  
	  ++job->second.current_step;
      } break;
      case ai::DESTROY_COMPONENT : { 
	  game_engine->ecs->delete_entity( step.component_id );
	  ++job->second.current_step;
      } break;
    }
}

}

void settler_ai_system::tick ( const double &duration_ms ) {
    if (world::paused) return;
    
    // Obtain a link to the calendar
    calendar_component * calendar = game_engine->ecs->find_entity_component<calendar_component> ( world::cordex_handle );

    vector<settler_ai_component> * settlers = game_engine->ecs->find_components_by_type<settler_ai_component>();
    for (settler_ai_component &settler : *settlers) {
	if (settler.next_tick > calendar->system_time) break;
      
	settler_ai_detail::settler_needs needs = settler_ai_detail::needs_clocks(settler);
	position_component * pos = game_engine->ecs->find_entity_component<position_component>(settler.entity_id);
	
	// Needs will override current action!
	if (needs.needs_sleep and settler.state_major != SLEEPING and settler.state_major != DRINKING and settler.state_major != EATING ) {
	    settler.state_major = SLEEPING;
	    settler.state_timer = 360;
	    world::log.write(settler_ai_detail::announce("falls asleep.", settler));
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
		settler_ai_detail::idle(settler, stats, renderable, pos);	
	    } else if (settler.state_major == SLEEPING) {
		settler_ai_detail::sleepy_time(settler, stats, renderable, pos);
	    } else if (settler.state_major == DRINKING) {
		settler_ai_detail::drinking_time(settler, stats, renderable, pos);
	    } else if (settler.state_major == EATING) {
		settler_ai_detail::eating_time(settler, stats, renderable, pos);
	    } else if (settler.state_major == JOB) {
		settler_ai_detail::do_your_job(settler, stats, renderable, pos);
	    }
	    
            // Random pause
            settler.next_tick = calendar->system_time + game_engine->rng.roll_dice(1,6) - stat_modifier(stats->dexterity);
        }
    }
}
