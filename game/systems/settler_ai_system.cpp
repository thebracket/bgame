#include "settler_ai_system.h"
#include "../game.h"
#include "../world/landblock.h"
#include "../world/tables.h"
#include "../messages/power_consumed_message.h"
#include "../messages/chat_emote_message.h"
#include "../tasks/help_wanted.h"
#include "../tasks/path_finding.h"
#include "../tasks/find_nearest_provision.h"
#include "../raws/raws.h"
#include "../world/skill_test.h"
#include <map>

namespace settler_ai_detail {

void consume_power ( const int &quantity )
{
     game_engine->messaging->add_message<power_consumed_message> ( power_consumed_message ( quantity ) );
}

void emote ( const string &message, const position_component * pos, const chat_emote_color_t &color )
{
     game_engine->messaging->add_message<chat_emote_message> ( chat_emote_message ( string ( " " ) +message, pos->x+1, pos->y, color ) );
}

void append_name ( stringstream &ss, const settler_ai_component &settler )
{
     ss << "@CYAN@" << settler.first_name << " " << settler.last_name << "@WHITE@ (" << settler.profession_tag << ") ";
}

string announce ( const string &state, const settler_ai_component &settler )
{
     stringstream announce;
     append_name ( announce, settler );
     announce << state;
     return announce.str();
}

struct settler_needs {
     bool needs_food;
     bool needs_drink;
     bool needs_sleep;
};

settler_needs needs_clocks ( settler_ai_component &settler )
{
     if ( settler.state_major != SLEEPING ) {
          settler.calories -= settler.calorie_burn_at_rest;
     }
     if ( settler.state_major != SLEEPING ) {
          settler.thirst--;
     }
     if ( settler.state_major != SLEEPING ) {
          settler.wakefulness-=3;
     }

     settler_needs result {false,false,false};

     if ( settler.calories < 300 ) {
          result.needs_food = true;
     }
     if ( settler.thirst < 10 ) {
          result.needs_drink = true;
     }
     if ( settler.wakefulness < 10 ) {
          result.needs_sleep = true;
     }
     return result;
}

bool is_move_possible ( const position_component * pos, const int &delta_x, const int &delta_y )
{
     const int nx = pos->x + delta_x;
     const int ny = pos->y + delta_y;
     const int idx = world::current_region->idx ( nx,ny );

     if ( nx < 0 ) {
          return false;
     }
     if ( nx > landblock_width-1 ) {
          return false;
     }
     if ( ny < 0 ) {
          return false;
     }
     if ( ny > landblock_height-1 ) {
          return false;
     }
     if ( world::walk_blocked[idx] == true ) {
          return false;
     }
     if ( world::current_region->tiles[idx].base_tile_type == tile_type::WATER ) {
          return false;
     }
     return true;
}

inline void move_to ( position_component * pos, const int &nx, const int &ny )
{
     facing_t new_facing = OMNI;
     if ( pos->x < nx ) {
          new_facing = EAST;
     }
     if ( pos->x > nx ) {
          new_facing = WEST;
     }
     if ( pos->y < ny ) {
          new_facing = SOUTH;
     }
     if ( pos->y > ny ) {
          new_facing = NORTH;
     }

     pos->moved=true;
     pos->x = nx;
     pos->y = ny;
     pos->facing = new_facing;
     game_engine->messaging->add_message<entity_moved_message>(entity_moved_message());
}

void wander_randomly ( settler_ai_component &settler, position_component * pos )
{
     // For now, they will wander around aimlessly with no purpose or collision-detection.
     int x = pos->x;
     int y = pos->y;

     int direction = game_engine->rng.roll_dice ( 1,5 );
     switch ( direction ) {
     case 1 :
          if ( is_move_possible ( pos, -1, 0 ) ) {
               move_to ( pos, x-1, y );
          }
          break;
     case 2 :
          if ( is_move_possible ( pos, 1, 0 ) ) {
               move_to ( pos, x+1, y );
          }
          break;
     case 3 :
          if ( is_move_possible ( pos, 0, -1 ) ) {
               move_to ( pos, x, y-1 );
          }
          break;
     case 4 :
          if ( is_move_possible ( pos, 0, 1 ) ) {
               move_to ( pos, x, y+1 );
          }
          break;
     }
}

int create_needs_fulfillment_job ( const int &need, settler_ai_component * settler, position_component * pos )
{
     int chosen_source_id = ai::find_nearest_provision ( need, std::make_pair ( pos->x, pos->y ) );
     if (chosen_source_id == -1) {
        // TODO: Physical effects of missing these needs
	switch (need) {
	  case 1 : { emote( "THERE IS NO FOOD!", pos, RED ); settler->calories += 500; } break;
	  case 2 : { emote( "THERE IS NO CLEAN WATER!", pos, RED ); settler->thirst += 100; } break;
	  case 3 : { emote( "THERE IS NOWHERE TO SLEEP!", pos, RED ); settler->wakefulness += 100; } break;
	}
	settler->state_major = IDLE;
	return -1;
     }
     
     if (need == 3) {
	// Claim the tent!
       provisions_component * bed = game_engine->ecs->find_entity_component<provisions_component>( chosen_source_id );
       bed->provides_quantity = 1;
     }
     
     ai::job_t job;
     job.assigned_to = settler->entity_id;
     job.current_step = 0;
     job.job_id = ai::get_next_job_id();
     job.type = ai::MEET_PHYSICAL_NEED;

     position_component * source_pos = game_engine->ecs->find_entity_component<position_component> ( chosen_source_id );
     job.steps.push_back ( ai::job_step_t { ai::MOVE_TO, source_pos->x, source_pos->y, chosen_source_id, false, "", 0 } );
     job.steps.push_back ( ai::job_step_t { ai::CONSUME_NEED, source_pos->x, source_pos->y, chosen_source_id, false, "", need } );
     ai::jobs_board [ job.job_id ] = job;
     return job.job_id;
}

void idle ( settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos )
{
     bool claimed_job = false;
     for ( auto it = ai::jobs_board.begin(); it != ai::jobs_board.end(); ++it ) {
          if ( it->second.assigned_to == 0 and !claimed_job ) {
               settler.state_major = JOB;
               it->second.assigned_to = settler.entity_id;
               settler.job_id = it->second.job_id;
               claimed_job = true;
	       settler.state_timer = 0;
          }
     }

     if ( settler.state_major == IDLE ) {
          renderable->glyph = '@';
          renderable->foreground = color_t {255,255,0};
          settler_ai_detail::wander_randomly ( settler, pos );
     }
}

void do_your_job ( settler_ai_component &settler, game_stats_component * stats, renderable_component * renderable, position_component * pos )
{
     auto job = ai::jobs_board.find ( settler.job_id );
     if ( job == ai::jobs_board.end() ) {
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
     switch ( step.type ) {
     case ai::MOVE_TO : {
          // Are we there yet?
          const int distance = std::sqrt ( ( std::abs ( pos->x - step.target_x ) *std::abs ( pos->x - step.target_x ) ) + ( std::abs ( pos->y - step.target_y ) * ( std::abs ( pos->y - step.target_y ) ) ) );
          if ( distance <= 1 ) {
               // We've reached our destination
               ++job->second.current_step;
               settler.current_path.reset();
               return;
          }

          // No - so we better consult the map
          if ( settler.current_path == nullptr ) {
               // If there is no path - request one to the destination and exit (no moving and pathing!)
               settler.current_path = ai::find_path ( std::make_pair ( pos->x, pos->y ), std::make_pair ( step.target_x, step.target_y ), world::walk_blocked );
               return;
          } else {
               // There is a path...
               if ( settler.current_path->destination.first != step.target_x or settler.current_path->destination.second != step.target_y ) {
                    // Does it go to the right place? If not, then make a new one and exit
                    settler.current_path.reset();
                    return;
               }
               // ... and it goes to the right place!
               if ( settler.current_path->steps.empty() ) {
                    settler.current_path.reset();
                    return;
               }
               std::pair<int,int> next_step = settler.current_path->steps.front();
               settler.current_path->steps.pop();
               const int delta_x = next_step.first - pos->x;
               const int delta_y = next_step.second - pos->y;
               if ( is_move_possible ( pos, delta_x, delta_y ) ) {
                    move_to ( pos, next_step.first, next_step.second );
                    return;
               } else {
                    // Reset the path - something went wrong
                    settler.current_path.reset();
                    return;
               }
          }
     }
     break;
     case ai::PICK_UP_COMPONENT : {
          // We've reached the component, so we pick it up
          engine::entity * item = game_engine->ecs->get_entity_by_handle ( step.component_id );
          item_storage_component * storage = game_engine->ecs->find_entity_component<item_storage_component> ( step.component_id );
	  if (storage != nullptr) {
	      storage->deleted = true; // It's not stored anymore, so delete the component
	  }
	  position_component * item_pos = game_engine->ecs->find_entity_component<position_component>( step.component_id );
	  if (item_pos != nullptr) {
	      item_pos->deleted = true;
	  }
          game_engine->ecs->add_component<item_carried_component> ( *item, item_carried_component ( settler.entity_id, 0 ) );
          game_engine->messaging->add_message<item_changed_message> ( item_changed_message ( item->handle ) );
	  game_engine->messaging->add_message<entity_moved_message>(entity_moved_message());
          ++job->second.current_step;
     }
     break;
     case ai::DROP_OFF_COMPONENT : {
          engine::entity * item = game_engine->ecs->get_entity_by_handle ( step.component_id );
          item_carried_component * carried = game_engine->ecs->find_entity_component<item_carried_component> ( step.component_id );
          carried->deleted = true; // It's not carried anymore, so delete the component
          game_engine->ecs->add_component<position_component> ( *item, position_component ( step.target_x, step.target_y ) );
          game_engine->messaging->add_message<item_changed_message> ( item_changed_message ( item->handle ) );
	  game_engine->messaging->add_message<entity_moved_message>(entity_moved_message());
          ++job->second.current_step;
     }
     case ai::DROP_OFF_TOOL : {
          engine::entity * item = game_engine->ecs->get_entity_by_handle ( step.component_id );
          item_carried_component * carried = game_engine->ecs->find_entity_component<item_carried_component> ( step.component_id );
          carried->deleted = true; // It's not carried anymore, so delete the component
          game_engine->ecs->add_component<position_component> ( *item, position_component ( step.target_x, step.target_y ) );
	  item_component * item_c = game_engine->ecs->find_entity_component<item_component>( step.component_id );
	  item_c->claimed = false;
          game_engine->messaging->add_message<item_changed_message> ( item_changed_message ( item->handle ) );
	  game_engine->messaging->add_message<entity_moved_message>(entity_moved_message());
          ++job->second.current_step;
     }
     break;
     case ai::CONSTRUCT_WITH_SKILL : {
	  ++settler.state_timer;
	  if ( settler.state_timer > step.required_skill_difficulty ) {
	      settler.state_timer = 0;
	      auto result = game_system::skill_roll( settler.entity_id, step.skill_name, step.required_skill_difficulty );
	      if ( result >= game_system::SUCCESS ) {
		  ++job->second.current_step;
		  emote( "Work Complete!", pos, chat_emote_color_t::BLUE);
	      } else {
		  if (result == game_system::CRITICAL_FAIL) {
		       emote( "OUCH!", pos, chat_emote_color_t::RED );
		       // TODO: Inflict damage on the hapless settler!
		  } else {
		      int random = game_engine->rng.roll_dice(1, 6);
		      switch (random) {
			case 1 : emote( "*working*", pos, chat_emote_color_t::BLUE); break;
			case 2 : emote( "This is harder than it looks!", pos, chat_emote_color_t::BLUE); break;
			case 3 : emote( "That didn't work, I'll try again.", pos, chat_emote_color_t::BLUE); break;
			case 4 : emote( "Do I really know what I'm doing?", pos, chat_emote_color_t::BLUE); break;
			case 5 : emote( "That wasn't what I expected to happen!", pos, chat_emote_color_t::BLUE); break;
			case 6 : emote( "Oh dear, trying again", pos, chat_emote_color_t::BLUE); break;
		      }
		  }
	      }
	  }
     }
     break;
     case ai::CONVERT_PLACEHOLDER_STRUCTURE : {
          debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component> ( step.placeholder_structure_id );
          game_engine->ecs->delete_entity ( step.placeholder_structure_id );
          raws::create_structure_from_raws ( name->debug_name, step.target_x, step.target_y );
	  game_engine->messaging->add_message<walkability_changed_message> ( walkability_changed_message ( ) );
	  game_engine->messaging->add_message<entity_moved_message>(entity_moved_message());
          ++job->second.current_step;
     }
     break;
     case ai::DESTROY_COMPONENT : {
          game_engine->messaging->add_message<item_changed_message> ( item_changed_message ( step.component_id ) );
	  game_engine->messaging->add_message<entity_moved_message>(entity_moved_message());
          game_engine->ecs->delete_entity ( step.component_id );
          ++job->second.current_step;
     }
     break;
     case ai::CONSUME_NEED : {
          if ( step.placeholder_structure_id == 3 ) {
               // Sleep mode
	       pos->x = step.target_x;
	       pos->y = step.target_y;
               const int wakeful_gain = stat_modifier ( stats->constitution ) + 8 + game_engine->rng.roll_dice ( 1,6 );
               settler.wakefulness += wakeful_gain;
               if ( settler.wakefulness > 1000 ) {
                    emote ( "YAWN!", pos, YELLOW );
		    // We need to free the bed
		    provisions_component * bed = game_engine->ecs->find_entity_component<provisions_component>( step.component_id );
		    bed->provides_quantity = 0;
                    ++job->second.current_step;
               }
          } else {
               provisions_component * provider = game_engine->ecs->find_entity_component<provisions_component> ( step.component_id );
               consume_power ( provider->power_drain );
               emote ( provider->action_name, pos, CYAN );
               if ( step.placeholder_structure_id == 2 ) {
                    settler.thirst += provider->provides_quantity;
               } else {
                    settler.calories += provider->provides_quantity;
               }
               ++job->second.current_step;
          }
     }
     break;
     case ai::CREATE_WOOD : {
	  const int number_of_logs = game_engine->rng.roll_dice(1,6);
	  for (int i=0; i<number_of_logs; ++i) {
	      int wood_id = raws::create_item_from_raws("Wood Logs");
	      game_engine->ecs->add_component<position_component>( *game_engine->ecs->get_entity_by_handle( wood_id ), position_component( step.target_x, step.target_y ) );
	  }
	  ++job->second.current_step;
     } break;
     }
}

}

void settler_ai_system::tick ( const double &duration_ms )
{
     if ( world::paused ) {
          return;
     }

     // Obtain a link to the calendar
     calendar_component * calendar = game_engine->ecs->find_entity_component<calendar_component> ( world::cordex_handle );

     vector<settler_ai_component> * settlers = game_engine->ecs->find_components_by_type<settler_ai_component>();
     for ( settler_ai_component &settler : *settlers ) {
          if ( settler.next_tick > calendar->system_time ) {
               break;
          }

          settler_ai_detail::settler_needs needs = settler_ai_detail::needs_clocks ( settler );
          position_component * pos = game_engine->ecs->find_entity_component<position_component> ( settler.entity_id );

          // Needs will override current action!
          if ( needs.needs_sleep and settler.state_major != JOB ) {
               settler.state_major = JOB;
               settler.job_id = settler_ai_detail::create_needs_fulfillment_job ( 3, &settler, pos );
          } else if ( needs.needs_drink and settler.state_major != JOB ) {
               settler.state_major = JOB;
               settler.job_id = settler_ai_detail::create_needs_fulfillment_job ( 2, &settler, pos );
          } else if ( needs.needs_food  and settler.state_major != JOB ) {
               settler.state_major = JOB;
               settler.job_id = settler_ai_detail::create_needs_fulfillment_job ( 1, &settler, pos );
          }

          // Perform actions
          renderable_component * renderable = game_engine->ecs->find_entity_component<renderable_component> ( settler.entity_id );
          game_stats_component * stats = game_engine->ecs->find_entity_component<game_stats_component> ( settler.entity_id );

          if ( settler.next_tick <= calendar->system_time ) {
               // Time for the settler to do something!
               if ( settler.state_major == IDLE ) {
                    settler_ai_detail::idle ( settler, stats, renderable, pos );
               } else if ( settler.state_major == JOB ) {
                    settler_ai_detail::do_your_job ( settler, stats, renderable, pos );
               }

               // Random pause
               int initiative_roll = 10 - ( game_engine->rng.roll_dice( 1, 6 ) + stat_modifier( stats->dexterity ) );
	       if (initiative_roll < 1) initiative_roll = 1;
	       if (initiative_roll > 10) initiative_roll = 10;
               settler.next_tick = calendar->system_time + initiative_roll;
          }
     }
}
