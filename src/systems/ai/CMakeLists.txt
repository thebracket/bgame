add_library(ai STATIC
        ai_action_shim.cpp
        ai_new_arrival.cpp
        ai_status_effects.cpp
        settler_ai_system.cpp
        workflow_system.cpp
        wildlife_population_system.cpp
        mode_rogue_system.cpp
        sentient_ai_system.cpp
        grazer_ai_system.cpp
        settler_spawner_system.cpp
        mining_system.cpp
        path_finding.cpp
        movement_system.cpp
        distance_map_system.cpp
        inventory_system.cpp
        visibility_system.cpp
        )
target_link_libraries(ai rltk utils components raws)
cotire(ai)
