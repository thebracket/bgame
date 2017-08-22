#include "tick_system.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../bengine/gl_include.hpp"

namespace systems {
    namespace tick {
        constexpr double MS_PER_TICK = 33.0;
        double time_count = 0.0;
        int slow_tick_count = 0;

        void run(const double &duration_ms) {
            major_tick = false;
            slow_tick = false;
            hour_elapsed = false;
            day_elapsed = false;

            if (ImGui::IsKeyDown(GLFW_KEY_SPACE)) {
                if (pause_mode == PAUSED) {
                    pause_mode = RUNNING;
                } else {
                    pause_mode = PAUSED;
                }
            }
            if (ImGui::IsKeyDown(GLFW_KEY_PERIOD)) {
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