#include "sentient_attacks.hpp"
#include "../../messages/inflict_damage_message.hpp"
#include "weapons_helpers.hpp"
#include "../../main/game_globals.hpp"
#include "../../raws/species.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../messages/log_message.hpp"
#include "../../raws/items.hpp"
#include "../../components/item.hpp"
#include "../../components/game_stats.hpp"
#include "../tasks/civ_dislike.hpp"
#include "../../messages/emit_particles_message.hpp"
#include "../../components/natural_attacks_t.hpp"

void sentient_melee_attack(const std::string &weapon_name,
                           const int &hit_bonus,
                           const int &weapon_n,
                           const int &weapon_d,
                           const int &weapon_mod,
                           const sentient_attack_message &msg) {

    LOG ss;
    ss.other_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with their ")->col(rltk::colors::YELLOW)->text(weapon_name + std::string(". "))->col(rltk::colors::WHITE);

    auto defender = entity(msg.victim);
    if (!defender) return;

    const int die_roll = rng.roll_dice(1, 20) + hit_bonus;
    const int armor_class = calculate_armor_class(*defender);

    if (die_roll > armor_class) {
        const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod);
        emit(inflict_damage_message{ msg.victim, damage, weapon_name });
        ss.text(std::string("The attack hits, for ")+std::to_string(damage)+std::string("."));
    } else {
        ss.text("The attack misses.");
    }

    emit_deferred(log_message{ss.chars});
}

void sentient_attacks_system::configure() {
    system_name = "Sentient Attacks System";
    subscribe_mbox<sentient_attack_message>();
    subscribe_mbox<sentient_ranged_attack_message>();
}

void sentient_attacks_system::update(const double duration_ms) {
    each_mbox<sentient_attack_message>([] (const sentient_attack_message &msg) {

        auto sentient_entity = entity(msg.attacker);
        if (!sentient_entity) return;
        auto ai = sentient_entity->component<sentient_ai>();
        if (!ai) return;

        std::size_t weapon_id = get_melee_id(msg.attacker);
        if (weapon_id == 0) {
            // Natural attacks
            auto * na = sentient_entity->component<natural_attacks_t>();
            if (na) {
                for (const auto &a : na->attacks) {
                    if (a.range == 0) {
                        sentient_melee_attack(a.type, a.hit_bonus, a.n_dice, a.die_type, a.die_mod, msg);
                    }
                }
            }
        } else {
            auto weapon_component = entity(weapon_id)->component<item_t>();
            if (weapon_component) {
                auto weapon_finder = item_defs.find(weapon_component->item_tag);
                if (weapon_finder != item_defs.end()) {
                    sentient_melee_attack(weapon_finder->second.name, 0,
                                          weapon_finder->second.damage_n, weapon_finder->second.damage_d,
                                          weapon_finder->second.damage_mod + get_material(weapon_component->material)->damage_bonus,
                                          msg);
                }
            }
        }
    });

    each_mbox<sentient_ranged_attack_message>([] (const sentient_ranged_attack_message &msg) {

        auto attacker = entity(msg.attacker);
        auto defender = entity(msg.victim);
        if (!attacker || !defender) return;
        auto attacker_stats = attacker->component<game_stats_t>();
        auto attacker_pos = attacker->component<position_t>();
        auto defender_pos = defender->component<position_t>();

        civ_dislike_attacker(defender);

        // Held Weapons
        std::size_t weapon_id, ammo_id;
        std::tie(weapon_id, ammo_id) = get_ranged_and_ammo_id(msg.attacker);
        std::string weapon_name = "fists";
        int weapon_n = 1;
        int weapon_d = 4;
        int weapon_mod = 0;
        if (weapon_id != 0) {
            auto weapon_component = entity(weapon_id)->component<item_t>();
            if (weapon_component) {
                auto weapon_finder = item_defs.find(weapon_component->item_tag);
                if (weapon_finder != item_defs.end()) {
                    weapon_name = weapon_finder->second.name;
                }
            }
        }
        if (ammo_id != 0) {
            auto ammo_component = entity(ammo_id)->component<item_t>();
            if (ammo_component) {
                auto ammo_finder = item_defs.find(ammo_component->item_tag);
                if (ammo_finder != item_defs.end()) {
                    weapon_n = ammo_finder->second.damage_n;
                    weapon_d = ammo_finder->second.damage_d + get_material(ammo_component->material)->damage_bonus;
                    weapon_mod = ammo_finder->second.damage_mod;
                }
                --ammo_component->stack_size;
                if (ammo_component->stack_size < 1) delete_entity(ammo_id);
            }
            emit(emit_particles_message{3, attacker_pos->x, attacker_pos->y, attacker_pos->z,
                                        defender_pos->x, defender_pos->y, defender_pos->z});
        }

        // Natural weapons
        auto na = attacker->component<natural_attacks_t>();
        if (na) {
            for (const auto &a : na->attacks) {
                if (a.range > 0) {
                    weapon_n = a.n_dice;
                    weapon_d = a.die_type;
                    weapon_mod = a.die_mod;
                    emit(emit_particles_message{3, attacker_pos->x, attacker_pos->y, attacker_pos->z,
                                                defender_pos->x, defender_pos->y, defender_pos->z});
                }
            }
        }

        LOG ss;
        ss.settler_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with their ")->col(rltk::colors::YELLOW)->text(weapon_name+std::string(". "))->col(rltk::colors::WHITE);
        const int skill_modifier = get_skill_modifier(*attacker_stats, "Ranged Attacks");
        const int die_roll = rng.roll_dice(1, 20) + stat_modifier(attacker_stats->dexterity + skill_modifier);
        const int armor_class = calculate_armor_class(*defender);
        if (die_roll > armor_class) {
            const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod + stat_modifier(attacker_stats->strength) + skill_modifier);
            ss.text("The attack hits for "+std::to_string(damage)+" points of damage.");
            emit(inflict_damage_message{ msg.victim, damage, weapon_name });
            gain_skill_from_success(msg.attacker, *attacker_stats, "Ranged Attacks", armor_class, rng);
        } else {
            ss.text("The attack misses.");
        }
        emit_deferred(log_message{ss.chars});
    });
}