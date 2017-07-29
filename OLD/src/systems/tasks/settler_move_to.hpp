#pragma once

inline void move_to(entity_t &e, position_t &pos, position_t &destination) {
	emit_deferred(entity_wants_to_move_message{e.id, destination});
}