#include "stdafx.h"
#include "kill_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../bengine/telemetry.hpp"
#include "../../global_assets/spatial_db.hpp"
#include "../ai/inventory_system.hpp"
#include "../ai/distance_map_system.hpp"
#include "../../global_assets/game_planet.hpp"

namespace systems {
	namespace kill_system {

		using namespace bengine;

		thread_safe_message_queue<entity_slain_message> kills;

		void fatality(entity_slain_message msg) {
			kills.enqueue(entity_slain_message{ msg.victim, msg.cause_of_death });
		}

		void run(const double &duration_ms) {
			kills.process_all([](const entity_slain_message &msg) {
				auto victim = entity(msg.victim);
				if (!victim) return;
				auto pos = victim->component<position_t>();
				// Any items carried are dropped
				each<item_carried_t>([&msg, pos](entity_t &e, item_carried_t &item) {
					if (item.carried_by == msg.victim) {
						inventory_system::drop_item(e.id, pos->x, pos->y, pos->z );
					}
				});

				// Spawn a body
				if (victim->component<settler_ai_t>()) {
					// It's a dead settler, we create a special item
					auto name = victim->component<name_t>();
					create_entity()
						->assign(position_t{ pos->x, pos->y, pos->z })
						->assign(renderable_t{ 2, 2, color_t{1.0f, 1.0f, 0.0f}, color_t{1.0f, 0.0f, 0.0f}, 110 })
						->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") })
						->assign(corpse_settler{ msg.cause_of_death });
					call_home("combat", "settler_death");
				}
				else if (victim->component<sentient_ai>()) {
					// It's a dead native					
					//planet.civs.civs[ [victim->component<sentient_ai>()->person_id].deceased = true;
					auto name = victim->component<name_t>();
					auto renderable = victim->component<renderable_t>();
					auto corpse = create_entity()
						->assign(position_t{ pos->x, pos->y, pos->z })
						->assign(renderable_t{ 2, 2, color_t{ 1.0f, 1.0f, 0.0f }, color_t{ 1.0f, 0.0f, 0.0f}, 110 })
						->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") })
						->assign(corpse_settler{msg.cause_of_death});
					call_home("sentient_death", name->first_name);
				}
				else {
					// It's something else that died.
					std::string tag = "";
					auto species = victim->component<species_t>();
					if (species) tag = species->tag;
					auto old_render = victim->component<renderable_t>();
					auto name = victim->component<name_t>();
					if (old_render && tag != "") {
						create_entity()
							->assign(position_t{ pos->x, pos->y, pos->z })
							->assign(renderable_t{ old_render->glyph, old_render->glyph_ascii, color_t{0.5f, 0.5f, 0.5f}, color_t{0.f, 0.f, 0.f}, 110 })
							->assign(corpse_harvestable{ tag })
							->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") });
						distance_map::refresh_butcherables_map();
					}
					call_home("other_death", tag);
				}

				// Remove the entity
				entity_octree.remove_node(octree_location_t{ static_cast<int>(pos->x), static_cast<int>(pos->y), pos->z, msg.victim });
				delete_entity(msg.victim);
			});
		}
	}
}
