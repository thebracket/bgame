#include "stdafx.h"
#include "play_game.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../global_assets/game_planet.hpp"
#include "../bengine/telemetry.hpp"
#include "../bengine/filesystem.hpp"
#include "../planet/region/region.hpp"
#include "../render_engine/render_engine.hpp"
#include "../global_assets/game_camera.hpp"
#include "../systems/run_systems.hpp"
#include "../global_assets/game_calendar.hpp"
#include "../global_assets/game_designations.hpp"
#include "../global_assets/game_logger.hpp"
#include "../global_assets/game_mode.hpp"
#include "../global_assets/game_pause.hpp"
#include "../global_assets/game_mining.hpp"
#include "../global_assets/farming_designations.hpp"
#include "../global_assets/building_designations.hpp"
#include "../ascii_engine/ascii_mode.hpp"
#include <atomic>
#include "../global_assets/architecture_designations.hpp"
#include "../global_assets/debug_flags.hpp"
#include "../bengine/IconsFontAwesome.h"
#include <thread>

namespace play_game {

    static std::atomic<bool> loaded(false);
    static std::unique_ptr<std::thread> loader_thread = nullptr;

    static void do_load_game() noexcept {
        using namespace bengine;

        call_home("menu", "playgame");
        // Load the game
		glog(log_target::GAME, log_severity::info, "Loading the planet");
        planet = load_planet();

        // Load the ECS
		glog(log_target::GAME, log_severity::info, "Loading game state");
        {
            std::unique_ptr<std::ifstream> lbfile = std::make_unique<std::ifstream>(save_filename(), std::ios::in | std::ios::binary);
            ecs_load(lbfile);
        }

        // Load the current region - check the camera for the world position
		glog(log_target::GAME, log_severity::info, "Storing important entity handles");

        int region_x, region_y;
        each<world_position_t, calendar_t, designations_t, logger_t, camera_options_t, mining_designations_t, farming_designations_t, building_designations_t, architecture_designations_t>(
			[&region_x, &region_y] (entity_t &entity, world_position_t &pos, calendar_t &cal, designations_t &design,
                                    logger_t &log, camera_options_t &camera_prefs, mining_designations_t &mining,
									farming_designations_t &farming, building_designations_t &building,
									architecture_designations_t &arch) 
		{
            camera_entity = entity.id;
            region_x = pos.world_x;
            region_y = pos.world_y;
            camera_position = &pos;
            calendar = &cal;
            designations = &design;
            logger = &log;
            camera = &camera_prefs;
			mining_designations = &mining;
			farm_designations = &farming;
			building_designations = &building;
			architecture_designations = &arch;
        });

         // Loading the region
		glog(log_target::GAME, log_severity::info, "Loading the region");
        region::load_current_region(region_x, region_y);
        region::tile_recalc_all();
		region::update_outdoor_calculation();

        // Setup systems
		glog(log_target::GAME, log_severity::info, "Setting up systems");
        //add_systems_to_ecs();
        systems::init();

		glog(log_target::GAME, log_severity::info, "Go!");

		game_master_mode = PLAY;
		pause_mode = ONE_STEP;

        loaded = true;
    }

	static constexpr size_t FPS_SLOTS = 256;
	static std::array<float, FPS_SLOTS> fps{};
	static size_t fps_counter = 0;
	static const std::string fps_header = std::string(ICON_FA_CLOCK_O) + std::string(" FPS");

    void tick(const double &duration_ms) noexcept {
        if (!loader_thread) {
            loader_thread = std::make_unique<std::thread>(do_load_game);
        }

        ImGui_ImplGlfwGL3_NewFrame();

        if (!loaded) {
            ImGui::Begin("Please wait - loading");
            ImGui::Text("Play game");
            ImGui::End();
        } else {
            // Game render goes here
			if (camera->ascii_mode) {
				render::ascii_render(duration_ms);
			}
			else {
				render::render_gl(duration_ms);
			}

			if (debug::show_fps) {
				fps[fps_counter] = 1000.0 / duration_ms;
				++fps_counter;
				fps_counter = fps_counter % FPS_SLOTS;
				ImGui::Begin(fps_header.c_str());
				ImGui::Text("Frame time: %f ms, %f FPS", duration_ms, 1000.0 / duration_ms);
				ImGui::PlotLines("FPS", &fps[0], FPS_SLOTS);
				ImGui::End();
			}

            systems::run(duration_ms);
        }

        ImGui::Render();
    }
}
