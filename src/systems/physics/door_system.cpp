#include "door_system.hpp"
#include "../../components/construct_provides_door.hpp"
#include "../../components/position.hpp"
#include "../../messages/build_request_message.hpp"
#include "../../messages/recalculate_mining_message.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/renderables_changed_message.hpp"
#include "../../planet/region/region.hpp"

using namespace region;

void door_system::configure() {
    system_name = "Door System";
    subscribe<door_changed_message>([this] (door_changed_message &msg) {
        dirty = true;
    });
}

void door_system::update(const double ms) {
    if (dirty) {
        each<construct_door_t, position_t>([] (entity_t &e, construct_door_t &door, position_t &pos) {
            const int idx = mapidx(pos.x, pos.y, pos.z);
            if (door.locked) {
                set_tile_type(idx, tile_type::CLOSED_DOOR);
            } else {
                set_tile_type(idx, tile_type::FLOOR);
            }

            tile_calculate(pos.x, pos.y, pos.z);
            for (int Z=-2; Z<3; ++Z) {
                for (int Y=-2; Y<3; ++Y) {
                    for (int X=-2; X<3; ++X) {
                        tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
                    }
                }
            }
        });
        emit(recalculate_mining_message{});
        emit(renderables_changed_message{});
        emit(map_dirty_message{});
        dirty = false;
    }
}
