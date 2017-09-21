#include <ctime>
#include "run_systems.hpp"
#include "io/camera_system.hpp"
#include "gui/tooltip_system.hpp"
#include "gui/hud_system.hpp"
#include "gui/log_system.hpp"
#include "gui/civ_ui_system.hpp"
#include "gui/settler_info_system.hpp"
#include "gui/units_info_system.hpp"
#include "scheduler/tick_system.hpp"
#include "../global_assets/game_pause.hpp"
#include "scheduler/calendar_system.hpp"
#include "power/power_system.hpp"
#include "scheduler/initiative_system.hpp"
#include "physics/fluid_system.hpp"
#include <string>
#include <boost/container/flat_map.hpp>
#include <vector>
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/gl_include.hpp"
#include "keydamper.hpp"
#include "mouse.hpp"
#include "../global_assets/game_mode.hpp"
#include <chrono>

namespace systems {
	constexpr int CAMERA_SYSTEM = 1;
	constexpr int HUD_SYSTEM = 2;
	constexpr int TICK_SYSTEM = 3;
	constexpr int CALENDAR_SYSTEM = 4;
	constexpr int POWER_SYSTEM = 5;
	constexpr int INITIATIVE_SYSTEM = 6;
	constexpr int FLUID_SYSTEM = 7;
	constexpr int LOG_SYSTEM = 8;
	constexpr int TOOLTIP_SYSTEM = 9;
	constexpr int CIVUI_SYSTEM = 10;
	constexpr int SETTLERUI_SYSTEM = 11;
	constexpr int UNITSUI_SYSTEM = 12;

    boost::container::flat_map<int, std::pair<int, std::vector<float>>> run_time;
    boost::container::flat_map<int, std::string> system_names;

    template <typename F>
    inline void run_system(F &run_func, const double &duration_ms, const int SYSTEM) {
        auto start_time = std::chrono::high_resolution_clock::now();
        run_func(duration_ms);
        auto end_time = std::chrono::high_resolution_clock::now();
        const double system_running_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        auto finder = run_time.find(SYSTEM);
        if (finder == run_time.end()) {
            std::vector<float> timer;
            timer.resize(100);
            timer[0] = static_cast<float>(system_running_time);
            run_time.insert(std::make_pair(SYSTEM, std::make_pair(1, timer)));
        } else {
            finder->second.second[finder->second.first] = static_cast<float>(system_running_time);
            ++finder->second.first;
            if (finder->second.first > 100) finder->second.first = 0;
        }
    }

    void init() {
        system_names[CAMERA_SYSTEM] = "Camera";
        system_names[HUD_SYSTEM] = "HUD";
        system_names[TICK_SYSTEM] = "Tick";
        system_names[CALENDAR_SYSTEM] = "Calendar";
		system_names[POWER_SYSTEM] = "Power";
		system_names[INITIATIVE_SYSTEM] = "Initiative";
		system_names[FLUID_SYSTEM] = "Fluids";
		system_names[LOG_SYSTEM] = "Logging";
		system_names[TOOLTIP_SYSTEM] = "Tooltips";
		system_names[CIVUI_SYSTEM] = "Civ UI";
		system_names[SETTLERUI_SYSTEM] = "Settler UI";
		system_names[UNITSUI_SYSTEM] = "Units UI";
		game_master_mode = PLAY;
    }

	void run(const double &duration_ms) {
		if (ImGui::IsKeyDown(GLFW_KEY_P)) {
			show_profiler = !show_profiler;
		}

		add_time(duration_ms); // Add time to the key damper
		poll_mouse();

		run_system(tick::run, duration_ms, TICK_SYSTEM);
		run_system(camerasys::run, duration_ms, CAMERA_SYSTEM);
		run_system(hud::run, duration_ms, HUD_SYSTEM);
		run_system(tooltips::run, duration_ms, TOOLTIP_SYSTEM);
		if (game_master_mode == CIVS || game_master_mode == CIV_NEGOTIATE)
		{
			run_system(civ_ui::run, duration_ms, CIVUI_SYSTEM);
		}
		else if (game_master_mode == UNITS) {
			run_system(units_ui::run, duration_ms, UNITSUI_SYSTEM);
		}
		else if (game_master_mode == SETTLER) {
			run_system(settler_ui::run, duration_ms, SETTLERUI_SYSTEM);
		}

        // Items that only run if the simulation has ticked
        if (major_tick) {
			logging::age_log();
            run_system(calendarsys::run, duration_ms, CALENDAR_SYSTEM);
			run_system(fluids::run, duration_ms, FLUID_SYSTEM);
			run_system(initiative::run, duration_ms, INITIATIVE_SYSTEM);
			run_system(power::run, duration_ms, POWER_SYSTEM);
        }

		// Logging goes at the end to catch new messages
		run_system(logging::run, duration_ms, LOG_SYSTEM);

        // Profiler
        if (show_profiler) {
            ImGui::Begin("Systems Profile");
            ImGui::Text("Frame time: %f", duration_ms);
            for (auto sys_finder=run_time.begin(); sys_finder != run_time.end(); ++sys_finder) {
                const int id = sys_finder->first;
                auto name_finder = system_names.find(id);
                if (name_finder != system_names.end()) {
                    ImGui::PlotLines(name_finder->second.c_str(), (const float *)&sys_finder->second.second.at(0), 100);
                }
            }
            ImGui::End();
        }
    }
}