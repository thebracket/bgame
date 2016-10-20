#include "gravity_system.hpp"
#include "../messages/tick_message.hpp"
#include "../messages/entity_moved_message.hpp"
#include "../messages/inflict_damage_message.hpp"
#include "../components/falling.hpp"
#include "../main/game_globals.hpp"
#include <boost/container/flat_set.hpp>

void gravity_system::configure() {
    system_name = "Gravity System";
    subscribe_mbox<tick_message>();
}

void gravity_system::update(const double ms) {
    std::queue<tick_message> * ticks = mbox<tick_message>();
	while (!ticks->empty()) {
		ticks->pop();

        // Entities that are currently falling
        boost::container::flat_set<std::size_t> falling_entities;

        each<position_t, falling_t>([&falling_entities] (entity_t &e, position_t &pos, falling_t &fall) {
            falling_entities.insert(e.id);
            const int idx = mapidx(pos.x, pos.y, pos.z);

            if (current_region->tile_flags[idx].test(CAN_STAND_HERE)) {
                // We hit the ground
                if (fall.distance > 2) {
                    emit_deferred(inflict_damage_message{e.id, rng.roll_dice(fall.distance, 6), "falling damage"});
                    delete_component<falling_t>(e.id);
                }
            } else {
                // Still falling
                emit_deferred(entity_wants_to_move_message{e.id, position_t{pos.x, pos.y, pos.z-1}});
                ++fall.distance;
            }
        });

        // See if any entities need to fall
        each<position_t>([&falling_entities] (entity_t &e, position_t &pos) {
            if (falling_entities.find(e.id) == falling_entities.end()) { // Ignore entities that are already falling
                const int idx = mapidx(pos.x, pos.y, pos.z);
                const int idx_below = mapidx(pos.x, pos.y, pos.z);

                bool is_falling = true;
                if (current_region->tile_flags[idx].test(CAN_STAND_HERE)) is_falling = false;
                if (is_falling) {
                    e.assign(falling_t{0});
                }
            }
        });
    }
}
