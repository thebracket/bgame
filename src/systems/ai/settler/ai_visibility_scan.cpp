#include "stdafx.h"
#include "ai_visibility_scan.hpp"
#include "../../../global_assets/game_planet.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../physics/movement_system.hpp"
#include "../../helpers/weapons_helper.hpp"
#include "../../damage/creature_attacks_system.hpp"
#include "../distance_map_system.hpp"
#include "../../damage/sentient_attacks_system.hpp"
#include "../../damage/settler_melee_attacks_system.hpp"
#include "../../damage/settler_ranged_attack_system.hpp"
#include "../../damage/turret_ranged_attack_system.hpp"
#include "../../../bengine/geometry.hpp"

namespace systems {
	namespace ai_visibility_scan {

		using namespace bengine;

		struct spotted_hostile_t {
			bool terrified = false;
			float terror_distance = std::numeric_limits<float>::max();
			int closest_fear = 0;
		};

		spotted_hostile_t can_see_hostile(const bengine::entity_t &e, const position_t &pos, const viewshed_t &view, const std::function<bool(bengine::entity_t&)> &test) {
			spotted_hostile_t result;

			for (const std::size_t other_entity : view.visible_entities) {
				const auto other = entity(other_entity);
				if (!other) {
					// We can't access the other entity - so skip it
					break;
				}

				if (other && test(*other)) {
					result.terrified = true;
					const auto other_pos = other->component<position_t>();
					if (other_pos) {
						const float d = distance3d_squared(pos.x, pos.y, pos.z, other_pos->x, other_pos->y, other_pos->z);
						if (d < result.terror_distance) {
							result.terror_distance = d;
							result.closest_fear = other_entity;
						}
					}
				}
			}

			return result;
		}

		namespace ai_visibility {
			sentient_ai *ai = nullptr;
		}

		bool grazer_hostile_scan(entity_t &other) {
			if (other.component<settler_ai_t>() || other.component<sentient_ai>()) {
				return true;
			}
			return false;			
		}

		bool sentient_hostile_scan(entity_t &other) {
			auto hostile_sentient = false;
			const auto other_sentient = other.component<sentient_ai>();
			if (other_sentient) {
				const auto my_civ = ai_visibility::ai->civ_id;
				const auto their_civ = other_sentient->civ_id;
				if (my_civ != their_civ) {
					const auto civfinder = planet.civs.civs[my_civ].relations.find(their_civ);
					if (civfinder != planet.civs.civs[my_civ].relations.end()) {
						if (civfinder->second < 0) hostile_sentient = true;
					}
				}
			}

			if ((other.component<grazer_ai>()) || (other.component<settler_ai_t>() && ai_visibility::ai->hostile) || hostile_sentient)
			{
				return true;
			}

			return false;
		}

		bool settler_hostile_scan(entity_t &other) {
			if (other.component<grazer_ai>() && designations->standing_order_wildlife_treatment != standing_orders::SO_WILDLIFE_IGNORE) return true;
			auto sentient = other.component<sentient_ai>();
			if (sentient && sentient->hostile) return true;
			return false;
		}

		bool turret_friendly_scan(entity_t &other) {
			// A friendly (civ0) turret
			if (other.component<grazer_ai>()) {
				if (designations->standing_order_wildlife_treatment != standing_orders::SO_WILDLIFE_IGNORE) {
					return true;
				}
				else {
					return false;
				}
			}
			auto sentient = other.component<sentient_ai>();
			if (sentient) {
				if (sentient->hostile) {
					return true;
				}
				else {
					return false;
				}
			}
			return false;
		}

		bool turret_hostile_scan(entity_t &other) {
			bool hostile_sentient = false;
			auto other_sentient = other.component<sentient_ai>();
			if (other_sentient) {
				const std::size_t my_civ = ai_visibility::ai->civ_id;
				const std::size_t their_civ = other_sentient->civ_id;
				if (my_civ != their_civ) {
					auto civfinder = planet.civs.civs[my_civ].relations.find(their_civ);
					if (civfinder != planet.civs.civs[my_civ].relations.end()) {
						if (civfinder->second < 0) hostile_sentient = true;
					}
				}
			}

			if ((other.component<grazer_ai>()) ||
				(other.component<settler_ai_t>() && ai_visibility::ai->hostile) ||
				hostile_sentient)
			{
				return true;
			}

			return false;
		}

