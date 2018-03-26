#include "stdafx.h"
#include "damage_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/rng.hpp"
#include "../../planet/region/region.hpp"
#include "kill_system.hpp"
#include "../helpers/inventory_assistant.hpp"

using namespace tile_flags;

namespace systems {
	namespace damage_system {

		using namespace bengine;

		thread_safe_message_queue<inflict_damage_message> damage;

		void inflict_damage(inflict_damage_message msg) {
			damage.enqueue(inflict_damage_message{ msg.victim, msg.damage_amount, msg.damage_type });
		}

		void do_building_damage(const inflict_damage_message &msg) {
			auto e = entity(msg.victim);
			if (!e) return;
			auto building = e->component<building_t>();
			if (!building) return;

			const int scaled_damage = msg.damage_amount / 10;
			if (building->hit_points <= scaled_damage) {
				// Building is destroyed
				logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(std::string(" is destroyed!"))->chars };
				logging::log(lmsg);
				delete_entity(msg.victim);
			}
			else {
				building->hit_points -= scaled_damage;
				logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(std::string(" suffers ") + std::to_string(msg.damage_amount) + std::string(" points of damage. Source: " + msg.damage_type))->chars };
				logging::log(lmsg);
			}
		}

		void run(const double &duration_ms) {
			damage.process_all([](const inflict_damage_message &msg) {
				if (!entity(msg.victim)) return;

				if (entity(msg.victim)->component<building_t>()) {
					do_building_damage(msg);
					return;
				}

				auto h = entity(msg.victim)->component<health_t>();

				if (h) {
					h->current_hitpoints -= msg.damage_amount;

					int total_size = 0;
					for (const health_part_t &p : h->parts) {
						total_size += p.size;
					}

					logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(std::string(" suffers ") + std::to_string(msg.damage_amount) + std::string(" points of damage. Source: " + msg.damage_type))->chars };
					logging::log(lmsg);
					if (h->current_hitpoints < 1) {
						if (h->current_hitpoints > -10) {
							h->unconscious = true;
							logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(" is unconscious!")->chars};
							logging::log(lmsg);
						}
						else {
							logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(" is dead!")->chars };
							logging::log(lmsg);
							kill_system::fatality(kill_system::entity_slain_message{ msg.victim, msg.damage_type });
						}
					}

					// Where was hit?
					health_part_t * hit_part = nullptr;
					int hit_location_roll = rng.roll_dice(1, total_size);
					for (health_part_t &p : h->parts) {
						if (hit_location_roll < p.size) {
							hit_part = &p;
							break;
						}
						hit_location_roll -= p.size;
					}
					if (hit_part == nullptr) hit_part = &(h->parts[h->parts.size() - 1]);

					// Damage clothing/armor in the affected area
					item_location_t affected = TORSO;
					if (hit_part->part == "head") {
						affected = HEAD;
					}
					else if (hit_part->part == "left_leg" || hit_part->part == "right_leg") {
						affected = LEGS;
					}
					else if (hit_part->part == "left_foot" || hit_part->part == "right_foot") {
						affected = FEET;
					}
					std::vector<std::size_t> items_to_destroy;
					bengine::each<item_t, item_carried_t, item_wear_t>([&msg, &affected, &items_to_destroy](bengine::entity_t &e, item_t &i, item_carried_t &l, item_wear_t &wear) {
						if (l.carried_by == msg.victim && l.location == affected) {
							if (wear.wear > msg.damage_amount) {
								wear.wear -= msg.damage_amount;
								logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text("'s ")->text(i.item_name)->text(" is damaged by the blow.")->chars };
								logging::log(lmsg);
							}
							else {
								logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text("'s ")->text(i.item_name)->text(" is destroyed!")->chars };
								logging::log(lmsg);
								items_to_destroy.push_back(e.id);
							}
						}
					});
					for (const auto item_id : items_to_destroy) {
						inventory::delete_item(item_id);
					}

					// Serious damage affects body parts too
					if (rng.roll_dice(1, 20) + msg.damage_amount > 15) {						
						hit_part->current_hitpoints -= msg.damage_amount;
						if (hit_part->current_hitpoints < 0) {
							if (hit_part->part == "head" && hit_part->current_hitpoints > -10) {
								logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(" passes out from head trauma.")->chars };
								logging::log(lmsg);
								h->unconscious = true;
							}
							else if (hit_part->part == "head" && hit_part->current_hitpoints < -9) {
								logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text("'s head is knocked clean off! Death is the inevitable result.'")->chars };
								logging::log(lmsg);
								kill_system::fatality(kill_system::entity_slain_message{ msg.victim, msg.damage_type });
							}
							else if (hit_part->current_hitpoints > -10) {
								logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(" passes out from ")->text(hit_part->part)->text(" trauma.")->chars };
								logging::log(lmsg);
								h->unconscious = true;
							}
							else {
								logging::log_message lmsg{ LOG{}.other_name(msg.victim)->text(" dies from ")->text(hit_part->part)->text(" trauma.")->chars };
								logging::log(lmsg);
								kill_system::fatality(kill_system::entity_slain_message{ msg.victim, msg.damage_type });
							}
						}
					}
					auto pos = entity(msg.victim)->component<position_t>();
					if (pos) {
						region::set_flag(mapidx(pos->x, pos->y, pos->z), BLOODSTAIN);
					}

				}
			});
		}
	}
}