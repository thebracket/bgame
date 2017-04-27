#include "ai_sleep_time_system.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/ai_tags/ai_tag_sleep_shift.hpp"
#include "../../../components/sleep_clock_t.hpp"
#include "../../../messages/tick_message.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../components/claimed_t.hpp"
#include "../distance_map_system.hpp"
#include "../../../components/logger.hpp"
#include "../../../messages/log_message.hpp"
#include "../../../messages/entity_moved_message.hpp"
#include "../../../components/construct_provides_sleep.hpp"

void ai_sleep_time_system::configure() {
    system_name = "AI Sleep System";
    subscribe_mbox<hour_elapsed_message>();
}

void ai_sleep_time_system::update(const double duration_ms) {
    // Every hour, we add 1 to everyone's sleep clock unless they are sleeping.
    each_mbox<hour_elapsed_message>([this] (const hour_elapsed_message msg) {
        each<sleep_clock_t>([] (entity_t &e, sleep_clock_t &sleep) {
            if (sleep.is_sleeping) {
                if (sleep.sleep_requirement > 0) {
                    --sleep.sleep_requirement;
                }
            } else {
                ++sleep.sleep_requirement;
            }
        });
    });

    each<settler_ai_t, ai_tag_my_turn_t, sleep_clock_t, position_t>([]
                       (entity_t &e, settler_ai_t &ai, ai_tag_my_turn_t &turn, sleep_clock_t &sleep, position_t &pos)
    {
        auto schedule = e.component<ai_tag_sleep_shift_t>();
        if (schedule == nullptr) {
            sleep.is_sleeping = false;
            return;
        }

        // It's my turn, and I'm sleeping
        delete_component<ai_tag_my_turn_t>(e.id);

        // Are we in bed already?
        if (sleep.is_sleeping) return;

        // Find a bed
        const int idx = mapidx(pos);
        const int16_t distance = bed_map.get(idx);
        if (distance == 0) {
            // We've reached a bed - sleepy time!
            sleep.is_sleeping = true;

            // Find the bed and claim it
            each<construct_provides_sleep_t, position_t>([&e, &pos] (entity_t &BED, construct_provides_sleep_t &sleep, position_t &bpos) {
                if (pos == bpos) {
                    BED.assign(claimed_t{e.id});
                    emit(bed_changed_message{});
                }
            });
            return;
        } else if (distance >= MAX_DIJSTRA_DISTANCE) {
            // There is no bed - sleep rough
            sleep.is_sleeping = true;
            // TODO: Bad thoughts!
            emit_deferred(log_message{LOG{}.settler_name(e.id)->text(" cannot find a bed, and is sleeping rough.")->chars});
            return;
        } else {
            // Path towards the bed
            position_t destination = bed_map.find_destination(pos);
            emit_deferred(entity_wants_to_move_message{e.id, destination});
            return;
        }
    });
}