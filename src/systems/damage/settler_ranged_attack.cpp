#include "settler_ranged_attack.hpp"
#include "../tasks/civ_dislike.hpp"
#include "../../components/game_stats.hpp"
#include "weapons_helpers.hpp"
#include "../../components/item.hpp"
#include "../../main/game_logger.hpp"

using namespace rltk;

void settler_ranged_attack_system::on_message(const settler_ranged_attack_message &msg) {
    auto attacker = entity(msg.attacker);
    auto defender = entity(msg.victim);
    if (!attacker || !defender) return;
    auto attacker_stats = attacker->component<game_stats_t>();
    auto attacker_pos = attacker->component<position_t>();
    auto defender_pos = defender->component<position_t>();

    civ_dislike_attacker(defender);

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
}
