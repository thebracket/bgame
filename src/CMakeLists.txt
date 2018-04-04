cmake_minimum_required(VERSION 3.5)
project("Nox Futura")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)
include(cotire)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR -Wno-deprecated-declarations -O3 -march=core2 -g -fopenmp -ffast-math -funroll-loops -msse2 -mfpmath=sse")
if (APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -all_load")
endif (APPLE)

#find_package(PkgConfig REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message("Using ccache")
endif(CCACHE_FOUND)

# Require CURL (for telemetry)
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# Require GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Require GLFW
find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})

# Require OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

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

#GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

#Lua
find_package(Lua 5.3 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
message(${LUA_INCLUDE_DIR})

#FMT
find_package(FMT 4.1 REQUIRED)
if (${FMT_FOUND})
    include_directories(${FMT_INCLUDE_DIR})
    link_libraries(${FMT_LIBRARIES})
    message("Fmt found" , ${FMT_LIBRARIES})
endif (${FMT_FOUND})

# Include the main src folder, to keep stdafx happy
include_directories(${PROJECT_SOURCE_DIR})

add_library(bengine STATIC
        bengine/analytics.cpp
        bengine/color_t.cpp
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
        bengine/stb_image_resize.cpp
        bengine/string_utils.cpp
        bengine/telemetry.cpp
        bengine/textures.cpp
        )

add_library(render STATIC
        ascii_engine/ascii_mode.cpp

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
        render_engine/fbo/buffertest.cpp
        render_engine/fbo/gbuffer.cpp
        render_engine/fbo/hdr_buffer.cpp
        render_engine/fbo/point_light_buffer.cpp

        render_engine/shaders/ascii_base_shader.cpp
        render_engine/shaders/ascii_light_shader.cpp
        render_engine/shaders/chunk_shader.cpp
        render_engine/shaders/chunk_depth_shader.cpp
        render_engine/shaders/cursor_shader.cpp
        render_engine/shaders/lighter_shader.cpp
        render_engine/shaders/lightstage_shader.cpp
        render_engine/shaders/particle_shader.cpp
        render_engine/shaders/pointlight_shader.cpp
        render_engine/shaders/skylight_shader.cpp
        render_engine/shaders/tonemap_shader.cpp
        render_engine/shaders/voxel_shader.cpp
        render_engine/shaders/voxel_shadow_shader.cpp
        render_engine/shaders/worldgen_shader.cpp

        render_engine/ubo/first_stage_ubo.cpp
        render_engine/ubo/lighting_ubo.cpp

        render_engine/vox/voxreader.cpp
        render_engine/vox/voxel_model.cpp
        render_engine/vox/renderables.cpp
        )

add_library(components STATIC
        components/calendar.cpp
        components/game_stats.cpp
        components/items/item.cpp
        components/logger.cpp
        components/species.cpp
        )

add_library(globals STATIC
        global_assets/architecture_designations.cpp
        global_assets/building_designations.cpp
        global_assets/debug_flags.cpp
        global_assets/farming_designations.cpp
        global_assets/game_building.cpp
        global_assets/game_calendar.cpp
        global_assets/game_camera.cpp
        global_assets/game_config.cpp
        global_assets/game_designations.cpp
        global_assets/game_ecs.cpp
        global_assets/game_logger.cpp
        global_assets/game_mining.cpp
        global_assets/game_mode.cpp
        global_assets/game_pause.cpp
        global_assets/game_planet.cpp
        global_assets/rng.cpp
        global_assets/shader_storage.cpp
        global_assets/spatial_db.cpp
        global_assets/texture_storage.cpp
        )

add_library(mainloops STATIC
        main_loops/first_run_screen.cpp
        main_loops/main_menu.cpp
        main_loops/play_game.cpp
        main_loops/splash_screen.cpp
        main_loops/worldgen.cpp

        )

add_library(planet STATIC
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
        planet/builder/regions/game_objects.cpp

        planet/region/region.cpp
        )

add_library(luabridge STATIC
        raws/lua_bridge.cpp
        )

add_library(raws STATIC
        raws/biomes.cpp
        raws/buildings_raw.cpp
        raws/clothing.cpp
        raws/creatures.cpp
        raws/health_factory.cpp
        raws/items.cpp
        raws/life_events.cpp
        raws/materials.cpp
        raws/plants.cpp
        raws/profession.cpp
        raws/raws.cpp
        raws/reactions.cpp
        raws/species_raw.cpp
        raws/string_table.cpp
        )

add_library(systems STATIC
        systems/run_systems.cpp
        systems/keydamper.cpp
        systems/mouse.cpp

        systems/io/camera_system.cpp

        systems/overworld/settler_spawner_system.cpp
        systems/overworld/world_system.cpp

        systems/power/power_system.cpp

        systems/scheduler/calendar_system.cpp
        systems/scheduler/corpse_system.cpp
        systems/scheduler/initiative_system.cpp
        systems/scheduler/tick_system.cpp
        )

add_library(systemshelpers STATIC
        systems/helpers/dijkstra_map.cpp
        systems/helpers/inventory_assistant.cpp
        systems/helpers/pathfinding.cpp
        systems/helpers/weapons_helper.cpp
        systems/helpers/workflow_assistant.cpp
        )

add_library(systemsphysics STATIC
        systems/physics/door_system.cpp
        systems/physics/explosive_system.cpp
        systems/physics/fluid_system.cpp
        systems/physics/gravity_system.cpp
        systems/physics/movement_system.cpp
        systems/physics/topology_system.cpp
        systems/physics/trigger_system.cpp
        systems/physics/vegetation_system.cpp
        systems/physics/visibility_system.cpp
        systems/physics/item_wear_system.cpp
        )

add_library(systemsai STATIC
        systems/ai/ai_status_effects.cpp
        systems/ai/ai_stuck.cpp
        systems/ai/architecture_system.cpp
        systems/ai/distance_map_system.cpp
        systems/ai/inventory_system.cpp
        systems/ai/mining_system.cpp
        systems/ai/sentient_ai_system.cpp
        systems/ai/stockpile_system.cpp
        systems/ai/wildlife_population.cpp
        systems/ai/workflow_system.cpp
        )

add_library(systemsaisettler STATIC
        systems/ai/settler/ai_idle_time.cpp
        systems/ai/settler/ai_deconstruct.cpp
        systems/ai/settler/ai_leisure_time.cpp
        systems/ai/settler/ai_new_arrival.cpp
        systems/ai/settler/ai_scheduler.cpp
        systems/ai/settler/ai_sleep_time.cpp
        systems/ai/settler/ai_visibility_scan.cpp
        systems/ai/settler/ai_work_architect.cpp
        systems/ai/settler/ai_work_building.cpp
        systems/ai/settler/ai_work_butcher.cpp
        systems/ai/settler/ai_work_farm_clear.cpp
        systems/ai/settler/ai_work_farm_fertilize.cpp
        systems/ai/settler/ai_work_farm_fixsoil.cpp
        systems/ai/settler/ai_work_farm_plant.cpp
        systems/ai/settler/ai_work_farm_water.cpp
        systems/ai/settler/ai_work_farm_weed.cpp
        systems/ai/settler/ai_work_guard.cpp
        systems/ai/settler/ai_work_harvest.cpp
        systems/ai/settler/ai_work_hunt.cpp
        systems/ai/settler/ai_work_lever_pull.cpp
        systems/ai/settler/ai_work_lumberjack.cpp
        systems/ai/settler/ai_work_mining.cpp
        systems/ai/settler/ai_work_order.cpp
        systems/ai/settler/ai_work_time.cpp
        systems/ai/settler/ai_work_stockpile.cpp
        systems/ai/settler/jobs_board.cpp
        )
add_library(systemsdamage STATIC
        systems/damage/creature_attacks_system.cpp
        systems/damage/damage_system.cpp
        systems/damage/healing_system.cpp
        systems/damage/kill_system.cpp
        systems/damage/sentient_attacks_system.cpp
        systems/damage/settler_melee_attacks_system.cpp
        systems/damage/settler_ranged_attack_system.cpp
        systems/damage/turret_ranged_attack_system.cpp

        )
add_library(systemsgui STATIC
        systems/gui/circuit_graph.cpp
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
        systems/gui/item_info_system.cpp
        systems/gui/job_center_ui.cpp
        systems/gui/log_system.cpp
        systems/gui/particle_system.cpp
        systems/gui/settler_info_system.cpp
        systems/gui/tooltip_system.cpp
        systems/gui/units_info_system.cpp
        systems/gui/workflow_ui_system.cpp
        systems/gui/wish_mode.cpp
        systems/gui/building_info.cpp
        )

add_executable(noxfutura
        main.cpp
        utils/system_log.cpp
        )


target_link_libraries(globals bengine)
target_link_libraries(raws bengine components luabridge)
target_link_libraries(render planet bengine)
target_link_libraries(systemsdamage systemsai)
target_link_libraries(systemsgui globals bengine components raws render systemsai planet systems)
target_link_libraries(planet components)
target_link_libraries(systemsphysics systemshelpers systemsgui)
target_link_libraries(globals bengine render)

target_link_libraries(noxfutura
        bengine
        render
        components
        globals
        planet
        raws
        mainloops
        systems
        systemsai
        systemsaisettler
        systemsdamage
        systemsgui
        systemsphysics
        systemshelpers
        )
target_link_libraries(noxfutura ${CURL_LIBRARIES})
target_link_libraries(noxfutura ${GLEW_LIBRARIES})
target_link_libraries(noxfutura ${GLFW3_LIBRARIES})
target_link_libraries(noxfutura ${OPENGL_LIBRARIES})
target_link_libraries(noxfutura ${Boost_LIBRARIES})
target_link_libraries(noxfutura ${ZLIB_LIBRARIES})
target_link_libraries(noxfutura ${GLM_LIBRARIES})
target_link_libraries(noxfutura glfw)
target_link_libraries(raws ${LUA_LIBRARIES})
target_link_libraries(planet ${ZLIB_LIBRARIES})
target_link_libraries(noxfutura ${FMT_LIBRARIES})
target_link_libraries(components ${FMT_LIBRARIES})
target_link_libraries(bengine ${GLEW_LIBRARIES})
target_link_libraries(render ${GLEW_LIBRARIES})
target_link_libraries(render ${GLFW3_LIBRARIES})
target_link_libraries(render ${OPENGL_LIBRARIES})
cotire(noxfutura)
cotire(bengine)
cotire(render)
cotire(components)
cotire(globals)
cotire(planet)
cotire(raws)
cotire(mainloops)
cotire(systems)
cotire(systemsai)
cotire(systemsdamage)
cotire(systemsgui)
##cotire(systemsaisettler)

## Install targets
install(TARGETS noxfutura DESTINATION /opt/noxfutura)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/../world_defs DESTINATION /opt/noxfutura/world_defs)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/../game_assets DESTINATION /opt/noxfutura/game_assets)
install(FILES ${PROJECT_SOURCE_DIR}/../README.md DESTINATION /opt/noxfutura)
install(FILES ${PROJECT_SOURCE_DIR}/../LICENSE DESTINATION /opt/noxfutura)

## Packaging with CPack
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/../LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
include(CPack)

