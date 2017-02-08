add_library(gui STATIC
        panel_render_system.cpp
        mode_units_system.cpp
        mode_rogue_render_system.cpp
        mode_settler_info_system.cpp
        mode_play_system.cpp
        mode_design_system.cpp
        mode_standing_orders.cpp
        mode_sentient_info_system.cpp
        particle_system.cpp
        game_over_system.cpp
        mode_civs_system.cpp
        log_system.cpp
        gui_system.cpp
        hud_system.cpp
        gui_elements.cpp
        wish_system.cpp

        )
target_link_libraries(gui rltk utils components raws)
cotire(gui)
