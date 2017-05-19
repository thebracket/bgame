#include "initiative_system.hpp"
#include "../../components/initiative.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/grazer_ai.hpp"
#include "../tasks/initiative.hpp"
#include "../../components/slidemove.hpp"
#include "../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../components/riding_t.hpp"

void initiative_system::on_message(const tick_message &msg) {
    each<initiative_t>([] (entity_t &e, initiative_t &i) {
       --i.initiative;
        if ( i.initiative + i.initiative_modifier < 1) {
            // Remove any sliding
            auto pos = e.component<position_t>();
            if (pos) {
                pos->offsetX = 0.0F;
                pos->offsetY = 0.0F;
                pos->offsetZ = 0.0F;
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

            if (mount_v) {
                auto mount_entity = entity(mount_v->riding);
                if (!mount_entity) {
                    // Mount has gone away - be confused and revert to old behavior!
                    delete_component<riding_t>(e.id);
                } else {
                    auto stats = mount_entity->component<game_stats_t>(); // Use the mount's initiative
                    if (stats) tasks::calculate_initiative(i, *stats);
                }
            } else if (settler_ai_v) {
                auto stats = e.component<game_stats_t>();
                if (stats) {
                    tasks::calculate_initiative(i, *stats);
                }
            } else if (sentient_ai_v) {
                auto stats = e.component<game_stats_t>();
                if (stats) {
                    tasks::calculate_initiative(i, *stats);
                }
            } else if (grazer_ai_v) {
                i.initiative = std::max(1, rng.roll_dice(1, 12) - i.initiative_modifier);
            }

            // Reset modifiers
            i.initiative_modifier = 0;
        } else {
            auto slide = e.component<slidemove_t>();
            auto pos = e.component<position_t>();
            if (slide && pos && slide->lifespan > 0) {
                pos->offsetX += slide->offsetX;
                pos->offsetY += slide->offsetY;
                pos->offsetZ += slide->offsetZ;
                --slide->lifespan;
            }
        }
    });
    emit(renderables_changed_message{});
}