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
			const double moon_time_overall = (calendar->hour - 18.0f + (static_cast<float>(calendar->minute) / 60.0f)) / 24.0f;
			const double time_as_float = time_overall;
			const double time_as_radians = (time_as_float * 6.28319);
			const double moon_time_as_radians = (moon_time_overall * 6.28319);
			//auto sun_pos = bengine::project_angle(0, 0, sun_distance, time_as_radians);
			const float X = std::cos(time_as_radians) * 129.0f;
			const float Y = std::sin(time_as_radians) * 129.0f;
			calendar->sun_x = X + 128.0f;
			calendar->sun_y = Y + 128.0f;
			calendar->sun_z = static_cast<float>(129.0f);

			const float MX = std::sin(moon_time_as_radians) * 129.0f;
			const float MY = std::cos(moon_time_as_radians) * 129.0f;
			calendar->moon_x = MX + 128.0f;
			calendar->moon_y = MY + 128.0f;
			calendar->moon_z = calendar->sun_z;

			calendar->moon_r = 0.15f;
			calendar->moon_g = 0.15f;
			calendar->moon_b = 0.15f;

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