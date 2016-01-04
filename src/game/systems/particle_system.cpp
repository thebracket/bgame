#include "particle_system.h"

#include "../game.h"

void particle_system::tick ( const double& duration_ms )
{
    vector<particle_message> * particles = game_engine->messaging->get_messages_by_type<particle_message>();
    for (particle_message &particle : *particles) {
	particle.offset_x += game_engine->rng.roll_dice(1,5)-3;
	particle.offset_y -= 1;
    }
    
    vector<particle_emitter_component> * emitters = ECS->find_components_by_type<particle_emitter_component>();
    for ( const particle_emitter_component &emitter : *emitters ) {
	if ( game_engine->rng.roll_dice( 1, emitter.emits_one_in_x ) == 1 ) {
	      position_component3d * pos = ECS->find_entity_component<position_component3d>( emitter.entity_id );
	      game_engine->messaging->add_message<particle_message>( particle_message ( emitter.message, pos->pos.x, pos->pos.y, pos->pos.z, emitter.ttl, static_cast<chat_emote_color_t>(emitter.color) ) );
	}
    }
}
