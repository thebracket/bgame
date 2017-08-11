add_library(globalassets STATIC
        shader_storage.cpp
        texture_storage.cpp
        game_config.cpp
        rng.cpp
        spatial_db.cpp
        game_planet.cpp
        game_camera.cpp
        )
cotire(globalassets)
