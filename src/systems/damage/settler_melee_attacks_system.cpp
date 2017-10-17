#include "settler_melee_attacks_system.hpp"
#include "../../bengine/ecs.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../components/game_stats.hpp"
#include "../helpers/weapons_helper.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../components/item.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../components/logger.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/rng.hpp"

namespace systems {
	namespace settler_melee_attack {

		using namespace bengine;
		using namespace weapons;

		thread_safe_message_queue<settler_attack_message> melee;

		void request_melee(settler_attack_message msg) {
			melee.enqueue(settler_attack_message{ msg.attacker, msg.victim });
		}

		void run(const double &duration_ms) {
			melee.process_all([](const settler_attack_message &msg) {
				auto attacker = entity(msg.attacker);
				auto defender = entity(msg.victim);
				if (!attacker || !defender) return;
				auto attacker_stats = attacker->component<game_stats_t>();

				// TODO: civ_dislike_attacker(defender);

				std::size_t weapon_id = get_melee_id(msg.attacker);
				std::string weapon_name = "fists";
				int weapon_n = 1;
				int weapon_d = 4;
				int weapon_mod = 0;
				if (weapon_id != 0) {
					auto weapon_component = entity(weapon_id)->component<item_t>();
					if (weapon_component) {
						auto weapon_finder = get_item_def(weapon_component->item_tag);
						if (weapon_finder != nullptr) {
							weapon_name = weapon_finder->name;
							weapon_n = weapon_finder->damage_n;
							weapon_d = weapon_finder->damage_d;
							weapon_mod = weapon_finder->damage_mod + get_material(weapon_component->material)->damage_bonus;;
						}
					}
				}

				LOG ss;
				ss.settler_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with their ")->col(color_t{ 1.0f,0.0f,0.f })->text(weapon_name + std::string(". "))->col(color_t{ 1.0f,1.0f,1.0f });
				const int skill_modifier = get_skill_modifier(*attacker_stats, "Melee Attacks");
				const int die_roll = rng.roll_dice(1, 20) + stat_modifier(attacker_stats->strength) + skill_modifier;
				const int armor_class = calculate_armor_class(*defender);
				if (die_roll > armor_class) {
					const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod + stat_modifier(attacker_stats->strength) + skill_modifier);
					// TODO: emit(inflict_damage_message{ msg.victim, damage, weapon_name });
					ss.text(std::string("The attack hits, for ") + std::to_string(damage) + std::string("."));
					gain_skill_from_success(msg.attacker, *attacker_stats, "Melee Attacks", armor_class, rng);
				}
				else {
					ss.text("The attack misses.");
				}

				logging::log(logging::log_message{ ss.chars });
			});
		}
	}
}