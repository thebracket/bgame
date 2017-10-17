#include "calendar_system.hpp"
#include "../../global_assets/game_calendar.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../bengine/geometry.hpp"
#include "../../planet/constants.hpp"

namespace systems {
    namespace calendarsys {
		float rgb_lerp(const float percent, const float start, const float end) {
			const float difference = end - start;
			const float addititve = difference * percent;
			return start + addititve;
		}

		void calculate_sun_moon() {
			const double latitude_sun = ((camera_position->world_y / (double)WORLD_HEIGHT) * REGION_HEIGHT);
			const double time_overall = (calendar->hour - 6) + (calendar->minute / 60.0f);
			const double time_as_float = time_overall / 24.0f;
			const double time_as_radians = (time_as_float * 6.28319);
			auto sun_pos = bengine::project_angle(0, 0, 1000.0f, time_as_radians);
			calendar->sun_x = static_cast<float>(sun_pos.first);
			calendar->sun_y = static_cast<float>(sun_pos.second);
			calendar->sun_z = static_cast<float>(latitude_sun);

			calendar->moon_x = 0.0f - calendar->sun_x;
			calendar->moon_y = 0.0f - calendar->sun_y;
			calendar->moon_z = calendar->sun_z;

			// Sun color; a bit blue at dawn, white at noon, a bit red at sunset
			if (calendar->hour > 5 && calendar->hour < 13) {
				const float sun_pct = (calendar->hour - 6.0f) / 6.0f;
				calendar->sun_r = rgb_lerp(sun_pct, 0.6f, 1.0f);
				calendar->sun_g = rgb_lerp(sun_pct, 0.6f, 1.0f);
				calendar->sun_b = rgb_lerp(sun_pct, 1.0f, 1.0f);
			}
			else if (calendar->hour > 12 && calendar->hour < 19) {
				const float sun_pct = (calendar->hour - 12.0f) / 6.0f;
				calendar->sun_r = rgb_lerp(sun_pct, 1.0f, 1.0f);
				calendar->sun_g = rgb_lerp(sun_pct, 1.0f, 0.6f);
				calendar->sun_b = rgb_lerp(sun_pct, 1.0f, 0.6f);
			}
			else {
				calendar->moon_r = 34.0f / 255.0f;
				calendar->moon_g = 63.0f / 255.0f;
				calendar->moon_b = 89.0f / 255.0f;
			}

			//std::cout << "At: " << +hour << ":" << +calendar->minute << ", sun is at " << calendar->sun_x << ", " << calendar->sun_y << ", " << calendar->sun_z << "\n";
		}

        void run(const double duration_ms) {
            auto hour = calendar->hour;
            auto day = calendar->day;
            calendar->next_minute();
			hour_elapsed = false;
			day_elapsed = false;
            if (calendar->hour != hour) hour_elapsed = true;
            if (calendar->day != day) day_elapsed = true;
			calculate_sun_moon();
        }
    }
}