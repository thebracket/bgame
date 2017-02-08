add_library(work_types STATIC
        building_work.cpp
        mining_work.cpp
        chopping_work.cpp
        reaction_work.cpp
        equip_melee_work.cpp
        equip_ranged_work.cpp
        equip_armor_work.cpp
        hunting_work.cpp
        guard_duty.cpp
        demolition_work.cpp
        hauling_work.cpp
        harvest_work.cpp
        architecture_work.cpp
        lever_work.cpp
        )
target_link_libraries(work_types rltk utils components raws)
cotire(work_types)
