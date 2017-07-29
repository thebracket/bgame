#include "turret_ranged_attack.hpp"
#include "../tasks/civ_dislike.hpp"
#include "../../components/turret_t.hpp"
#include "weapons_helpers.hpp"
#include "../../components/item.hpp"
#include "../../main/game_logger.hpp"
#include "../../main/game_rng.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/defs/material_def_t.hpp"

using namespace rltk;

void turret_ranged_attack_system::on_message(const turret_ranged_attack_message &msg) {
    auto attacker = entity(msg.attacker);
    auto defender = entity(msg.victim);
    auto turret = attacker->component<turret_t>();
    if (!attacker || !defender || !turret) return;

    auto attacker_pos = attacker->component<position_t>();
    auto defender_pos = defender->component<position_t>();

    // Send the bolt on its way
    emit(emit_particles_message{3, attacker_pos->x, attacker_pos->y, attacker_pos->z,
                                defender_pos->x, defender_pos->y, defender_pos->z});

    LOG ss;
    const int die_roll = rng.roll_dice(1, 20) + turret->hit_bonus;
    const int armor_class = calculate_armor_class(*defender);
    if (die_roll > armor_class) {
        const int damage = std::max(1, rng.roll_dice(turret->damage_dice, turret->damage_dice) + turret->damage_bonus);
        ss.text("The turret hits for "+std::to_string(damage)+" points of damage.");
        emit(inflict_damage_message{ msg.victim, damage, "turret" });
    } else {
        ss.text("The turret misses.");
    }
    emit_deferred(log_message{ss.chars});
}
