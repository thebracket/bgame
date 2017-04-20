#include "settler_melee_attacks.hpp"
#include "../../messages/messages.hpp"
#include "../tasks/civ_dislike.hpp"
#include "../../components/game_stats.hpp"
#include "weapons_helpers.hpp"
#include "../../components/item.hpp"
#include "../../main/game_rng.hpp"
#include "../../main/game_logger.hpp"

using namespace rltk;
void settler_melee_attacks_system::on_message(const settler_attack_message &msg) {
    auto attacker = entity(msg.attacker);
    auto defender = entity(msg.victim);
    if (!attacker || !defender) return;
    auto attacker_stats = attacker->component<game_stats_t>();

    civ_dislike_attacker(defender);

    std::size_t weapon_id = get_melee_id(msg.attacker);
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
                weapon_n = weapon_finder->second.damage_n;
                weapon_d = weapon_finder->second.damage_d;
                weapon_mod = weapon_finder->second.damage_mod + get_material(weapon_component->material)->damage_bonus;;
            }
        }
    }

    LOG ss;
    ss.settler_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with their ")->col(rltk::colors::YELLOW)->text(weapon_name + std::string(". "))->col(rltk::colors::WHITE);
    const int skill_modifier = get_skill_modifier(*attacker_stats, "Melee Attacks");
    const int die_roll = rng.roll_dice(1, 20) + stat_modifier(attacker_stats->strength) + skill_modifier;
    const int armor_class = calculate_armor_class(*defender);
    if (die_roll > armor_class) {
        const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod + stat_modifier(attacker_stats->strength) + skill_modifier);
        emit(inflict_damage_message{ msg.victim, damage, weapon_name });
        ss.text(std::string("The attack hits, for ")+std::to_string(damage)+std::string("."));
        gain_skill_from_success(msg.attacker, *attacker_stats, "Melee Attacks", armor_class, rng);
    } else {
        ss.text("The attack misses.");
    }

    emit_deferred(log_message{ss.chars});
}
