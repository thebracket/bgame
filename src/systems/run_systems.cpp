#include "stdafx.h"
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
#include "ai/mining_system.hpp"
#include "ai/architecture_system.hpp"
#include "ai/inventory_system.hpp"
#include "ai/stockpile_system.hpp"
#include "ai/workflow_system.hpp"
#include "ai/settler/ai_visibility_scan.hpp"
#include "ai/settler/ai_new_arrival.hpp"
#include "ai/settler/ai_scheduler.hpp"
#include "ai/settler/ai_leisure_time.hpp"
#include "ai/settler/ai_sleep_time.hpp"
#include "ai/settler/ai_work_time.hpp"
#include "ai/settler/ai_work_lumberjack.hpp"
#include "ai/settler/ai_work_mining.hpp"
#include "ai/settler/ai_work_guard.hpp"
#include "ai/settler/ai_work_harvest.hpp"
#include "ai/settler/ai_work_lever_pull.hpp"
#include "ai/settler/ai_work_building.hpp"
#include "ai/settler/ai_work_order.hpp"
#include "ai/settler/ai_work_architect.hpp"
#include "ai/settler/ai_work_hunt.hpp"
#include "ai/settler/ai_work_butcher.hpp"
#include "ai/settler/ai_idle_time.hpp"
#include "ai/settler/ai_work_farm_clear.hpp"
#include "ai/settler/ai_work_farm_plant.hpp"
#include "ai/settler/ai_work_farm_fertilize.hpp"
#include "ai/settler/ai_work_farm_fixsoil.hpp"
#include "ai/settler/ai_work_farm_water.hpp"
#include "ai/settler/ai_work_farm_weed.hpp"
#include "physics/trigger_system.hpp"
#include "physics/movement_system.hpp"
#include "physics/explosive_system.hpp"
#include "overworld/world_system.hpp"
#include "overworld/settler_spawner_system.hpp"
#include "physics/gravity_system.hpp"
#include "physics/door_system.hpp"
#include "physics/item_wear_system.hpp"
#include "damage/settler_ranged_attack_system.hpp"
#include "damage/settler_melee_attacks_system.hpp"
#include "damage/sentient_attacks_system.hpp"
#include "damage/creature_attacks_system.hpp"
#include "damage/turret_ranged_attack_system.hpp"
#include "damage/damage_system.hpp"
#include "damage/kill_system.hpp"
#include "damage/healing_system.hpp"
#include "physics/topology_system.hpp"
#include "physics/visibility_system.hpp"
#include "physics/vegetation_system.hpp"
#include "gui/design_mode.hpp"
#include "gui/design_lumberjack.hpp"
#include "gui/design_buildings.hpp"
#include "gui/design_harvest.hpp"
#include "gui/design_guardpoints.hpp"
#include "gui/design_architecture.hpp"
#include "gui/design_mining.hpp"
#include "gui/design_stockpiles.hpp"
#include "gui/particle_system.hpp"
#include "gui/workflow_ui_system.hpp"
#include "gui/item_info_system.hpp"
#include "gui/job_center_ui.hpp"
#include "gui/wish_mode.hpp"
#include "../bengine/imgui.h"
#include "keydamper.hpp"
#include "mouse.hpp"
#include "../global_assets/game_mode.hpp"
#include <boost/container/flat_map.hpp>
#include "../global_assets/debug_flags.hpp"
#include "ai/settler/ai_work_stockpile.hpp"
#include "ai/settler/ai_deconstruct.hpp"
#include "gui/building_info.hpp"
#include "scheduler/hunger_system.hpp"
#include "ai/settler/ai_leisure_eat.hpp"
#include "ai/settler/ai_leisure_drink.hpp"

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
	constexpr int CORPSE_SYSTEM = 24;
	constexpr int MINING_SYSTEM = 25;
	constexpr int INVENTORY_SYSTEM = 26;
	constexpr int STOCKPILE_SYSTEM = 27;
	constexpr int WORKFLOW_SYSTEM = 28;
	constexpr int VISIBILITY_SCAN_SYSTEM = 29;
	constexpr int NEW_ARRIVAL_SYSTEM = 30;
	constexpr int AI_SCHEDULER = 31;
	constexpr int AI_LEISURE_TIME = 32;
	constexpr int AI_SLEEP_TIME = 33;
	constexpr int AI_WORK_TIME = 34;
	constexpr int AI_WORK_LUMBERJACK = 35;
	constexpr int AI_WORK_MINING = 36;
	constexpr int AI_WORK_GUARD = 37;
	constexpr int AI_WORK_HARVEST = 38;
	constexpr int AI_WORK_LEVER_PULL = 39;
	constexpr int AI_WORK_BUILDING = 40;
	constexpr int AI_WORK_ORDER = 41;
	constexpr int AI_WORK_ARCHITECT = 42;
	constexpr int AI_WORK_HUNT = 43;
	constexpr int AI_IDLE = 44;
	constexpr int TRIGGER_SYSTEM = 45;
	constexpr int SETTLER_RANGED_ATTACK = 46;
	constexpr int SETTLER_MELEE_ATTACK = 47;
	constexpr int SENTIENT_ATTACKS = 48;
	constexpr int CREATURE_ATTACKS = 49;
	constexpr int TURRET_ATTACKS = 50;
	constexpr int DAMAGE_SYSTEM = 51;
	constexpr int KILL_SYSTEM = 52;
	constexpr int HEALING_SYSTEM = 53;
	constexpr int TOPOLOGY_SYSTEM = 54;
	constexpr int VISIBILITY_SYSTEM = 55;
	constexpr int VEGETATION_SYSTEM = 56;
	constexpr int DESIGN_LUMBERJACK_SYSTEM = 57;
	constexpr int DESIGN_MODE_SYSTEM = 58;
	constexpr int PARTICLE_SYSTEM = 59;
	constexpr int DESIGN_BUILDINGS_SYSTEM = 60;
	constexpr int DESIGN_GUARDPOINTS_SYSTEM = 61;
	constexpr int DESIGN_HARVEST_SYSTEM = 62;
	constexpr int DESIGN_ARCHITECTURE_SYSTEM = 63;
	constexpr int DESIGN_MINING_SYSTEM = 64;
	constexpr int DESIGN_STOCKPILES_SYSTEM = 65;
	constexpr int WORKFLOW_UI_SYSTEM = 66;
	constexpr int ITEM_WEAR_SYSTEM = 67;
	constexpr int ITEM_UI_SYSTEM = 68;
	constexpr int JOB_CENTER_SYSTEM = 69;
	constexpr int WISH_MODE_SYSTEM = 70;
	constexpr int AI_FARM_CLEAR = 71;
	constexpr int AI_FARM_FIXSOIL = 72;
	constexpr int AI_FARM_FERTILIZE = 73;
	constexpr int AI_FARM_PLANT = 74;
	constexpr int AI_FARM_WATER = 75;
	constexpr int AI_FARM_WEED = 76;
	constexpr int ARCHITECTURE_SYSTEM = 77;
	constexpr int AI_BUTCHER_SYSTEM = 78;
	constexpr int AI_STOCKPILE_SYSTEM = 79;
	constexpr int AI_DECONSTRUCT_SYSTEM = 80;
	constexpr int BUILDING_INFO_SYS = 81;
	constexpr int HUNGER_SYSTEM = 82;
	constexpr int EAT_SYSTEM = 83;
	constexpr int DRINK_SYSTEM = 84;

    boost::container::flat_map<int, std::pair<int, std::vector<float>>> run_time;
    boost::container::flat_map<int, std::string> system_names;

    template <typename F>
    inline void run_system(F &run_func, const double &duration_ms, const int SYSTEM) {
        const auto start_time = std::chrono::high_resolution_clock::now();
        run_func(duration_ms);
        const auto end_time = std::chrono::high_resolution_clock::now();
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
		system_names[CORPSE_SYSTEM] = "Corpses";
		system_names[MINING_SYSTEM] = "Mining";
		system_names[INVENTORY_SYSTEM] = "Inventory";
		system_names[STOCKPILE_SYSTEM] = "Stockpiles";
		system_names[WORKFLOW_SYSTEM] = "Workflow";
		system_names[VISIBILITY_SCAN_SYSTEM] = "AI Visibility";
		system_names[NEW_ARRIVAL_SYSTEM] = "New Arrivals";
		system_names[AI_SCHEDULER] = "AI Scheduler";
		system_names[AI_LEISURE_TIME] = " AI Leisure";
		system_names[AI_SLEEP_TIME] = "AI Sleep";
		system_names[AI_WORK_TIME] = "AI Work";
		system_names[AI_WORK_LUMBERJACK] = "AI Lumberjack";
		system_names[AI_WORK_MINING] = "AI Mining";
		system_names[AI_WORK_GUARD] = "AI Guarding";
		system_names[AI_WORK_HARVEST] = "AI Harvest";
		system_names[AI_WORK_LEVER_PULL] = "AI Lever Pull";
		system_names[AI_WORK_BUILDING] = "AI Building";
		system_names[AI_WORK_ARCHITECT] = "AI Architect";
		system_names[AI_WORK_ORDER] = "AI Workorders";
		system_names[AI_WORK_HUNT] = "AI Hunting";
		system_names[AI_IDLE] = "AI Idle";
		system_names[TRIGGER_SYSTEM] = "Trigger System";
		system_names[SETTLER_RANGED_ATTACK] = "Settler Ranged";
		system_names[SETTLER_MELEE_ATTACK] = "Settler Melee";
		system_names[CREATURE_ATTACKS] = "Creature Attacks";
		system_names[TURRET_ATTACKS] = "Turret Attacks";
		system_names[DAMAGE_SYSTEM] = "Damage System";
		system_names[KILL_SYSTEM] = "Kill System";
		system_names[HEALING_SYSTEM] = "Healing System";
		system_names[TOPOLOGY_SYSTEM] = "Topology System";
		system_names[VISIBILITY_SYSTEM] = "Visibility System";
		system_names[VEGETATION_SYSTEM] = "Vegetation";
		system_names[DESIGN_LUMBERJACK_SYSTEM] = "Design - Lumberjack";
		system_names[DESIGN_MODE_SYSTEM] = "Design";
		system_names[PARTICLE_SYSTEM] = "Particles";
		system_names[DESIGN_BUILDINGS_SYSTEM] = "Design - Buildings";
		system_names[DESIGN_HARVEST_SYSTEM] = "Design - Harvest";
		system_names[DESIGN_GUARDPOINTS_SYSTEM] = "Design - Guardpoints";
		system_names[DESIGN_ARCHITECTURE_SYSTEM] = "Design - Architecture";
		system_names[DESIGN_MINING_SYSTEM] = "Design - Mining";
		system_names[DESIGN_STOCKPILES_SYSTEM] = "Design - Stockpiles";
		system_names[WORKFLOW_UI_SYSTEM] = "Workflow UI";
		system_names[ITEM_WEAR_SYSTEM] = "Item Wear System";
		system_names[ITEM_UI_SYSTEM] = "Item UI";
		system_names[JOB_CENTER_SYSTEM] = "Job Center UI";
		system_names[WISH_MODE_SYSTEM] = "Wish Mode UI";
		system_names[AI_FARM_CLEAR] = "Farm - Clear";
		system_names[AI_FARM_FERTILIZE] = "Farm - Fertilize";
		system_names[AI_FARM_FIXSOIL] = "Farm - Fix Soil";
		system_names[AI_FARM_PLANT] = "Farm - Plant";
		system_names[AI_FARM_WATER] = "Farm - Water";
		system_names[AI_FARM_WEED] = "Farm - Weed";
		system_names[ARCHITECTURE_SYSTEM] = "Architecture System";
		system_names[AI_BUTCHER_SYSTEM] = "Butcher System";
		system_names[AI_STOCKPILE_SYSTEM] = "AI Stockpiling";
		system_names[AI_DECONSTRUCT_SYSTEM] = "Deconstruction";
		system_names[BUILDING_INFO_SYS] = "Building Info";
		system_names[HUNGER_SYSTEM] = "Hunger System";
		system_names[EAT_SYSTEM] = "Eating System";
		system_names[DRINK_SYSTEM] = "Drinking System";
		game_master_mode = PLAY;
    }

	void run(const double &duration_ms) {

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
		else if (game_master_mode == WORKFLOW) {
			run_system(workflow_ui::run, duration_ms, WORKFLOW_UI_SYSTEM);
		}
		else if (game_master_mode == ITEM_INFO) {
			run_system(item_ui::run, duration_ms, ITEM_UI_SYSTEM);
		}
		else if (game_master_mode == JOB_CENTER_MODE) {
			run_system(job_center_ui::run, duration_ms, JOB_CENTER_SYSTEM);
		}
		else if (game_master_mode == WISHMODE) {
			run_system(wishmode::run, duration_ms, WISH_MODE_SYSTEM);
		}
		else if (game_master_mode == BUILDING_INFO) {
			run_system(building_info::run, duration_ms, WISH_MODE_SYSTEM);
		}

        // Items that only run if the simulation has ticked
        if (major_tick) {
			fluids::copy_to_gpu();
			logging::age_log();
            run_system(calendarsys::run, duration_ms, CALENDAR_SYSTEM);
			run_system(hunger_system::run, duration_ms, HUNGER_SYSTEM);
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
			run_system(corpse_system::run, duration_ms, CORPSE_SYSTEM);
			run_system(mining_system::run, duration_ms, MINING_SYSTEM);
			run_system(architecture_system::run, duration_ms, ARCHITECTURE_SYSTEM);
			run_system(stockpile_system::run, duration_ms, STOCKPILE_SYSTEM);
			run_system(power::run, duration_ms, POWER_SYSTEM);
			run_system(workflow_system::run, duration_ms, WORKFLOW_SYSTEM);
			run_system(ai_status_effects::run, duration_ms, AI_STATUS_SYSTEM);
			run_system(ai_stuck::run, duration_ms, AI_STUCK_SYSTEM);
			run_system(ai_visibility_scan::run, duration_ms, VISIBILITY_SCAN_SYSTEM);
			run_system(ai_new_arrival::run, duration_ms, NEW_ARRIVAL_SYSTEM);
			run_system(ai_scheduler::run, duration_ms, AI_SCHEDULER);
			run_system(ai_leisure_time::run, duration_ms, AI_LEISURE_TIME);
			run_system(ai_sleep_time::run, duration_ms, AI_SLEEP_TIME);
			run_system(ai_work_time::run, duration_ms, AI_WORK_TIME);
			run_system(ai_work_lumberjack::run, duration_ms, AI_WORK_LUMBERJACK);
			run_system(ai_mining::run, duration_ms, AI_WORK_MINING);
			run_system(ai_guard::run, duration_ms, AI_WORK_GUARD);
			run_system(ai_harvest::run, duration_ms, AI_WORK_HARVEST);
			run_system(ai_farm_plant::run, duration_ms, AI_FARM_PLANT);
			run_system(ai_farm_fertilize::run, duration_ms, AI_FARM_FERTILIZE);
			run_system(ai_farm_clear::run, duration_ms, AI_FARM_CLEAR);
			run_system(ai_farm_fixsoil::run, duration_ms, AI_FARM_FIXSOIL);
			run_system(ai_farm_water::run, duration_ms, AI_FARM_WATER);
			run_system(ai_farm_weed::run, duration_ms, AI_FARM_WEED);
			run_system(ai_lever::run, duration_ms, AI_WORK_LEVER_PULL);
			run_system(ai_building::run, duration_ms, AI_WORK_BUILDING);
			run_system(ai_workorder::run, duration_ms, AI_WORK_ORDER);
			run_system(ai_architect::run, duration_ms, AI_WORK_ARCHITECT);
			run_system(ai_hunt::run, duration_ms, AI_WORK_HUNT);
			run_system(ai_butcher::run, duration_ms, AI_BUTCHER_SYSTEM);
			run_system(ai_work_stockpiles::run, duration_ms, AI_STOCKPILE_SYSTEM);
			run_system(ai_deconstruction::run, duration_ms, AI_DECONSTRUCT_SYSTEM);
			run_system(ai_leisure_eat::run, duration_ms, EAT_SYSTEM);
			run_system(ai_leisure_drink::run, duration_ms, DRINK_SYSTEM);
			run_system(ai_idle_time::run, duration_ms, AI_IDLE);
			run_system(movement::run, duration_ms, MOVEMENT_SYSTEM);
			run_system(triggers::run, duration_ms, TRIGGER_SYSTEM);
			run_system(settler_ranged_attack::run, duration_ms, SETTLER_RANGED_ATTACK);
			run_system(settler_melee_attack::run, duration_ms, SETTLER_MELEE_ATTACK);
			run_system(sentient_attacks::run, duration_ms, SENTIENT_ATTACKS);
			run_system(creature_attacks::run, duration_ms, CREATURE_ATTACKS);
			run_system(turret_attacks::run, duration_ms, TURRET_ATTACKS);
			run_system(damage_system::run, duration_ms, DAMAGE_SYSTEM);
			run_system(kill_system::run, duration_ms, KILL_SYSTEM);
			if (hour_elapsed) run_system(healing_system::run, duration_ms, HEALING_SYSTEM);
			run_system(topology::run, duration_ms, TOPOLOGY_SYSTEM);
			run_system(visibility::run, duration_ms, VISIBILITY_SYSTEM);
			run_system(vegetation::run, duration_ms, VEGETATION_SYSTEM);
			if (day_elapsed) run_system(item_wear::run, duration_ms, ITEM_WEAR_SYSTEM);
			fluids::copy_from_gpu();
        }
		run_system(design_mode::run, duration_ms, DESIGN_MODE_SYSTEM);
		if (game_master_mode == DESIGN) {
			if (game_design_mode == CHOPPING) run_system(design_lumberjack::run, duration_ms, DESIGN_LUMBERJACK_SYSTEM);
			if (game_design_mode == BUILDING) run_system(design_buildings::run, duration_ms, DESIGN_BUILDINGS_SYSTEM);
			if (game_design_mode == HARVEST) run_system(design_harvest::run, duration_ms, DESIGN_HARVEST_SYSTEM);
			if (game_design_mode == GUARDPOINTS) run_system(design_guardpoints::run, duration_ms, DESIGN_GUARDPOINTS_SYSTEM);
			if (game_design_mode == ARCHITECTURE) run_system(design_architecture::run, duration_ms, DESIGN_GUARDPOINTS_SYSTEM);
			if (game_design_mode == DIGGING) run_system(design_mining::run, duration_ms, DESIGN_GUARDPOINTS_SYSTEM);
			if (game_design_mode == STOCKPILES) run_system(design_stockpiles::run, duration_ms, DESIGN_GUARDPOINTS_SYSTEM);
		}
		if (game_master_mode == TRIGGER_MANAGEMENT)
		{
			triggers::edit_triggers();
		}
		run_system(inventory_system::run, duration_ms, INVENTORY_SYSTEM);
		run_system(particles::run, duration_ms, PARTICLE_SYSTEM);

		// Logging goes at the end to catch new messages
		run_system(logging::run, duration_ms, LOG_SYSTEM);

        // Profiler
        if (debug::show_profiler) {
            ImGui::Begin("Systems Profile");
            ImGui::Text("Frame time: %f", duration_ms);
            for (auto sys_finder=run_time.begin(); sys_finder != run_time.end(); ++sys_finder) {
                const auto id = sys_finder->first;
                const auto name_finder = system_names.find(id);
                if (name_finder != system_names.end()) {
                    ImGui::PlotLines(name_finder->second.c_str(), static_cast<const float *>(&sys_finder->second.second.at(0)), 100);
                }
            }
            ImGui::End();
        }
    }
}