		void run(const double &duration_ms) {
			each<ai_tag_my_turn_t, viewshed_t, position_t>([](entity_t &e, ai_tag_my_turn_t &t, viewshed_t &view, position_t &pos) {
				auto grazer = e.component<grazer_ai>();
				auto sentient = e.component<sentient_ai>();
				auto settler = e.component<settler_ai_t>();
				auto initiative = e.component<initiative_t>();
				auto turret = e.component<turret_t>();

				std::function<bool(bengine::entity_t&)> scanner_func;
				if (grazer) {
					scanner_func = grazer_hostile_scan;
				}
				else if (sentient) {
					scanner_func = sentient_hostile_scan;
					ai_visibility::ai = sentient;
				}
				else if (settler) {
					scanner_func = settler_hostile_scan;
				}
				else if (turret) {
					if (turret->owner_civilization == 0) {
						scanner_func = turret_friendly_scan;
					}
					else {
						scanner_func = turret_hostile_scan;
					}
				}
				else {
					throw std::runtime_error("Visibility present, but no AI?");
				}

				auto hostile = can_see_hostile(e, pos, view, scanner_func);

				if (!hostile.terrified) return; // Nothing to do - no fight/flight reactions

				if (grazer) {
					// Poor creature is scared!
					if (hostile.terror_distance < 1.5F) {
						// Attack the target
						auto health = entity(e.id)->component<health_t>();
						if (health) {
							if (!health->unconscious) {
								creature_attacks::request_attack(creature_attacks::creature_attack_message{ e.id, hostile.closest_fear });
								delete_component<ai_tag_my_turn_t>(e.id);
							}
						}
					}
					else {
						systems::movement::request_flee(e.id, hostile.closest_fear);
						delete_component<ai_tag_my_turn_t>(e.id);
					}
				}
				else if (sentient) {
					// Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
					// is implemented it's a good idea not to run away from your target.
					auto mounted = e.component<riding_t>();

					const float range = static_cast<float>(weapons::shooting_range(e, pos));
					if (hostile.terror_distance < 1.5F) {
						// Hit it with melee weapon
						sentient_attacks::request_attack(sentient_attacks::sentient_attack_message{ e.id, hostile.closest_fear });
						initiative->initiative_modifier += weapons::get_weapon_initiative_penalty(weapons::get_melee_id(e));
						delete_component<ai_tag_my_turn_t>(e.id);
						if (mounted) {
							// Let the mount attack also
							creature_attacks::request_attack(creature_attacks::creature_attack_message(mounted->riding, hostile.closest_fear));
						}
					}
					else if (range != -1 && range < hostile.terror_distance) {
						// Shoot it
						sentient_attacks::request_ranged_attack(sentient_attacks::sentient_ranged_attack_message{ e.id, hostile.closest_fear });
						initiative->initiative_modifier += weapons::get_weapon_initiative_penalty(weapons::get_ranged_and_ammo_id(e).first);
						delete_component<ai_tag_my_turn_t>(e.id);
					}
					else {
						systems::movement::request_charge(e.id, hostile.closest_fear );
						ai_visibility::ai->goal = SENTIENT_GOAL_CHARGE;
					}

				}
				else if (settler) {
					// Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
					// is implemented it's a good idea not to run away from your target.
					const auto range = weapons::shooting_range(e, pos);
					//std::cout << "Range: " << range << "\n";
					if (hostile.terror_distance < 1.5F) {
						// Hit it with melee weapon
						settler_melee_attack::request_melee(settler_melee_attack::settler_attack_message{ e.id, hostile.closest_fear });
						initiative->initiative_modifier += weapons::get_weapon_initiative_penalty(weapons::get_melee_id(e));
					}
					else if (range != -1 && range < hostile.terror_distance) {
						// Shoot it
						settler_ranged_attack::request_settler_ranged_attack(settler_ranged_attack::settler_ranged_attack_message{ e.id, hostile.closest_fear });
						initiative->initiative_modifier += weapons::get_weapon_initiative_penalty(weapons::get_ranged_and_ammo_id(e).first);
					}
					else {
						movement::request_flee(e.id, hostile.closest_fear);
					}
				}
				else if (turret) {
					const int range = weapons::shooting_range(e, pos);
					if (range <= turret->range) {
						turret_attacks::request_attack(turret_attacks::turret_ranged_attack_message{ e.id, hostile.closest_fear });
					}
				}
			});
		}
	}
}
