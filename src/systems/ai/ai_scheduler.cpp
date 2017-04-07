#include "ai_scheduler.hpp"
#include "../../components/ai_tag_my_turn.hpp"
#include "../../components/ai_tag_my_turn.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/ai_mode_idle.hpp"
#include "../../components/ai_tag_leisure_shift.hpp"
#include "../../components/ai_tag_sleep_shift.hpp"
#include "../../components/ai_tag_work_shift.hpp"
#include "../../main/game_globals.hpp"

void ai_scheduler::configure() {
    system_name = "AI Scheduler";
}

void ai_scheduler::update(const double duration_ms)
{
    // We're idle, so we need to determine what time it is and engage the appropriate AI tags
    each<ai_tag_my_turn_t, settler_ai_t, ai_mode_idle_t>([] (entity_t &e, ai_tag_my_turn_t &turn, settler_ai_t &ai, ai_mode_idle_t &idle) {
        const int shift_id = ai.shift_id;
        const int hour_of_day = calendar->hour;
        const shift_type_t current_schedule = calendar->defined_shifts[shift_id].hours[hour_of_day];

        delete_component<ai_mode_idle_t>(e.id);
        delete_component<ai_tag_leisure_shift_t>(e.id);
        delete_component<ai_tag_work_shift_t>(e.id);
        delete_component<ai_tag_sleep_shift_t>(e.id);

        switch (current_schedule) {
            case SLEEP_SHIFT : { e.assign(ai_tag_sleep_shift_t{}); } break;
            case LEISURE_SHIFT : { e.assign(ai_tag_leisure_shift_t{}); } break;
            case WORK_SHIFT : { e.assign(ai_tag_work_shift_t{}); } break;
        }
    });
}