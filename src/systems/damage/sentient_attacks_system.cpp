#include "stdafx.h"
#include "sentient_attacks_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/rng.hpp"
#include "../helpers/weapons_helper.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "damage_system.hpp"
#include "../gui/particle_system.hpp"

namespace systems {
	namespace sentient_attacks {

		using namespace bengine;
		using namespace weapons;

		thread_safe_message_queue<sentient_attack_message> attacks;
		thread_safe_message_queue<sentient_ranged_attack_message> ranged_attacks;

		void request_attack(sentient_attack_message msg) {
			attacks.enqueue(sentient_attack_message{ msg.attacker, msg.victim });
		}

		void request_ranged_attack(sentient_ranged_attack_message msg) {
			ranged_attacks.enqueue(sentient_ranged_attack_message{ msg.attacker, msg.victim });
		}

		void sentient_melee_attack(const std::string &weapon_name,
			const int &hit_bonus,
			const int &weapon_n,
			const int &weapon_d,
			const int &weapon_mod,
			const sentient_attack_message &msg) {

			LOG ss;
			ss.other_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with their ")->col(color_t{ 1.0f, 1.0f, .0f })->text(weapon_name + std::string(". "))->col(color_t{ 1.0f,1.0f,1.0f });

			auto defender = entity(msg.victim);
			if (!defender) return;

			auto attacker_pos = entity(msg.attacker)->component<position_t>();
			auto defender_pos = entity(msg.victim)->component<position_t>();
			if (!attacker_pos || !defender_pos) return;
			color_t red{ 1.0f, 0.0f, 0.0f };
			particles::melee_attack(*attacker_pos, *defender_pos, red);

			const int die_roll = rng.roll_dice(1, 20) + hit_bonus;
			const int armor_class = calculate_armor_class(*defender);

			if (die_roll > armor_class) {
				const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod);
				damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.victim, damage, weapon_name });
				ss.text(std::string("The attack hits, for ") + std::to_string(damage) + std::string("."));
			}
			else {
				ss.text("The attack misses.");
			}

			logging::log_message lmsg{ ss.chars };
			logging::log(lmsg);
		}



		void run(const double &duration_ms) {
			attacks.process_all([](const sentient_attack_message &msg) {

				auto sentient_entity = entity(msg.attacker);
				if (!sentient_entity) return;
				auto ai = sentient_entity->component<sentient_ai>();
				if (!ai) return;

				std::size_t weapon_id = get_melee_id(*sentient_entity);
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
				}
				else {
					auto weapon_component = entity(weapon_id)->component<item_t>();
					if (weapon_component) {
						auto weapon_finder = get_item_def(weapon_component->item_tag);
						if (weapon_finder != nullptr) {
							sentient_melee_attack(weapon_finder->name, 0,
								weapon_finder->damage_n, weapon_finder->damage_d,
								weapon_finder->damage_mod + get_material(weapon_component->material)->damage_bonus,
								msg);
						}
					}
				}
			});

			ranged_attacks.process_all([](const sentient_ranged_attack_message &msg) {

				auto attacker = entity(msg.attacker);
				auto defender = entity(msg.victim);
				if (!attacker || !defender) return;
				auto attacker_stats = attacker->component<game_stats_t>();
				auto attacker_pos = attacker->component<position_t>();
				auto defender_pos = defender->component<position_t>();
				if (!attacker_pos || !defender_pos) return;
				color_t red{ 1.0f, 0.0f, 0.0f };
				particles::ranged_attack(*attacker_pos, *defender_pos, red);

				// TODO: civ_dislike_attacker(defender);

				// Held Weapons
				auto [weapon_id, ammo_id] = get_ranged_and_ammo_id(*attacker);
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

				// Natural weapons
				auto na = attacker->component<natural_attacks_t>();
				if (na) {
					for (const auto &a : na->attacks) {
						if (a.range > 0) {
							weapon_n = a.n_dice;
							weapon_d = a.die_type;
							weapon_mod = a.die_mod;
							// TODO: emit(emit_particles_message{ 3, attacker_pos->x, attacker_pos->y, attacker_pos->z, defender_pos->x, defender_pos->y, defender_pos->z });
						}
					}
				}

				LOG ss;
				ss.settler_name(msg.attacker)->text(" attacks ")->other_name(msg.victim)->text(" with their ")->col(color_t{ 1.0f,1.0f,0.0f })->text(weapon_name + std::string(". "))->col(color_t{ 0.0f, 0.0f, 0.0f });
				const int skill_modifier = get_skill_modifier(*attacker_stats, "Ranged Attacks");
				const int die_roll = rng.roll_dice(1, 20) + stat_modifier(attacker_stats->dexterity + skill_modifier);
				const int armor_class = calculate_armor_class(*defender);
				if (die_roll > armor_class) {
					const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod + stat_modifier(attacker_stats->strength) + skill_modifier);
					ss.text("The attack hits for " + std::to_string(damage) + " points of damage.");
					damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.victim, damage, weapon_name });
					gain_skill_from_success(msg.attacker, *attacker_stats, "Ranged Attacks", armor_class, rng);
				}
				else {
					ss.text("The attack misses.");
				}
				logging::log_message lmsg{ ss.chars };
				logging::log(lmsg);
			});
		}
	}
}
