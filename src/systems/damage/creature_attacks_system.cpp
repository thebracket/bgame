#include "creature_attacks_system.hpp"
#include "../../bengine/ecs.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../components/species.hpp"
#include "../../raws/creatures.hpp"
#include "../../raws/defs/raw_creature_t.hpp"
#include "../../components/game_stats.hpp"
#include "../helpers/weapons_helper.hpp"
#include "../gui/log_system.hpp"
#include "../../components/logger.hpp"
#include "../../global_assets/rng.hpp"
#include "damage_system.hpp"

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
				auto creaturefinder = get_creature_def(attack_species->tag);
				if (!creaturefinder) {
					std::cout << "Unable to find creature: " << attack_species->tag << "\n";
					return;
				}
				auto creature = *creaturefinder;

				auto defender = entity(msg.victim);
				if (!defender) return;
				auto defender_stats = defender->component<game_stats_t>();

				for (const creature_attack_t &weapon : creature.attacks) {
					LOG ss;
					ss.other_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with its ")->col(color_t{ 1.0f, 1.0f, 0.f })->text(weapon.type)->col(color_t{ 1.0f, 1.0f, 1.0f })->text(". ");
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
					logging::log(logging::log_message{ ss.chars });
				}
			});
		}
	}
}