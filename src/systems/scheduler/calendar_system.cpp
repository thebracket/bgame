#include "calendar_system.hpp"
#include "../../global_assets/game_calendar.hpp"
#include "../../global_assets/game_pause.hpp"

namespace systems {
    namespace calendarsys {
        void run(const double duration_ms) {
            auto hour = calendar->hour;
            auto day = calendar->day;
            calendar->next_minute();
            if (calendar->hour != hour) hour_elapsed = true;
            if (calendar->day != day) day_elapsed = true;
        }
    }
}