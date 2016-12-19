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
#include "../movement_system.hpp"

using namespace rltk;

void kill_system::configure() {
    system_name = "Kill System";
    subscribe_mbox<entity_slain_message>();
}

void kill_system::inflict_death() {
    std::queue<entity_slain_message> * deaths = mbox<entity_slain_message>();
    while (!deaths->empty()) {
        entity_slain_message msg = deaths->front();
        deaths->pop();

        auto victim = entity(msg.victim);
        if (!victim) break;
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
                    ->assign(renderable_t{ 2, rltk::colors::YELLOW, rltk::colors::RED })
                    ->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") })
                    ->assign(corpse_settler{msg.cause_of_death});
        } else if (victim->component<sentient_ai>()) {
            // It's a dead native
            planet.civs.unimportant_people[victim->component<sentient_ai>()->person_id].deceased = true;
            auto name = victim->component<name_t>();
            auto renderable = victim->component<renderable_t>();
            auto corpse = create_entity()
                    ->assign(position_t{ pos->x, pos->y, pos->z })
                    ->assign(renderable_t{ renderable->glyph, rltk::colors::YELLOW, rltk::colors::RED })
                    ->assign(name_t{ name->first_name, name->last_name + std::string("'s corpse") })
                    ->assign(corpse_settler{msg.cause_of_death});
        } else {
            // It's something else that died.
            std::string tag = "";
            auto species = victim->component<species_t>();
            if (species) tag = species->tag;
            auto old_render = victim->component<renderable_t>();
            if (old_render && tag != "") {
                auto corpse = create_entity()
                        ->assign(position_t{ pos->x, pos->y, pos->z })
                        ->assign(renderable_t{ old_render->glyph, rltk::colors::GREY, rltk::colors::BLACK })
                        ->assign(corpse_harvestable{tag});
            }
        }

        // Remove the entity
        entity_octree.remove_node(octree_location_t{pos->x, pos->y, pos->z, msg.victim});
        delete_entity(msg.victim);
    }
}

void kill_system::update(const double ms) {
    inflict_death();
}

