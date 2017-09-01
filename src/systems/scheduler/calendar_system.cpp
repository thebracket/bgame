#include "calendar_system.hpp"
#include "../../global_assets/game_calendar.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../bengine/geometry.hpp"

namespace systems {
    namespace calendarsys {
        void run(const double duration_ms) {
            auto hour = calendar->hour;
            auto day = calendar->day;
            calendar->next_minute();
            if (calendar->hour != hour) hour_elapsed = true;
            if (calendar->day != day) day_elapsed = true;

			const double time_overall = calendar->hour + (calendar->minute / 60.0f);
			const double time_as_float = time_overall / 24.0f;
			const double time_as_radians = (time_as_float * 6.28319);
			auto sun_pos = bengine::project_angle(0, 0, 1000.0f, time_as_radians);
			calendar->sun_x = sun_pos.first;
			calendar->sun_y = sun_pos.second;
			calendar->sun_z = 128.0f;
        }
    }
}