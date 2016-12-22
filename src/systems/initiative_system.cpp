#include "initiative_system.hpp"
#include "../components/initiative.hpp"
#include "../components/game_stats.hpp"
#include "../messages/messages.hpp"
#include "../components/settler_ai.hpp"
#include "../components/sentient_ai.hpp"
#include "../components/grazer_ai.hpp"
#include "tasks/initiative.hpp"
#include "../main/game_globals.hpp"
#include "../components/slidemove.hpp"

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
            emit_deferred(action_available_message{e.id});

            // Roll initiative!

            auto settler_ai_v = e.component<settler_ai_t>();
            auto sentient_ai_v = e.component<sentient_ai>();
            auto grazer_ai_v = e.component<grazer_ai>();

            if (settler_ai_v) {
                auto stats = e.component<game_stats_t>();
                if (stats) {
                    tasks::calculate_initiative(i, stats.get());
                }
            } else if (sentient_ai_v) {
                auto stats = e.component<game_stats_t>();
                if (stats) {
                    tasks::calculate_initiative(i, stats.get());
                }
            } else if (grazer_ai_v) {
                i.initiative = std::max(1, rng.roll_dice(1, 12) - i.initiative_modifier);
            } else {
                std::cout << "Warning: Entity has initiative but no AI???\n";
            }

            // Reset modifiers
            i.initiative_modifier = 0;
        } else {
            auto slide = e.component<slidemove_t>();
            auto pos = e.component<position_t>();
            if (slide && pos && slide->lifespan > 0) {
                if (e.id == 126) {
                    std::cout << "Slide life " << slide->lifespan << " (entity " << e.id << ")\n";
                    std::cout << "X:" << pos->x << " + " << pos->offsetX << " / " << slide->offsetX << "\n";
                    std::cout << "Y:" << pos->y << " + " << pos->offsetY << " / " << slide->offsetY << "\n\n";
                }

                pos->offsetX += slide->offsetX;
                pos->offsetY += slide->offsetY;
                pos->offsetZ += slide->offsetZ;
                --slide->lifespan;
            }
        }
    });
    emit(renderables_changed_message{});
}