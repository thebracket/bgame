cmake_minimum_required(VERSION 3.5)
project("Nox Futura")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
include(cotire)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED on)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	message("Using ccache")
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR -Wno-deprecated-declarations -O3 -march=core2 -g -fopenmp -ffast-math -funroll-loops -msse2 -mfpmath=sse")
#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}")

#find_package(PkgConfig REQUIRED)

# Require CURL (for telemetry)
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
link_libraries(${CURL_LIBRARIES})

# Require Lua
if(NOT MINGW)
	find_package(Lua 5.1 REQUIRED)
	include_directories(${LUA_INCLUDE_DIR})
	link_libraries(${LUA_LIBRARIES})
	#message("Lua libraries detected (should be both liblua and liblualib): " ${LUA_LIBRARIES} ${LUA_LIBRARY})
endif(NOT MINGW)

# Require GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# Require GLFW
find_package(glfw3 REQUIRED)
#find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})
link_libraries(${GLFW3_LIBRARIES})

# Require OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# Require GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Require Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})
link_libraries(${Boost_system_LIBRARY})
link_libraries(${Boost_filesystem_LIBRARY})

# Cereal
find_package(cereal REQUIRED)
include_directories(${CEREAL_INCLUDE_DIR})

# Zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBRARIES})

#GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})
link_libraries(${GLM_LIBRARIES})

# Include the main src folder, to keep stdafx happy
include_directories(${PROJECT_SOURCE_DIR})

if(MINGW)
add_subdirectory(lua)
include_directories(${PROJECT_SOURCE_DIR}/lua/src)
endif(MINGW)

