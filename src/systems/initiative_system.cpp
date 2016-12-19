#include "initiative_system.hpp"
#include "../components/initiative.hpp"
#include "../components/game_stats.hpp"
#include "../messages/messages.hpp"
#include "../components/settler_ai.hpp"
#include "../components/sentient_ai.hpp"
#include "../components/grazer_ai.hpp"
#include "tasks/initiative.hpp"
#include "../main/game_globals.hpp"

void initiative_system::on_message(const tick_message &msg) {
    each<initiative_t>([] (entity_t &e, initiative_t &i) {
       --i.initiative;
        if ( i.initiative < 1) {
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
        }
    });
}