#include "settler_wander.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../main/game_designations.hpp"

void wander_randomly(entity_t &entity, position_t &original) {
	// Never wander if it is prohibited
	if (designations->standing_order_idle_move == standing_orders::SO_IDLE_STATIC) return;

	// Don't wander far from Cordex if that is prohibited
	if (designations->standing_order_idle_move == standing_orders::SO_IDLE_WANDER_CLOSE) {
		const float distance = distance3d(original.x, original.y, original.z, REGION_WIDTH/2, REGION_HEIGHT/2, REGION_DEPTH/2);
		if (distance > 15.0F) return;
	}

	emit_deferred(entity_wants_to_move_randomly_message{entity.id});
}