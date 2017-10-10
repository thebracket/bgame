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
#include "scheduler/corpse_system.hpp"
#include "../global_assets/game_pause.hpp"
#include "scheduler/calendar_system.hpp"
#include "power/power_system.hpp"
#include "scheduler/initiative_system.hpp"
#include "physics/fluid_system.hpp"
#include "ai/ai_status_effects.hpp"
#include "ai/ai_stuck.hpp"
#include "ai/distance_map_system.hpp"
#include "ai/wildlife_population.hpp"
#include "ai/sentient_ai_system.hpp"
#include "physics/movement_system.hpp"
#include "physics/explosive_system.hpp"
#include "overworld/world_system.hpp"
#include "overworld/settler_spawner_system.hpp"
#include "physics/gravity_system.hpp"
#include "physics/door_system.hpp"
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
	constexpr int AI_STATUS_SYSTEM = 13;
	constexpr int AI_STUCK_SYSTEM = 14;
	constexpr int MOVEMENT_SYSTEM = 15;
	constexpr int WORLD_SYSTEM = 16;
	constexpr int SETTLER_SPAWNER_SYSTEM = 17;
	constexpr int GRAVITY_SYSTEM = 18;
	constexpr int EXPLOSIVE_SYSTEM = 19;
	constexpr int DOOR_SYSTEM = 20;
	constexpr int DISTANCE_MAP_SYSTEM = 21;
	constexpr int WILDLIFE_POPULATION_SYSTEM = 22;
	constexpr int SENTIENT_AI_SYSTEM = 23;
	constexpr int CORPSE_SYSEM = 24;

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
		system_names[AI_STATUS_SYSTEM] = "AI Status";
		system_names[AI_STUCK_SYSTEM] = "AI Stuck";
		system_names[MOVEMENT_SYSTEM] = "Movement";
		system_names[WORLD_SYSTEM] = "World";
		system_names[SETTLER_SPAWNER_SYSTEM] = "Settler Spawner";
		system_names[GRAVITY_SYSTEM] = "Gravity";
		system_names[EXPLOSIVE_SYSTEM] = "Explosions";
		system_names[DOOR_SYSTEM] = "Doors";
		system_names[DISTANCE_MAP_SYSTEM] = "Dijkstra";
		system_names[WILDLIFE_POPULATION_SYSTEM] = "Fisheries and Wildlife";
		system_names[SENTIENT_AI_SYSTEM] = "Sentient AI";
		system_names[CORPSE_SYSEM] = "Corpses";
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
			if (hour_elapsed) run_system(settler_spawner::run, duration_ms, SETTLER_SPAWNER_SYSTEM);
			if (day_elapsed || wildlife_population::first_run) run_system(wildlife_population::run, duration_ms, WILDLIFE_POPULATION_SYSTEM);
			run_system(fluids::run, duration_ms, FLUID_SYSTEM);
			run_system(explosives::run, duration_ms, EXPLOSIVE_SYSTEM);
			run_system(doors::run, duration_ms, DOOR_SYSTEM);
			run_system(gravity::run, duration_ms, GRAVITY_SYSTEM);
			run_system(distance_map::run, duration_ms, DISTANCE_MAP_SYSTEM);
			run_system(world::run, duration_ms, WORLD_SYSTEM);
			run_system(initiative::run, duration_ms, INITIATIVE_SYSTEM);
			if (day_elapsed) run_system(sentient_ai_system::run, duration_ms, SENTIENT_AI_SYSTEM);
			run_system(corpse_system::run, duration_ms, CORPSE_SYSEM);
			run_system(power::run, duration_ms, POWER_SYSTEM);
			run_system(ai_status_effects::run, duration_ms, AI_STATUS_SYSTEM);
			run_system(ai_stuck::run, duration_ms, AI_STUCK_SYSTEM);
			run_system(movement::run, duration_ms, MOVEMENT_SYSTEM);
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