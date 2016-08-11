#include "movement_system.hpp"
#include "../messages/entity_moved_message.hpp"
#include "../messages/renderables_changed_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../game_globals.hpp"
#include <rltk.hpp>

octree_t entity_octree{REGION_WIDTH, REGION_HEIGHT, REGION_DEPTH};

void movement_system::configure() {
    system_name = "Movement System";
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
    subscribe<entity_wants_to_flee_message>([] (entity_wants_to_flee_message &msg) {
        position_t * pos = entity(msg.entity_id)->component<position_t>();
        position_t * other_pos = entity(msg.flee_from_id)->component<position_t>();

        if (pos->x > other_pos->x && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_EAST)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x+1, pos->y, pos->z } });
        } else if (pos->x < other_pos->x && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_WEST)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x-1, pos->y, pos->z } });
        } else if (pos->y < other_pos->y && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_NORTH)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y-1, pos->z } });
        } else if (pos->y > other_pos->y && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_SOUTH)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y+1, pos->z } });
        } else {
            emit(entity_wants_to_move_randomly_message{msg.entity_id});
        }
    });

    // When an entity actually moves, update the octree
    subscribe_mbox<entity_moved_message>();
}

void movement_system::update(const double ms) {
    // If the octree is empty, fill it
    if (entity_octree.total_nodes == 0) {
        each<position_t>([] (entity_t &e, position_t &pos) {
            entity_octree.add_node(octree_location_t{pos.x, pos.y, pos.z, e.id});
        });
    }

    std::queue<entity_wants_to_move_message> * movers = mbox<entity_wants_to_move_message>();
	while (!movers->empty()) {
        entity_wants_to_move_message msg = movers->front();
		movers->pop();

        position_t * epos = entity(msg.entity_id)->component<position_t>();
        position_t origin{epos->x, epos->y, epos->z};
        epos->x = msg.destination.x;
        epos->y = msg.destination.y;
        epos->z = msg.destination.z;

        // Do vegetation damage
        const int idx = mapidx(msg.destination.x, msg.destination.y, msg.destination.z);
        if (current_region->tile_vegetation_type[idx] > 0) {
            --current_region->tile_hit_points[idx];
            //std::cout << "Vegetation Damaged by Walking - " << +current_region->tile_hit_points[idx] << " hp remain\n";
            if (current_region->tile_hit_points[idx] < 1) {
                // We've destroyed the vegetation!
                //std::cout << "Vegetation Destroyed\n";
                current_region->tile_hit_points[idx] = 0;
                current_region->tile_vegetation_type[idx] = 0;
                current_region->calc_render(idx);
                emit(map_dirty_message{});
            }
        }

        emit(entity_moved_message{msg.entity_id, origin, msg.destination});
	    emit(renderables_changed_message{});
	}

    std::queue<entity_moved_message> * moved = mbox<entity_moved_message>();
    while (!moved->empty()) {
        entity_moved_message msg = moved->front();
        moved->pop();

        octree_location_t start = octree_location_t{msg.origin.x, msg.origin.y, msg.origin.z, msg.entity_id};
        octree_location_t end = octree_location_t{msg.destination.x, msg.destination.y, msg.destination.z, msg.entity_id};
        entity_octree.remove_node(start);
        entity_octree.add_node(end);
    }
}
