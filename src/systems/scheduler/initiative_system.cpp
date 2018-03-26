#include "stdafx.h"
#include "initiative_system.hpp"
#include "../../global_assets/rng.hpp"

namespace systems {
	namespace initiative {

		inline void calculate_initiative(initiative_t &ai, game_stats_t &stats) {
			ai.initiative = std::max(1, rng.roll_dice(1, 12) - stat_modifier(stats.dexterity) + ai.initiative_modifier);
		}

		void run(const double &duration_ms) {
			using namespace bengine;

			each<initiative_t>([](entity_t &e, initiative_t &i) {
				--i.initiative;
				if (i.initiative + i.initiative_modifier < 1) {
					// Remove any sliding
					auto pos = e.component<position_t>();
					if (pos) {
						pos->offset_x = 0.0F;
						pos->offset_y = 0.0F;
						pos->offset_z = 0.0F;
					}
					auto slide = e.component<slidemove_t>();
					if (slide) {
						slide->offsetX = 0.0F;
						slide->offsetY = 0.0F;
						slide->offsetZ = 0.0F;
						slide->lifespan = 0;
					}

					// Emit a message that it's the entity's turn
					e.assign(ai_tag_my_turn_t{});

					// Roll initiative!

					auto settler_ai_v = e.component<settler_ai_t>();
					auto sentient_ai_v = e.component<sentient_ai>();
					auto grazer_ai_v = e.component<grazer_ai>();
					auto mount_v = e.component<riding_t>();
					auto turret_v = e.component<turret_t>();

					if (mount_v) {
						auto mount_entity = entity(mount_v->riding);
						if (!mount_entity) {
							// Mount has gone away - be confused and revert to old behavior!
							delete_component<riding_t>(e.id);
						}
						else {
							auto stats = mount_entity->component<game_stats_t>(); // Use the mount's initiative
							if (stats) calculate_initiative(i, *stats);
						}
					}
					else if (settler_ai_v) {
						auto stats = e.component<game_stats_t>();
						if (stats) {
							calculate_initiative(i, *stats);
						}
					}
					else if (sentient_ai_v) {
						auto stats = e.component<game_stats_t>();
						if (stats) {
							calculate_initiative(i, *stats);
						}
					}
					else if (grazer_ai_v) {
						i.initiative = std::max(1, rng.roll_dice(1, 12) - i.initiative_modifier);
					}
					else if (turret_v) {
						i.initiative = 10;
					}

					// Reset modifiers
					i.initiative_modifier = 0;
				}
				else {
					auto slide = e.component<slidemove_t>();
					auto pos = e.component<position_t>();
					if (slide && pos && slide->lifespan > 0) {
						pos->offset_x += slide->offsetX;
						pos->offset_y += slide->offsetY;
						pos->offset_z += slide->offsetZ;
						--slide->lifespan;
					}
				}
			});
		}
	}
}
