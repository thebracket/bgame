#include "damage_system.h"
#include "../game.h"

void damage_system::tick ( const double& duration_ms )
{
    vector<inflict_damage_message> * damages = game_engine->messaging->get_messages_by_type<inflict_damage_message>();
    for (inflict_damage_message &damage : *damages) {
	if (!damage.deleted) {
	    game_health_component * health = game_engine->ecs->find_entity_component<game_health_component>( damage.target_entity );
	    if ( health != nullptr ) {
		health->current_hit_points -= damage.damage_taken;
		std::cout << "Target takes " << damage.damage_taken << " hit points of damage. Cause: " << damage.damage_source << "\n";
		if (health->current_hit_points < 0) std::cout << "If death were implemented, that would have been fatal!\n";
	    } else {
		std::cout << "Warning: Attempting to inflict damage on something with no hitpoints!\n";
	    }
	    
	    // TODO: Blood splatters
	}
	damage.deleted = true;	
    }
}
