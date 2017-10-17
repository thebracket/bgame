#include "turret_ranged_attack_system.hpp"
#include "../../bengine/ecs.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../components/turret_t.hpp"
#include "../../components/position.hpp"
#include "../gui/log_system.hpp"
#include "../../components/logger.hpp"
#include "../../global_assets/rng.hpp"
#include "../helpers/weapons_helper.hpp"

namespace systems {
	namespace turret_attacks {

		using namespace bengine;
		using namespace weapons;

		thread_safe_message_queue<turret_ranged_attack_message> attacks;

		void request_attack(turret_ranged_attack_message msg) {
			attacks.enqueue(turret_ranged_attack_message{ msg.attacker, msg.victim });
		}

		void run(const double &duration_ms) {
			attacks.process_all([](const turret_ranged_attack_message &msg) {
				auto attacker = entity(msg.attacker);
				auto defender = entity(msg.victim);
				auto turret = attacker->component<turret_t>();
				if (!attacker || !defender || !turret) return;

				auto attacker_pos = attacker->component<position_t>();
				auto defender_pos = defender->component<position_t>();

				// Send the bolt on its way
				// TODO: emit(emit_particles_message{ 3, attacker_pos->x, attacker_pos->y, attacker_pos->z, defender_pos->x, defender_pos->y, defender_pos->z });

				LOG ss;
				const int die_roll = rng.roll_dice(1, 20) + turret->hit_bonus;
				const int armor_class = calculate_armor_class(*defender);
				if (die_roll > armor_class) {
					const int damage = std::max(1, rng.roll_dice(turret->damage_dice, turret->damage_dice) + turret->damage_bonus);
					ss.text("The turret hits for " + std::to_string(damage) + " points of damage.");
					// TODO: emit(inflict_damage_message{ msg.victim, damage, "turret" });
				}
				else {
					ss.text("The turret misses.");
				}
				logging::log(logging::log_message{ ss.chars });
			});
		}
	}
}