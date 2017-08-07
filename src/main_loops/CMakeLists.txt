add_library(mainloops
        splash_screen.cpp
        main_menu.cpp
        worldgen.cpp
        play_game.cpp
        )
target_link_libraries(mainloops bengine)
cotire(mainloops)
