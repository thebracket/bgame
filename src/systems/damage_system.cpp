#include "damage_system.hpp"
#include "../messages/messages.hpp"
#include "../components/components.hpp"
#include "../game_globals.hpp"
#include "movement_system.hpp"
#include <rltk.hpp>
#include <iostream>
#include <sstream>

void damage_system::configure() {
    system_name = "Damage System";
    subscribe_mbox<inflict_damage_message>();
    subscribe_mbox<creature_attack_message>();
    subscribe_mbox<entity_slain_message>();
}

void damage_system::update(const double ms) {
    // Creature attacks
    std::queue<creature_attack_message> * cattack = mbox<creature_attack_message>();
    while (!cattack->empty()) {
        creature_attack_message msg = cattack->front();
        cattack->pop();

        // Start by determining the attack
        std::stringstream ss;
        entity_t * attacker = entity(msg.attacker);
        species_t * attack_species = attacker->component<species_t>();
        name_t * attacker_name = attacker->component<name_t>();
        auto creature = creature_defs.find(attack_species->tag);

        entity_t * defender = entity(msg.victim);
        name_t * defender_name = defender->component<name_t>();
        game_stats_t * defender_stats = defender->component<game_stats_t>();

        if (creature == creature_defs.end()) {
            throw std::runtime_error("Undefined species - " + attack_species->tag);
        }
        for (const creature_attack_t &weapon : creature->second.attacks) {
            ss << attacker_name->first_name << " attacks " << defender_name->first_name << " with its " << weapon.type << ".";
            const int hit_roll = rng.roll_dice(1,20) + weapon.hit_bonus;
            // TODO: Armor!
            const int target = 10 + stat_modifier(defender_stats->dexterity);
            if (hit_roll < target) {
                ss << " The attack misses (roll: " << hit_roll << ").\n";
            } else {
                const int damage = rng.roll_dice(weapon.damage_n_dice, weapon.damage_dice) + weapon.damage_mod;
                ss << " The attack hits (roll: " << hit_roll << "), for " << damage << " points of damage.\n";
                emit(inflict_damage_message{ msg.victim, damage, weapon.type });
            }

        }

        std::cout << ss.str();
    }

    // Apply damage
    std::queue<inflict_damage_message> * damage = mbox<inflict_damage_message>();
	while (!damage->empty()) {
        inflict_damage_message msg = damage->front();
        health_t * h = entity(msg.victim)->component<health_t>();
        name_t * name = entity(msg.victim)->component<name_t>();

        if (h != nullptr) {
            h->current_hitpoints -= msg.damage_amount;

            int total_size = 0;
            for (const health_part_t &p : h->parts) {
                total_size += p.size;
            }

            std::cout << name->first_name << " suffers " << msg.damage_amount << " points of damage. Source: " << msg.damage_type << "\n";
            if (h->current_hitpoints < 1) {
                if (h->current_hitpoints > -10) {
                    h->unconscious = true;
                    std::cout << name->first_name << " is unconscious!\n";
                } else {
                    std::cout << name->first_name << " is dead!\n";
                    emit(entity_slain_message{msg.victim});
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
                        std::cout << " - head trauma results in unconsciousness.\n";
                        h->unconscious = true;
                    } else if (hit_part->part == "head" && hit_part->current_hitpoints < -9) {
                        std::cout << " - head trauma results in instant death!\n";
                        emit(entity_slain_message{msg.victim});
                    } else if (hit_part->current_hitpoints > -10) {
                        std::cout << " - the victim passes out from " << hit_part->part << " trauma.\n";
                        h->unconscious = true;
                    } else {
                        std::cout << " - the victim dies of " << hit_part->part << " trauma.\n";
                        emit(entity_slain_message{msg.victim});
                    }
                }
            }
            position_t * pos = entity(msg.victim)->component<position_t>();
            current_region->blood_stains[mapidx(pos->x, pos->y, pos->z)] = true;

        }

		damage->pop();
	}

    std::queue<entity_slain_message> * deaths = mbox<entity_slain_message>();
	while (!deaths->empty()) {
        entity_slain_message msg = deaths->front();
        deaths->pop();

        entity_t * victim = entity(msg.victim);
        position_t * pos = victim->component<position_t>();
        // Any items carried are dropped
        each<item_carried_t>([&msg, pos] (entity_t &e, item_carried_t &item) {
            if (item.carried_by == msg.victim) {
                emit(drop_item_message{e.id, pos->x, pos->y, pos->z});
            }
        });
                
        // Spawn a body
        if (victim->component<settler_ai_t>() != nullptr) {
            // It's a dead settler, we create a special item
            auto corpse = create_entity()
                ->assign(position_t{ pos->x, pos->y, pos->z })
                ->assign(renderable_t{ 2, rltk::colors::YELLOW, rltk::colors::RED });
        } else {
            // TODO: Normal corpses!
        }

        // Remove the entity
        entity_octree.remove_node(octree_location_t{pos->x, pos->y, pos->z, msg.victim});
        delete_entity(msg.victim);
    }
}
