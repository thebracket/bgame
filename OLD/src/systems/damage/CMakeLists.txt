add_library(damage STATIC
        damage_system.cpp
        weapons_helpers.cpp
        settler_ranged_attack.cpp
        settler_melee_attacks.cpp
        creature_attacks.cpp
        kill_system.cpp
        healing_system.cpp
        sentient_attacks.cpp
        turret_ranged_attack.cpp
        )
target_link_libraries(damage rltk utils components raws)
cotire(damage)
