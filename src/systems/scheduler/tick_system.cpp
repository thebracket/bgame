#include "stdafx.h"
#include "tick_system.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../bengine/imgui.h"
#include "../keydamper.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/game_config.hpp"

namespace systems {
    namespace tick {
		const double MS_PER_TICK = config::game_config.ticks_per_ms;
        double time_count = 0.0;
        int slow_tick_count = 0;

        void run(const double &duration_ms) {
            major_tick = false;
            slow_tick = false;
            hour_elapsed = false;
            day_elapsed = false;

            if (is_key_down(GLFW_KEY_SPACE)) {
                if (pause_mode == PAUSED) {
                    pause_mode = RUNNING;
                } else {
                    pause_mode = PAUSED;
                }
            }
            if (is_key_down(GLFW_KEY_SLASH)) {
                pause_mode = ONE_STEP;
            }

            if (pause_mode != PAUSED) {
                time_count += duration_ms;

                if (time_count > MS_PER_TICK) {
                    time_count = 0.0;
                    major_tick = true;

                    ++slow_tick_count;
                    if (slow_tick_count > 9) {
                        slow_tick_count = 0;
                        slow_tick = true;
                    }

                    if (pause_mode == ONE_STEP) {
			        	pause_mode = PAUSED;
			        }
                }
            }
        }
    }
}
