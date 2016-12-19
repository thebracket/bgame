#include "damage_system.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_globals.hpp"
#include "../movement_system.hpp"
#include "../weapons_helpers.hpp"
#include "../../messages/log_message.hpp"
#include "../../components/logger.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/item.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/health.hpp"
#include "../../components/renderable.hpp"
#include "../../components/corpse_settler.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "../../components/species.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/creatures.hpp"
#include "../tasks/civ_dislike.hpp"
#include <rltk.hpp>
#include <iostream>
#include <sstream>

void damage_system::configure() {
    system_name = "Damage System";
    subscribe_mbox<inflict_damage_message>();
    subscribe_mbox<hour_elapsed_message>();
}

void damage_system::apply_damage() {
    // Apply damage
    std::queue<inflict_damage_message> * damage = mbox<inflict_damage_message>();
	while (!damage->empty()) {
        inflict_damage_message msg = damage->front();
        if (!entity(msg.victim)) break;
        auto h = entity(msg.victim)->component<health_t>();
        auto name = entity(msg.victim)->component<name_t>();

        if (h) {
            h->current_hitpoints -= msg.damage_amount;

            int total_size = 0;
            for (const health_part_t &p : h->parts) {
                total_size += p.size;
            }

            emit_deferred(log_message{LOG{}.other_name(msg.victim)->text(std::string(" suffers ") + std::to_string(msg.damage_amount) + std::string(" points of damage. Source: "+msg.damage_type))->chars});
            if (h->current_hitpoints < 1) {
                if (h->current_hitpoints > -10) {
                    h->unconscious = true;
                    emit_deferred(log_message{LOG{}.other_name(msg.victim)->text(" is unconscious!")->chars});
                } else {
                    emit_deferred(log_message{LOG{}.other_name(msg.victim)->text(" is dead!")->chars});
                    emit(entity_slain_message{msg.victim, msg.damage_type});
                }
            }

            // Serious damage affects body parts too
            if (rng.roll_dice(1, 20) + msg.damage_amount > 15) {
                health_part_t * hit_part = nullptr;
                int hit_location_roll = rng.roll_dice(1, total_size);
                for (health_part_t &p : h->parts) {
                    if (hit_location_roll < p.size) {
                        hit_part = &p;
                        break;
                    }
                    hit_location_roll -= p.size;
                }
                if (hit_part == nullptr) hit_part = &(h->parts[h->parts.size()-1]);

                hit_part -> current_hitpoints -= msg.damage_amount;
                if (hit_part->current_hitpoints < 0) {
                    if (hit_part->part == "head" && hit_part->current_hitpoints > -10) {
                        emit_deferred(log_message{LOG{}.other_name(msg.victim)->text(" passes out from head trauma.")->chars});
                        h->unconscious = true;
                    } else if (hit_part->part == "head" && hit_part->current_hitpoints < -9) {
                        emit_deferred(log_message{LOG{}.other_name(msg.victim)->text("'s head is knocked clean off! Death is the inevitable result.'")->chars});
                        emit(entity_slain_message{msg.victim, msg.damage_type});
                    } else if (hit_part->current_hitpoints > -10) {
                        emit_deferred(log_message{LOG{}.other_name(msg.victim)->text(" passes out from ")->text(hit_part->part)->text(" trauma.")->chars});
                        h->unconscious = true;
                    } else {
                        emit_deferred(log_message{LOG{}.other_name(msg.victim)->text(" dies from ")->text(hit_part->part)->text(" trauma.")->chars});
                        emit(entity_slain_message{msg.victim, msg.damage_type});
                    }
                }
            }
            auto pos = entity(msg.victim)->component<position_t>();
            if (pos) {
                current_region->blood_stains[mapidx(pos->x, pos->y, pos->z)] = true;
            }

        }

		damage->pop();
	}
}

void damage_system::update(const double ms) {
    apply_damage();
}
