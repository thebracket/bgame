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
			const double time_overall = (calendar->hour - 6.0f + (static_cast<float>(calendar->minute)/60.0f)) / 24.0f;
			const double time_as_float = time_overall;
			const double time_as_radians = (time_as_float * 6.28319);
			//auto sun_pos = bengine::project_angle(0, 0, sun_distance, time_as_radians);
			const float X = std::cos(time_as_radians) * 129.0f;
			const float Y = std::sin(time_as_radians) * 129.0f;
			calendar->sun_x = X + 128.0f;
			calendar->sun_y = Y + 128.0f;
			calendar->sun_z = static_cast<float>(129.0f);

			calendar->moon_x = 0.0f - calendar->sun_x;
			calendar->moon_y = 0.0f - calendar->sun_y;
			calendar->moon_z = calendar->sun_z;

			// Sun color; a bit blue at dawn, white at noon, a bit red at sunset
			/*if (calendar->hour > 5 && calendar->hour < 13) {
				const float sun_pct = (calendar->hour - 6.0f) / 6.0f;
				calendar->sun_r = rgb_lerp(sun_pct, 0.9f, 1.5f);
				calendar->sun_g = rgb_lerp(sun_pct, 0.9f, 1.5f);
				calendar->sun_b = rgb_lerp(sun_pct, 1.5f, 1.5f);
			}
			else if (calendar->hour > 12 && calendar->hour < 19) {
				const float sun_pct = (calendar->hour - 12.0f) / 6.0f;
				calendar->sun_r = rgb_lerp(sun_pct, 1.5f, 1.5f);
				calendar->sun_g = rgb_lerp(sun_pct, 1.5f, 0.9f);
				calendar->sun_b = rgb_lerp(sun_pct, 1.5f, 0.9f);
			}
			else {
				calendar->moon_r = 34.0f / 255.0f;
				calendar->moon_g = 63.0f / 255.0f;
				calendar->moon_b = 89.0f / 255.0f;
			}*/
			calendar->sun_r = 4.0f;
			calendar->sun_g = 4.0f;
			calendar->sun_b = 4.0f;

			//std::cout << "At: " << +hour << ":" << +calendar->minute << ", sun is at " << calendar->sun_x << ", " << calendar->sun_y << ", " << calendar->sun_z << "\n";
		}

        void run(const double duration_ms) {
			bool do_sun = false;
            auto hour = calendar->hour;
            auto day = calendar->day;
			if (calendar->minute % 10 == 0) do_sun = true;
            calendar->next_minute();
			hour_elapsed = false;
			day_elapsed = false;
            if (calendar->hour != hour) hour_elapsed = true;
            if (calendar->day != day) day_elapsed = true;
			if (do_sun) calculate_sun_moon();
        }
    }
}