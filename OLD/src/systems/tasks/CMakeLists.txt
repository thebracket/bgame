add_subdirectory(work_types)
add_library(tasks STATIC
        idle_mode.cpp
        settler_glyph.cpp
        settler_job_status.cpp
        settler_drop_tool.cpp
        settler_cancel_action.cpp
        settler_leisure.cpp
        threat_scanner.cpp
        settler_wander.cpp
        settler_work_time.cpp
        civ_dislike.cpp
        world_queries.cpp
        )
target_link_libraries(tasks work_types rltk utils components raws)
cotire(tasks)
