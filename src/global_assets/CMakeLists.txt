add_library(globalassets
        shader_storage.cpp
        texture_storage.cpp
        game_config.cpp
        rng.cpp
        spatial_db.cpp
        game_planet.cpp
        game_camera.cpp
        game_calendar.cpp
        game_designations.cpp
        game_pause.cpp
        )
target_link_libraries(globalassets shaders)
#cotire(globalassets)
