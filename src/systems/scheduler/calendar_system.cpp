#include "stdafx.h"
#include "calendar_system.hpp"
#include "../../global_assets/game_calendar.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../bengine/geometry.hpp"
#include "../../planet/constants.hpp"
#include "../../render_engine/camera.hpp"

namespace systems {
    namespace calendarsys {
		constexpr float sun_distance = 64.0f;

		float rgb_lerp(const float percent, const float start, const float end) {
			const float difference = end - start;
			const float addititve = difference * percent;
			return start + addititve;
		}

		void calculate_sun_moon() {
			render::sun_moved = true;
			const float minutes_fraction = calendar->minute / 60.0f;
			const float hours_fraction = (float)calendar->hour + minutes_fraction - 6.0f;
			const float time_overall = hours_fraction / 24.0f;
			const float time_as_float = time_overall; // Should be in the range 0-1
			const float time_as_radians = (time_as_float * 6.28319f);
			//auto sun_pos = bengine::project_angle(0, 0, sun_distance, time_as_radians);
			const float X = std::cos(time_as_radians);
			const float Y = std::sin(time_as_radians);
			calendar->sun_x = X;
			calendar->sun_y = Y;
			calendar->sun_z = static_cast<float>(0.0f);

			//std::cout << "At: " << +calendar->hour << ":" << +calendar->minute << ", sun is at " << calendar->sun_x << ", " << calendar->sun_y << ", " << calendar->sun_z << "\n";
		}

		static bool first_calendar = true;

        void run(const double duration_ms) {
            auto hour = calendar->hour;
            auto day = calendar->day;
            calendar->next_minute();
			hour_elapsed = false;
			day_elapsed = false;
            if (calendar->hour != hour) hour_elapsed = true;
            if (calendar->day != day) day_elapsed = true;
			//if (hour_elapsed || first_calendar) {
				calculate_sun_moon();
			//	first_calendar = false;
			//}
        }
    }
}