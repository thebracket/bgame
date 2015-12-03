#include "particle_system.h"

#include "../game.h"

void particle_system::tick ( const double& duration_ms )
{
    vector<particle_message> * particles = game_engine->messaging->get_messages_by_type<particle_message>();
    for (particle_message &particle : *particles) {
	particle.offset_x += game_engine->rng.roll_dice(1,4)-2;
	particle.offset_y -= 1;
    }
    
    vector<particle_emitter_component> * emitters = game_engine->ecs->find_components_by_type<particle_emitter_component>();
    for ( const particle_emitter_component &emitter : *emitters ) {
	if ( game_engine->rng.roll_dice( 1, emitter.emits_one_in_x ) == 1 ) {
	      position_component * pos = game_engine->ecs->find_entity_component<position_component>( emitter.entity_id );
	      game_engine->messaging->add_message<particle_message>( particle_message ( emitter.message, pos->x, pos->y, emitter.ttl ) );
	}
    }
}
