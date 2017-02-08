add_library(ai STATIC
        settler_ai_system.cpp
        workflow_system.cpp
        wildlife_population_system.cpp
        mode_rogue_system.cpp
        sentient_ai_system.cpp
        grazer_ai_system.cpp
        )
target_link_libraries(ai rltk utils components raws)
cotire(ai)
