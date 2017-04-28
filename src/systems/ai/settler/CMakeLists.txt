add_library(settler_ai STATIC
        ai_action_shim.cpp
        ai_idle.cpp
        ai_leisure_time.cpp
        ai_work_lever_pull.cpp
        ai_sleep_time_system.cpp
        ai_new_arrival.cpp
        ai_scheduler.cpp
        ai_status_effects.cpp
        ai_stuck.cpp
        ai_work_architect.cpp
        ai_work_building.cpp
        ai_work_guard.cpp
        ai_work_harvest.cpp
        ai_work_hunt.cpp
        ai_work_lumberjack.cpp
        ai_work_mining.cpp
        ai_work_order.cpp
        ai_work_time.cpp
        ai_visibility_scan.cpp
        job_board.cpp
    )
target_link_libraries(settler_ai rltk utils components raws)
cotire(settler_ai)