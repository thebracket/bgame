#include "cordex_ai_system.h"
#include <iostream>
#include <map>
#include "../game.h"
#include "../raws/raws.h"
#include "../tasks/help_wanted.h"

struct selected_component {
  int id;
  int x;
  int y;
};

void cordex_ai_system::tick ( const double& duration_ms )
{
    // Handle new build orders
    vector<build_order_message> * orders = game_engine->messaging->get_messages_by_type< build_order_message >();
    for (build_order_message &msg : *orders) {
	msg.deleted = true; // No repeats
	
	// Create a place-holder entity for the new structure (just a renderable and a position, obtain the entity ID)
	const int entity_id = raws::create_placeholder_structure_from_raws( msg.structure_name, msg.x, msg.y );
	
	// Create a job
	ai::job_t job;
	job.job_id = ai::get_next_job_id();
	job.current_step = 0;
	job.assigned_to = 0;
	job.type = ai::BUILD_STRUCTURE;
	
	// Create the sub-tasks; we need to know what components go into the task
	vector<string> components = raws::get_buildable_requirements( msg.structure_name );
	vector<world::available_item *> chosen_components;
	for (const string &component : components) {
	    auto finder = world::inventory.find( component );
	    if (finder == world::inventory.end()) return; // Can't do it!
	    // Find the closest candidate
	    std::map<int,world::available_item *> distance_components;
	    for (world::available_item item : finder->second) {
		item_storage_component * isc = game_engine->ecs->find_entity_component<item_storage_component>( item.entity_id );
		if (!isc->claimed) {
		    float distance = std::sqrt( (std::abs(msg.x - item.location.first)*std::abs(msg.x - item.location.first)) + (std::abs(msg.y - item.location.second)*std::abs(msg.y - item.location.second)));
		    int distance_i = distance;
		    distance_components[distance_i] = &item;
		}
	    }
	    chosen_components.push_back( distance_components.begin()->second );
	    item_storage_component * isc = game_engine->ecs->find_entity_component<item_storage_component>( distance_components.begin()->second->entity_id );
	    isc->claimed = true;
	}
	
	
	// For each required component
	for (world::available_item * component : chosen_components) {
	    const int component_x = component->location.first;
	    const int component_y = component->location.second;
	    const int component_id = component->entity_id;
	  
	    job.steps.push_back( ai::job_step_t{ ai::MOVE_TO, component_x, component_y, 0, false, "", 0 } );
	    job.steps.push_back( ai::job_step_t{ ai::PICK_UP_COMPONENT, component_x, component_y, component_id, false, "", 0 } );
	    job.steps.push_back( ai::job_step_t{ ai::MOVE_TO, msg.x, msg.y, 0, false, "", 0 } );
	    job.steps.push_back( ai::job_step_t{ ai::DROP_OFF_COMPONENT, msg.x, msg.y, component_id, false, "", 0 } );
	}
	
	// Add the skill check
	job.steps.push_back( ai::job_step_t{ ai::CONSTRUCT_WITH_SKILL, msg.x, msg.y, 0, true, "Construction", 0 } );
	
	// Once the skill has been passed
	//	For each component
	for (world::available_item * component : chosen_components) {
	    const int component_id = component->entity_id;
	    job.steps.push_back( ai::job_step_t{ ai::DESTROY_COMPONENT, msg.x, msg.y, component_id, false, "", 0 } );
	}
	job.steps.push_back( ai::job_step_t{ ai::CONVERT_PLACEHOLDER_STRUCTURE, msg.x, msg.y, 0, false, "", entity_id } );

	
	// Add it to the help-wanted board
	ai::jobs_board[ job.job_id ] = job;
    }
}
