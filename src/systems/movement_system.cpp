#include "movement_system.hpp"
#include "../messages/entity_moved_message.hpp"
#include "../messages/renderables_changed_message.hpp"
#include "../game_globals.hpp"
#include <rltk.hpp>

void movement_system::configure() {
    subscribe_mbox<entity_wants_to_move_message>();
    subscribe<entity_wants_to_move_randomly_message>([] (entity_wants_to_move_randomly_message &msg) {

        position_t * original = entity(msg.entity_id)->component<position_t>();

        position_t pos = *original;
        const int tile_index = mapidx(pos.x, pos.y, pos.z);
        const int direction = rng.roll_dice(1,6);
        switch (direction) {
            case 1 : if (current_region->tile_flags[tile_index].test(CAN_GO_UP)) pos.z++; break;
            case 2 : if (current_region->tile_flags[tile_index].test(CAN_GO_DOWN)) pos.z--; break;
            case 3 : if (current_region->tile_flags[tile_index].test(CAN_GO_NORTH)) pos.y--; break;
            case 4 : if (current_region->tile_flags[tile_index].test(CAN_GO_SOUTH)) pos.y++; break;
            case 5 : if (current_region->tile_flags[tile_index].test(CAN_GO_EAST)) pos.x++; break;
            case 6 : if (current_region->tile_flags[tile_index].test(CAN_GO_WEST)) pos.x--; break;
        }
        if (!current_region->solid[tile_index]) { 
            emit(entity_wants_to_move_message{msg.entity_id, pos});
        }
    });
}

void movement_system::update(const double ms) {
    std::queue<entity_wants_to_move_message> * movers = mbox<entity_wants_to_move_message>();
	while (!movers->empty()) {
        entity_wants_to_move_message msg = movers->front();
		movers->pop();

        position_t * epos = entity(msg.entity_id)->component<position_t>();
        epos->x = msg.destination.x;
        epos->y = msg.destination.y;
        epos->z = msg.destination.z;

        emit(entity_moved_message{msg.entity_id, msg.destination});
	    emit(renderables_changed_message{});
	}
}
