#include "health_factory.hpp"
#include "../main/game_rng.hpp"
#include "raws.hpp"
#include "creatures.hpp"
#include "species.hpp"

health_t create_health_component_sentient(const raw_species_t * species, const int base_hp)
{
    health_t result;

    result.max_hitpoints = base_hp;
    result.current_hitpoints = base_hp;
    for (const auto &part : species->body_parts) {
        const int n_parts = std::get<1>(part);
        for (int i=0; i<n_parts; ++i) {
            health_part_t p;
            float pct = static_cast<float>(std::get<2>(part)) / 100.0F;
            float hitpoints = static_cast<float>(base_hp) * pct;
            if (hitpoints < 1.0F) hitpoints = 1.0F;
            p.part = std::get<0>(part);
            p.max_hitpoints = (int) hitpoints;
            p.current_hitpoints = (int) hitpoints;
            p.size = std::get<2>(part);
            result.parts.push_back(p);
        }
    }
    return result;
}

health_t create_health_component_settler(const std::string &tag, const int base_hp) {
    health_t result;

    result.max_hitpoints = base_hp;
    result.current_hitpoints = base_hp;

    std::cout << "Finding parts for " << tag << "\n";
    auto species = *get_species_def(tag);
    for (const auto &part : species.body_parts) {
        const int n_parts = std::get<1>(part);
        for (int i=0; i<n_parts; ++i) {
            health_part_t p;
            float pct = static_cast<float>(std::get<2>(part)) / 100.0F;
            float hitpoints = static_cast<float>(base_hp) * pct;
            if (hitpoints < 1.0F) hitpoints = 1.0F;
            p.part = std::get<0>(part);
            p.max_hitpoints = (int)hitpoints;
            p.current_hitpoints = (int)hitpoints;
            p.size = std::get<2>(part);
            result.parts.push_back(p);
        }
    }

    return result;
}

health_t create_health_component_creature(const std::string &tag) {
    health_t result;

    auto species = *get_creature_def(tag);
    int base_hp = rng.roll_dice( species.hp_n, species.hp_dice ) + species.hp_mod;
    if (base_hp < 0) base_hp = 1;

    result.max_hitpoints = base_hp;
    result.current_hitpoints = base_hp;

    for (const auto &part : species.body_parts) {
        const int n_parts = std::get<1>(part);
        for (int i=0; i<n_parts; ++i) {
            health_part_t p;
            float pct = static_cast<float>(std::get<2>(part)) / 100.0F;
            float hitpoints = static_cast<float>(base_hp) * pct;
            if (hitpoints < 1.0F) hitpoints = 1.0F;
            p.part = std::get<0>(part);
            p.max_hitpoints = (int)hitpoints;
            p.current_hitpoints = (int)hitpoints;
            p.size = std::get<2>(part);
            result.parts.push_back(p);
        }
    }

    return result;
}