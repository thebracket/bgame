#include "kill_system.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_globals.hpp"
#include "../../components/settler_ai.hpp"
#include "../../planet/planet.hpp"
#include "../../components/renderable.hpp"
#include "../../components/corpse_settler.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "../../components/species.hpp"
#include "../../components/sentient_ai.hpp"
#include "../ai/movement_system.hpp"
#include "../../utils/telemetry.hpp"

using namespace rltk;

void kill_system::on_message(const entity_slain_message &msg) {
    auto victim = entity(msg.victim);
    if (!victim) return;
    auto pos = victim->component<position_t>();
    // Any items carried are dropped
    each<item_carried_t>([&msg, pos] (entity_t &e, item_carried_t &item) {
        if (item.carried_by == msg.victim) {
            emit(drop_item_message{e.id, pos->x, pos->y, pos->z});
        }
    });

    // Spawn a body
    if (victim->component<settler_ai_t>()) {
        // It's a dead settler, we create a special item
        auto name = victim->component<name_t>();
        auto corpse = create_entity()
                ->assign(position_t{ pos->x, pos->y, pos->z })
                ->assign(renderable_t{ 2, 2, rltk::colors::YELLOW, rltk::colors::RED })
                ->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") })
                ->assign(corpse_settler{msg.cause_of_death});
        call_home("settler_death");
    } else if (victim->component<sentient_ai>()) {
        // It's a dead native
        /*
        planet.civs.population[victim->component<sentient_ai>()->person_id].deceased = true;
        auto name = victim->component<name_t>();
        auto renderable = victim->component<renderable_t>();
        auto corpse = create_entity()
                ->assign(position_t{ pos->x, pos->y, pos->z })
                ->assign(renderable_t{ renderable->glyph, rltk::colors::YELLOW, rltk::colors::RED })
                ->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") })
                ->assign(corpse_settler{msg.cause_of_death});
        call_home("sentient_death", name->first_name);*/
    } else {
        // It's something else that died.
        std::string tag = "";
        auto species = victim->component<species_t>();
        if (species) tag = species->tag;
        auto old_render = victim->component<renderable_t>();
        auto name = victim->component<name_t>();
        if (old_render && tag != "") {
            auto corpse = create_entity()
                    ->assign(position_t{ pos->x, pos->y, pos->z })
                    ->assign(renderable_t{ old_render->glyph, old_render->glyph_ascii, rltk::colors::GREY, rltk::colors::BLACK })
                    ->assign(corpse_harvestable{tag})
                    ->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") });
            emit_deferred(butcherable_moved_message{});
        }
        call_home("other_death", tag);
    }

    // Remove the entity
    entity_octree.remove_node(octree_location_t{static_cast<int>(pos->x), static_cast<int>(pos->y), pos->z, msg.victim});
    delete_entity(msg.victim);
}