add_executable(noxfutura
        stdafx.cpp
        main.cpp

		bengine/analytics.cpp
		bengine/color_t.cpp
		bengine/ecs.cpp
		bengine/FastNoise.cpp
		bengine/filesystem.cpp
		bengine/geometry.cpp
		bengine/imgui.cpp
		bengine/imgui_draw.cpp
		bengine/imgui_impl_glfw_gl3.cpp
		bengine/main_window.cpp
		bengine/octree.cpp
		bengine/pcg_basic.cpp
		bengine/random_number_generator.cpp
		bengine/rexspeeder.cpp
		bengine/shaders.cpp
		bengine/simple_sprite.cpp
		bengine/stb_image.cpp
		bengine/string_utils.cpp
		bengine/telemetry.cpp
		bengine/textures.cpp
		bengine/threadpool.cpp

		ascii_engine/ascii_mode.cpp

		components/calendar.cpp
		components/game_stats.cpp
		components/items/item.cpp
		components/logger.cpp
		components/species.cpp

		global_assets/game_building.cpp
		global_assets/game_calendar.cpp
		global_assets/game_camera.cpp
		global_assets/game_config.cpp
		global_assets/game_designations.cpp
		global_assets/game_logger.cpp
		global_assets/game_mode.cpp
		global_assets/game_pause.cpp
		global_assets/game_planet.cpp
		global_assets/rng.cpp
		global_assets/shader_storage.cpp
		global_assets/spatial_db.cpp
		global_assets/texture_storage.cpp

		main_loops/first_run_screen.cpp
		main_loops/main_menu.cpp
		main_loops/play_game.cpp
		main_loops/splash_screen.cpp
		main_loops/worldgen.cpp

		planet/planet.cpp
		planet/planet_builder.cpp

		planet/builder/settler_builder.cpp
		planet/builder/map_builder.cpp
		planet/builder/biome_builder.cpp
		planet/builder/region_builder.cpp
		planet/builder/history_builder.cpp
		planet/builder/sentient_builder.cpp
		planet/builder/river_builder.cpp

		planet/builder/regions/starting_point.cpp
		planet/builder/regions/heightmap.cpp
		planet/builder/regions/water_features.cpp
		planet/builder/regions/strata.cpp
		planet/builder/regions/decorators.cpp
		planet/builder/regions/trees.cpp
		planet/builder/regions/buildings.cpp
		planet/builder/regions/blight_builder.cpp

		planet/region/region.cpp

		raws/biomes.cpp
		raws/buildings_raw.cpp
		raws/clothing.cpp
		raws/creatures.cpp
		raws/health_factory.cpp
		raws/items.cpp
		raws/life_events.cpp
		raws/lua_bridge.cpp
		raws/materials.cpp
		raws/plants.cpp
		raws/profession.cpp
		raws/raws.cpp
		raws/reactions.cpp
		raws/species_raw.cpp
		raws/string_table.cpp

		render_engine/camera.cpp
		render_engine/design_render.cpp
		render_engine/pointlights.cpp
		render_engine/render_engine.cpp
		render_engine/renderbuffers.cpp
		render_engine/ssao.cpp

		render_engine/chunks/chunks.cpp
		render_engine/chunks/cursors.cpp
		render_engine/chunks/geometry_helper.cpp
		render_engine/chunks/water_render.cpp

		render_engine/fbo/base_lit_buffer.cpp
		render_engine/fbo/bloom_ping_pong.cpp
		render_engine/fbo/buffertest.cpp
		render_engine/fbo/gbuffer.cpp
		render_engine/fbo/hdr_buffer.cpp
		render_engine/fbo/point_light_buffer.cpp

		render_engine/shaders/chunk_shader.cpp
		render_engine/shaders/lightstage_shader.cpp
		render_engine/shaders/voxel_shader.cpp
		render_engine/shaders/voxel_shadow_shader.cpp

		render_engine/vox/voxreader.cpp
		render_engine/vox/voxel_model.cpp
		render_engine/vox/renderables.cpp

		systems/run_systems.cpp
		systems/keydamper.cpp
		systems/mouse.cpp

		systems/ai/ai_status_effects.cpp
		systems/ai/ai_stuck.cpp
		systems/ai/distance_map_system.cpp
		systems/ai/inventory_system.cpp
		systems/ai/mining_system.cpp
		systems/ai/sentient_ai_system.cpp
		systems/ai/stockpile_system.cpp
		systems/ai/wildlife_population.cpp
		systems/ai/workflow_system.cpp

		systems/ai/settler/ai_idle_time.cpp
		systems/ai/settler/ai_leisure_time.cpp
		systems/ai/settler/ai_new_arrival.cpp
		systems/ai/settler/ai_scheduler.cpp
		systems/ai/settler/ai_sleep_time.cpp
		systems/ai/settler/ai_visibility_scan.cpp
		systems/ai/settler/ai_work_architect.cpp
		systems/ai/settler/ai_work_building.cpp
		systems/ai/settler/ai_work_butcher.cpp
		systems/ai/settler/ai_work_guard.cpp
		systems/ai/settler/ai_work_harvest.cpp
		systems/ai/settler/ai_work_hunt.cpp
		systems/ai/settler/ai_work_lever_pull.cpp
		systems/ai/settler/ai_work_lumberjack.cpp
		systems/ai/settler/ai_work_mining.cpp
		systems/ai/settler/ai_work_order.cpp
		systems/ai/settler/ai_work_time.cpp
		systems/ai/settler/jobs_board.cpp

		systems/damage/creature_attacks_system.cpp
		systems/damage/damage_system.cpp
		systems/damage/healing_system.cpp
		systems/damage/kill_system.cpp
		systems/damage/sentient_attacks_system.cpp
		systems/damage/settler_melee_attacks_system.cpp
		systems/damage/settler_ranged_attack_system.cpp
		systems/damage/turret_ranged_attack_system.cpp

		systems/gui/civ_ui_system.cpp
		systems/gui/design_architecture.cpp
		systems/gui/design_buildings.cpp
		systems/gui/design_guardpoints.cpp
		systems/gui/design_harvest.cpp
		systems/gui/design_lumberjack.cpp
		systems/gui/design_mining.cpp
		systems/gui/design_mode.cpp
		systems/gui/design_stockpiles.cpp
		systems/gui/hud_system.cpp
		systems/gui/log_system.cpp
		systems/gui/particle_system.cpp
		systems/gui/settler_info_system.cpp
		systems/gui/tooltip_system.cpp
		systems/gui/units_info_system.cpp
		systems/gui/workflow_ui_system.cpp

		systems/helpers/dijkstra_map.cpp
		systems/helpers/inventory_assistant.cpp
		systems/helpers/pathfinding.cpp
		systems/helpers/weapons_helper.cpp
		systems/helpers/workflow_assistant.cpp

		systems/io/camera_system.cpp

		systems/overworld/settler_spawner_system.cpp
		systems/overworld/world_system.cpp

		systems/physics/door_system.cpp
		systems/physics/explosive_system.cpp
		systems/physics/fluid_system.cpp
		systems/physics/gravity_system.cpp
		systems/physics/movement_system.cpp
		systems/physics/topology_system.cpp
		systems/physics/trigger_system.cpp
		systems/physics/vegetation_system.cpp
		systems/physics/visibility_system.cpp

		systems/power/power_system.cpp

		systems/scheduler/calendar_system.cpp
		systems/scheduler/corpse_system.cpp
		systems/scheduler/initiative_system.cpp
		systems/scheduler/tick_system.cpp
)
#target_link_libraries(noxfutura ${LUA_LIBRARIES})
#target_link_libraries(noxfutura liblua)
target_link_libraries(noxfutura ${Boost_LIBRARIES})
target_link_libraries(noxfutura glfw)
cotire(noxfutura)
