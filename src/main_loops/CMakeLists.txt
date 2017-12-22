add_library(mainloops
        first_run_screen.cpp
        main_menu.cpp
        play_game.cpp
        splash_screen.cpp
        worldgen.cpp
        )
target_link_libraries(mainloops bengine vox systems)
cotire(mainloops)
