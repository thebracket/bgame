#include "stdafx.h"
#include "creature_attacks_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../raws/creatures.hpp"
#include "../../raws/defs/raw_creature_t.hpp"
#include "../helpers/weapons_helper.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/rng.hpp"
#include "damage_system.hpp"
#include "../gui/particle_system.hpp"
#include "../../utils/system_log.hpp"

namespace systems {
	namespace creature_attacks {

		using namespace bengine;
		using namespace weapons;

		thread_safe_message_queue<creature_attack_message> attacks;

		void request_attack(creature_attack_message msg) {
			attacks.enqueue(creature_attack_message{ msg.attacker, msg.victim });
		}

		void run(const double &duration_ms) {
			attacks.process_all([](const creature_attack_message &msg) {
				// Start by determining the attack
				auto attacker = entity(msg.attacker);
				if (!attacker) return;
				auto attack_species = attacker->component<species_t>();
				if (!attack_species) {
					glog(log_target::GAME, log_severity::warning, "WARNING: Attacker has no species");
					return;
				}
				auto creaturefinder = get_creature_def(attack_species->tag);
				if (!creaturefinder) {
					glog(log_target::GAME, log_severity::warning, "Unable to find creature: {0}", attack_species->tag);
					return;
				}
				auto creature = *creaturefinder;

				auto defender = entity(msg.victim);
				if (!defender) return;

				auto attacker_pos = entity(msg.attacker)->component<position_t>();
				auto defender_pos = entity(msg.victim)->component<position_t>();
				if (!attacker_pos || !defender_pos) return;
				color_t red{ 1.0f, 0.0f, 0.0f };
				particles::melee_attack(*attacker_pos, *defender_pos, red);

				auto defender_stats = defender->component<game_stats_t>();

				for (const creature_attack_t &weapon : creature.attacks) {
					LOG ss;
					color_t yellow{ 1.0f, 1.0f, 0.f };
					color_t white{ 1.0f, 1.0f, 1.0f };
					ss.other_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with its ")->col(yellow)->text(weapon.type)->col(white)->text(". ");
					const int hit_roll = rng.roll_dice(1, 20) + weapon.hit_bonus;
					const int target = calculate_armor_class(*defender) + stat_modifier(defender_stats->dexterity);
					if (hit_roll < target) {
						ss.text("The attack misses.");
					}
					else {
						const int damage = std::max(1, rng.roll_dice(weapon.damage_n_dice, weapon.damage_dice) + weapon.damage_mod);
						ss.text(std::string("The attack hits, for ") + std::to_string(damage) + std::string(" points of damage."));
						damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.victim, damage, weapon.type });
					}
					logging::log_message lmsg{ ss.chars };
					logging::log(lmsg);
				}
			});
		}
	}
}
