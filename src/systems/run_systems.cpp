#include <ctime>
#include "run_systems.hpp"
#include "io/camera_system.hpp"
#include "gui/hud_system.hpp"
#include "scheduler/tick_system.hpp"
#include "../global_assets/game_pause.hpp"
#include "scheduler/calendar_system.hpp"
#include "power/power_system.hpp"
#include "scheduler/initiative_system.hpp"
#include <string>
#include <boost/container/flat_map.hpp>
#include <vector>
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/gl_include.hpp"
#include <chrono>

namespace systems {
	constexpr int CAMERA_SYSTEM = 1;
	constexpr int HUD_SYSTEM = 2;
	constexpr int TICK_SYSTEM = 3;
	constexpr int CALENDAR_SYSTEM = 4;
	constexpr int POWER_SYSTEM = 5;
	constexpr int INITIATIVE_SYSTEM = 6;

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
    }

    void run(const double &duration_ms) {
        if (ImGui::IsKeyDown(GLFW_KEY_P)) {
            show_profiler = !show_profiler;
        }

        run_system(tick::run, duration_ms, TICK_SYSTEM);
        run_system(camerasys::run, duration_ms, CAMERA_SYSTEM);
        run_system(hud::run, duration_ms, HUD_SYSTEM);

        // Items that only run if the simulation has ticked
        if (major_tick) {
            run_system(calendarsys::run, duration_ms, CALENDAR_SYSTEM);
			run_system(initiative::run, duration_ms, INITIATIVE_SYSTEM);
			run_system(power::run, duration_ms, POWER_SYSTEM);
        }

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