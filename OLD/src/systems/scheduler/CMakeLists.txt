add_library(scheduler STATIC
        calendar_system.cpp
        corpse_system.cpp
        pause_system.cpp
        initiative_system.cpp
        )
cotire(scheduler)
