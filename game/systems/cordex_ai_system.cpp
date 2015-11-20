#include "cordex_ai_system.h"
#include <iostream>
#include "../game.h"
#include "../raws/raws.h"

void cordex_ai_system::tick ( const double& duration_ms )
{
    // Handle new build orders
    vector<build_order_message> * orders = game_engine->messaging->get_messages_by_type< build_order_message >();
    for (build_order_message &msg : *orders) {
	std::cout << "Logged an order to build [" << msg.structure_name << "], at {" << msg.x << "," << msg.y << "}\n";
	msg.deleted = true; // No repeats
	
	// Create a place-holder entity for the new structure (just a renderable and a position, obtain the entity ID)
	const int entity_id = raws::create_placeholder_structure_from_raws( msg.structure_name, msg.x, msg.y );
	std::cout << "Constructed as entity # " << entity_id << "\n";
	
	// Create a job
	// Create the sub-tasks
	// Add it to the help-wanted board
    }
}
