#include "health_factory.hpp"
#include "../main/game_globals.hpp"
#include "raws.hpp"

health_t create_health_component_sentient(const std::string &tag, const int base_hp) {
    health_t result;

    result.max_hitpoints = base_hp;
    result.current_hitpoints = base_hp;

    auto finder = species_defs.find(tag);
    if (finder != species_defs.end()) {
        for (const auto &part : finder->second.body_parts) {
            const int n_parts = std::get<1>(part);
            for (int i=0; i<n_parts; ++i) {
                health_part_t p;
                float pct = static_cast<float>(std::get<2>(part)) / 100.0F;
                float hitpoints = static_cast<float>(base_hp) * pct;
                if (hitpoints < 1.0F) hitpoints = 1.0F;
                p.part = std::get<0>(part);
                p.max_hitpoints = hitpoints;
                p.current_hitpoints = hitpoints;
                p.size = std::get<2>(part);
                result.parts.push_back(p);
            }
        }
    } else {
        std::cout << "Warning: could not find species: " << tag << "\n";
    }

    return result;
}

health_t create_health_component_creature(const std::string &tag) {
    health_t result;

    auto finder = creature_defs.find(tag);
    if (finder != creature_defs.end()) {
        int base_hp = rng.roll_dice( finder->second.hp_n, finder->second.hp_dice ) + finder->second.hp_mod;
        if (base_hp < 0) base_hp = 1;

        result.max_hitpoints = base_hp;
        result.current_hitpoints = base_hp;

        for (const auto &part : finder->second.body_parts) {
            const int n_parts = std::get<1>(part);
            for (int i=0; i<n_parts; ++i) {
                health_part_t p;
                float pct = static_cast<float>(std::get<2>(part)) / 100.0F;
                float hitpoints = static_cast<float>(base_hp) * pct;
                if (hitpoints < 1.0F) hitpoints = 1.0F;
                p.part = std::get<0>(part);
                p.max_hitpoints = hitpoints;
                p.current_hitpoints = hitpoints;
                p.size = std::get<2>(part);
                result.parts.push_back(p);
            }
        }
    } else {
        std::cout << "Warning: could not find species: " << tag << "\n";
    }

    return result;
}