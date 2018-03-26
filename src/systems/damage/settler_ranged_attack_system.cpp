#include "stdafx.h"
#include "settler_ranged_attack_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../helpers/weapons_helper.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/rng.hpp"
#include "damage_system.hpp"
#include "../gui/particle_system.hpp"
#include "../helpers/inventory_assistant.hpp"

namespace systems {
	namespace settler_ranged_attack {

		using namespace bengine;
		using namespace weapons;
		using namespace logging;

		thread_safe_message_queue<settler_ranged_attack_message> attacks;

		void request_settler_ranged_attack(settler_ranged_attack_message msg) {
			attacks.enqueue(settler_ranged_attack_message{ msg.attacker, msg.victim });
		}

		void run(const double &duration_ms) {
			attacks.process_all([](settler_ranged_attack_message &msg) {
				auto attacker = entity(msg.attacker);
				auto defender = entity(msg.victim);
				if (!attacker || !defender) return;
				auto attacker_stats = attacker->component<game_stats_t>();
				auto attacker_pos = attacker->component<position_t>();
				auto defender_pos = defender->component<position_t>();
				if (!attacker_pos || !defender_pos) return;
				color_t magenta{ 1.0f, 0.0f, 1.0f };
				particles::ranged_attack(*attacker_pos, *defender_pos, magenta);

				// TODO: civ_dislike_attacker(defender);

				auto [weapon_id, ammo_id] = get_ranged_and_ammo_id(*attacker);
				std::string weapon_name = "fists";
				int weapon_n = 1;
				int weapon_d = 4;
				int weapon_mod = 0;
				int weapon_quality = 0;
				if (weapon_id != 0) {
					auto weapon_component = entity(weapon_id)->component<item_t>();
					if (weapon_component) {
						auto weapon_finder = get_item_def(weapon_component->item_tag);
						if (weapon_finder != nullptr) {
							weapon_name = weapon_finder->name;
						}
						auto q = entity(weapon_id)->component<item_quality_t>();
						if (q && q->quality > 3) ++weapon_quality;
						if (q && q->quality == 7) ++weapon_quality;
						auto wear = entity(weapon_id)->component<item_wear_t>();
						if (wear) {
							if (wear->wear > 0) {
								--wear->wear;
							}
							else {
								LOG logger;
								logger.text(weapon_name + std::string(" has broken!"));
								log_message lmsg{ logger.chars };
								logging::log( lmsg );
								inventory::delete_item(weapon_id);
							}
						}
					}
				}
				if (ammo_id != 0) {
					auto ammo_component = entity(ammo_id)->component<item_t>();
					if (ammo_component) {
						auto ammo_finder = get_item_def(ammo_component->item_tag);
						if (ammo_finder != nullptr) {
							weapon_n = ammo_finder->damage_n;
							weapon_d = ammo_finder->damage_d + get_material(ammo_component->material)->damage_bonus;
							weapon_mod = ammo_finder->damage_mod;
						}
						--ammo_component->stack_size;
						if (ammo_component->stack_size < 1) delete_entity(ammo_id);
					}
					// TODO: emit(emit_particles_message{ 3, attacker_pos->x, attacker_pos->y, attacker_pos->z, defender_pos->x, defender_pos->y, defender_pos->z });
				}

				LOG ss;
				ss.settler_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with their ")->col(color_t{ 1.0f, 1.0f, 0.0f })->text(weapon_name + std::string(". "))->col(color_t{ 1.0f, 1.0f, 1.0f });
				const int skill_modifier = get_skill_modifier(*attacker_stats, "Ranged Attacks");
				const int die_roll = rng.roll_dice(1, 20) + stat_modifier(attacker_stats->dexterity + skill_modifier) + weapon_quality;
				const int armor_class = calculate_armor_class(*defender);
				if (die_roll > armor_class) {
					const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod + stat_modifier(attacker_stats->strength) + skill_modifier) + weapon_quality;
					ss.text("The attack hits for " + std::to_string(damage) + " points of damage.");
					damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.victim, damage, weapon_name });
					gain_skill_from_success(msg.attacker, *attacker_stats, "Ranged Attacks", armor_class, rng);
				}
				else {
					ss.text("The attack misses.");
				}
				log_message lmsg{ ss.chars };
				logging::log(lmsg);
			});
		}
	}
}
