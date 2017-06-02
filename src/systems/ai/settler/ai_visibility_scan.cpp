#include "ai_visibility_scan.hpp"
#include "../../../main/game_planet.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/viewshed.hpp"
#include "../../tasks/threat_scanner.hpp"
#include "../../../components/grazer_ai.hpp"
#include "../../../components/sentient_ai.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../components/health.hpp"
#include "../../../messages/entity_moved_message.hpp"
#include "../../../messages/inflict_damage_message.hpp"
#include "../../damage/weapons_helpers.hpp"
#include "../../../components/initiative.hpp"
#include "../../../main/game_pause.hpp"
#include "../../../main/game_designations.hpp"
#include "../../../components/riding_t.hpp"
#include "../../../components/turret_t.hpp"

void ai_visibility_scan::configure() {}

namespace ai_visibility {
    sentient_ai *ai = nullptr;
}

bool grazer_hostile_scan(entity_t &other) {
    if (other.component<settler_ai_t>() || other.component<sentient_ai>()) {
        return true;
    } else {
        return false;
    }
}

bool sentient_hostile_scan(entity_t &other) {
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
        } else {
            return false;
        }
    }
    auto sentient = other.component<sentient_ai>();
    if (sentient) {
        if (sentient->hostile) {
            return true;
        } else {
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


void ai_visibility_scan::update(const double duration_ms) {
    if (pause_mode != RUNNING) return;

    each<ai_tag_my_turn_t, viewshed_t, position_t>([] (entity_t &e, ai_tag_my_turn_t &t, viewshed_t &view, position_t &pos) {
        auto grazer = e.component<grazer_ai>();
        auto sentient = e.component<sentient_ai>();
        auto settler = e.component<settler_ai_t>();
        auto initiative = e.component<initiative_t>();
        auto turret = e.component<turret_t>();

        std::function<bool(rltk::entity_t&)> scanner_func;
        if (grazer) {
            scanner_func = grazer_hostile_scan;
        } else if (sentient) {
            scanner_func = sentient_hostile_scan;
            ai_visibility::ai = sentient;
        } else if (settler) {
            scanner_func = settler_hostile_scan;
        } else if (turret) {
            if (turret->owner_civilization == 0) {
                scanner_func = turret_friendly_scan;
            } else {
                scanner_func = turret_hostile_scan;
            }
        } else {
            throw std::runtime_error("Visibility present, but no AI?");
        }

        auto hostile = tasks::can_see_hostile(e, pos, view, scanner_func);

        if (!hostile.terrified) return; // Nothing to do - no fight/flight reactions

        if (grazer) {
            // Poor creature is scared!
            if (hostile.terror_distance < 1.5F) {
                // Attack the target
                auto health = entity(e.id)->component<health_t>();
                if (health) {
                    if (!health->unconscious) {
                        emit_deferred(creature_attack_message{e.id, hostile.closest_fear});
                        emit_deferred(huntable_moved_message{});
                        delete_component<ai_tag_my_turn_t>(e.id);
                    }
                }
            } else {
                emit_deferred(entity_wants_to_flee_message{e.id, hostile.closest_fear});
                emit_deferred(huntable_moved_message{});
                delete_component<ai_tag_my_turn_t>(e.id);
            }
        } else if (sentient) {
            // Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
            // is implemented it's a good idea not to run away from your target.
            auto mounted = e.component<riding_t>();

            const float range = shooting_range(e, pos);
            if (hostile.terror_distance < 1.5F) {
                // Hit it with melee weapon
                emit_deferred(sentient_attack_message{e.id, hostile.closest_fear});
                initiative->initiative_modifier += get_weapon_initiative_penalty(get_melee_id(e));
                delete_component<ai_tag_my_turn_t>(e.id);
                if (mounted) {
                    // Let the mount attack also
                    emit_deferred(creature_attack_message(mounted->riding, hostile.closest_fear));
                }
            } else if ( range != -1 && range < hostile.terror_distance) {
                // Shoot it
                emit_deferred(sentient_ranged_attack_message{e.id, hostile.closest_fear});
                initiative->initiative_modifier += get_weapon_initiative_penalty(get_ranged_and_ammo_id(e).first);
                delete_component<ai_tag_my_turn_t>(e.id);
            } else {
                emit_deferred(entity_wants_to_charge_message{e.id, hostile.closest_fear});
                ai_visibility::ai->goal = SENTIENT_GOAL_CHARGE;
            }

        } else if (settler) {
            // Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
            // is implemented it's a good idea not to run away from your target.
            const int range = shooting_range(e, pos);
            if (hostile.terror_distance < 1.5F) {
                // Hit it with melee weapon
                emit_deferred(settler_attack_message{e.id, hostile.closest_fear});
                initiative->initiative_modifier += get_weapon_initiative_penalty(get_melee_id(e));
            } else if (range != -1 && range < hostile.terror_distance) {
                // Shoot it
                emit_deferred(settler_ranged_attack_message{e.id, hostile.closest_fear});
                initiative->initiative_modifier += get_weapon_initiative_penalty(get_ranged_and_ammo_id(e).first);
            } else {
                emit_deferred(entity_wants_to_flee_message{e.id, hostile.closest_fear});
            }
        } else if (turret) {
            const int range = shooting_range(e, pos);
            if (range <= turret->range) {
                emit_deferred(turret_ranged_attack_message{ e.id, hostile.closest_fear });
            }
        }
    });
}