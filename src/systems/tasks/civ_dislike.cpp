#include "civ_dislike.hpp"
#include "../../planet/planet.hpp"
#include "../../messages/log_message.hpp"
#include "../../components/logger.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../main/game_globals.hpp"

using namespace rltk;

void civ_dislike_attacker(entity_t * victim) {
    /*
    if (!victim) return;
    auto victim_ai = victim->component<sentient_ai>();
    if (victim_ai) {
        const std::size_t civ_id = planet.civs.population[victim_ai->person_id].civ_id;
        if (planet.civs.civs[civ_id].cordex_feelings > -10) {
            --planet.civs.civs[civ_id].cordex_feelings;
            emit_deferred(log_message{LOG{}.civ_name(civ_id)->text(" dislikes you more.")->chars});
        }

        for (auto &relation : planet.civs.civs[civ_id].relations) {
            if (relation.first == civ_id && relation.second <0) {
                const std::size_t other_civ_id = relation.first;
                if (!planet.civs.civs[other_civ_id].extinct) {
                    for (auto &rel2 : planet.civs.civs[other_civ_id].relations) {
                        if (rel2.first == civ_id && rel2.second < 0) {
                            ++planet.civs.civs[other_civ_id].cordex_feelings;
                            emit_deferred(log_message{LOG{}.civ_name(other_civ_id)->text(" is grateful for your assistance against ")->civ_name(civ_id)->chars});
                        }
                    }
                }
            }
        }
    }*/
}
