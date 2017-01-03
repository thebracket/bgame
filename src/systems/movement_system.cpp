#include "movement_system.hpp"
#include "../messages/entity_moved_message.hpp"
#include "../messages/renderables_changed_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/vegetation_damage_message.hpp"
#include "../main/game_globals.hpp"
#include "../components/slidemove.hpp"
#include "../components/initiative.hpp"
#include <rltk.hpp>

octree_t entity_octree{REGION_WIDTH, REGION_HEIGHT, REGION_DEPTH};

void movement_system::configure() {
    system_name = "Movement System";
    subscribe_mbox<entity_wants_to_move_message>();
    subscribe<entity_wants_to_move_randomly_message>([] (entity_wants_to_move_randomly_message &msg) {

        auto original = entity(msg.entity_id)->component<position_t>();

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
            bool can_go = true;
            const int dest = mapidx(pos);

            if (current_region->water_level[dest]>2 && current_region->water_level[tile_index]<3) can_go = false;

            if (can_go) emit(entity_wants_to_move_message{msg.entity_id, pos});
        }
    });
    subscribe<entity_wants_to_flee_message>([] (entity_wants_to_flee_message &msg) {
        auto pos = entity(msg.entity_id)->component<position_t>();
        auto other_pos = entity(msg.flee_from_id)->component<position_t>();

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
    subscribe<entity_wants_to_charge_message>([] (entity_wants_to_charge_message &msg) {
        auto pos = entity(msg.entity_id)->component<position_t>();
        auto other_pos = entity(msg.charge_to_id)->component<position_t>();

        if (pos->x > other_pos->x && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_WEST)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x-1, pos->y, pos->z } });
        } else if (pos->x < other_pos->x && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_EAST)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x+1, pos->y, pos->z } });
        } else if (pos->y < other_pos->y && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_SOUTH)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y+1, pos->z } });
        } else if (pos->y > other_pos->y && current_region->tile_flags[mapidx(pos->x,pos->y,pos->z)].test(CAN_GO_NORTH)) {
            emit(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y-1, pos->z } });
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
            entity_octree.add_node(octree_location_t{static_cast<int>(pos.x), static_cast<int>(pos.y), pos.z, e.id});
        });
    }

    std::queue<entity_wants_to_move_message> * movers = mbox<entity_wants_to_move_message>();
	while (!movers->empty()) {
        entity_wants_to_move_message msg = movers->front();
		movers->pop();

	    if (!entity(msg.entity_id)) break;
        auto epos = entity(msg.entity_id)->component<position_t>();
        if (!epos) break;
        position_t origin{epos->x, epos->y, epos->z};

        // Bounds check
        if (msg.destination.x < 1 || msg.destination.x > REGION_WIDTH-1 || msg.destination.y < 1 || msg.destination.y > REGION_HEIGHT-1
            || msg.destination.z < 1 || msg.destination.z > REGION_DEPTH-1) break;

        // Add sliding effect
        auto slide = entity(msg.entity_id)->component<slidemove_t>();
        auto initiative = entity(msg.entity_id)->component<initiative_t>();

        const int dX = msg.destination.x - epos->x;
        const int dY = msg.destination.y - epos->y;
        const int dZ = msg.destination.z - epos->z;

        const float deltaX = (float)dX / (float)initiative->initiative;
        const float deltaY = (float)dY / (float)initiative->initiative;
        const float deltaZ = (float)dZ / (float)initiative->initiative;

        if (!slide && initiative) {
            entity(msg.entity_id)->assign(slidemove_t{deltaX, deltaY, deltaZ, initiative->initiative});
        } else if (slide && initiative) {
            slide->offsetX = deltaX;
            slide->offsetY = deltaY;
            slide->offsetZ = deltaZ;
            slide->lifespan = initiative->initiative;
        }

        // Move
        epos->x = msg.destination.x;
        epos->y = msg.destination.y;
        epos->z = msg.destination.z;
        epos->offsetX = 0.0F - (float)dX;
        epos->offsetY = 0.0F - (float)dY;
        epos->offsetZ = 0.0F - (float)dZ;

        // Do vegetation damage
        const auto idx = mapidx(msg.destination.x, msg.destination.y, msg.destination.z);
        if (current_region->tile_vegetation_type[idx] > 0) {
            emit_deferred(vegetation_damage_message{idx, 1});
        }

        emit(entity_moved_message{msg.entity_id, origin, msg.destination});
	    emit(renderables_changed_message{});
	}

    std::queue<entity_moved_message> * moved = mbox<entity_moved_message>();
    while (!moved->empty()) {
        entity_moved_message msg = moved->front();
        moved->pop();        

        octree_location_t start = octree_location_t{static_cast<int>(msg.origin.x), static_cast<int>(msg.origin.y), msg.origin.z, msg.entity_id};
        octree_location_t end = octree_location_t{static_cast<int>(msg.destination.x), static_cast<int>(msg.destination.y), msg.destination.z, msg.entity_id};
        entity_octree.remove_node(start);
        entity_octree.add_node(end);
    }
}
