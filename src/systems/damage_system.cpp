#include "damage_system.hpp"
#include "../messages/messages.hpp"
#include "../components/components.hpp"
#include "../main/game_globals.hpp"
#include "movement_system.hpp"
#include "weapons_helpers.hpp"
#include <rltk.hpp>
#include <iostream>
#include <sstream>

void damage_system::configure() {
    system_name = "Damage System";
    subscribe_mbox<inflict_damage_message>();
    subscribe_mbox<settler_attack_message>();
    subscribe_mbox<settler_ranged_attack_message>();
    subscribe_mbox<creature_attack_message>();
    subscribe_mbox<entity_slain_message>();
    subscribe_mbox<hour_elapsed_message>();
}

void damage_system::update(const double ms) {
    // Settler attacks - Ranged
    std::queue<settler_ranged_attack_message> * rattack = mbox<settler_ranged_attack_message>();
    while (!rattack->empty()) {
        settler_ranged_attack_message msg = rattack->front();
        rattack->pop();

        std::stringstream ss;
        auto attacker = entity(msg.attacker);
        auto defender = entity(msg.victim);
        auto attacker_stats = attacker->component<game_stats_t>();
        auto attacker_name = attacker->component<name_t>();
        auto defender_name = defender->component<name_t>();
        auto attacker_pos = attacker->component<position_t>();
        auto defender_pos = defender->component<position_t>();

        auto victim_ai = defender->component<sentient_ai>();
        if (victim_ai) {
            if (planet.civs.civs[planet.civs.unimportant_people[victim_ai->person_id].civ_id].cordex_feelings > -10) {
                --planet.civs.civs[planet.civs.unimportant_people[victim_ai->person_id].civ_id].cordex_feelings;
            }
        }

        std::size_t weapon_id, ammo_id;
        std::tie(weapon_id, ammo_id) = get_ranged_and_ammo_id(msg.attacker);
        std::string weapon_name = "fists";
        int weapon_n = 1;
        int weapon_d = 4;
        int weapon_mod = 0;
        if (weapon_id != 0) {
            auto weapon_component = entity(weapon_id)->component<item_t>();
            if (weapon_component) {
                auto weapon_finder = item_defs.find(weapon_component->item_tag);
                if (weapon_finder != item_defs.end()) {
                    weapon_name = weapon_finder->second.name;
                }
            }
        }
        if (ammo_id != 0) {
            auto ammo_component = entity(ammo_id)->component<item_t>();
            if (ammo_component) {
                auto ammo_finder = item_defs.find(ammo_component->item_tag);
                if (ammo_finder != item_defs.end()) {
                    weapon_n = ammo_finder->second.damage_n;
                    weapon_d = ammo_finder->second.damage_d;
                    weapon_mod = ammo_finder->second.damage_mod;
                }
                --ammo_component->stack_size;
                if (ammo_component->stack_size < 1) delete_entity(ammo_id);
            }
            emit(emit_particles_message{3, attacker_pos->x, attacker_pos->y, attacker_pos->z, 
                    defender_pos->x, defender_pos->y, defender_pos->z});
        }

        ss << attacker_name->first_name << " attacks " << defender_name->first_name << " with their " << weapon_name << ". ";
        const int die_roll = rng.roll_dice(1, 20) + stat_modifier(attacker_stats->dexterity);
        ss << "(Dice roll: " << die_roll << ") ";
        if (die_roll > calculate_armor_class(*defender)) {
            ss << "The attack hits, ";
            const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod + stat_modifier(attacker_stats->strength));
            ss << "for " << damage << " points of damage.\n";
            emit(inflict_damage_message{ msg.victim, damage, weapon_name });
        } else {
            ss << "The attack misses.\n";
        }

        std::cout << ss.str();
    }

    // Settler attacks - Melee
    std::queue<settler_attack_message> * sattack = mbox<settler_attack_message>();
    while (!sattack->empty()) {
        settler_attack_message msg = sattack->front();
        sattack->pop();

        std::stringstream ss;
        auto attacker = entity(msg.attacker);
        auto defender = entity(msg.victim);
        if (!attacker || !defender) break;
        auto attacker_stats = attacker->component<game_stats_t>();
        auto attacker_name = attacker->component<name_t>();
        auto defender_name = defender->component<name_t>();

        auto victim_ai = defender->component<sentient_ai>();
        if (victim_ai) {
            if (planet.civs.civs[planet.civs.unimportant_people[victim_ai->person_id].civ_id].cordex_feelings > -10) {
                --planet.civs.civs[planet.civs.unimportant_people[victim_ai->person_id].civ_id].cordex_feelings;
            }
        }

        std::size_t weapon_id = get_melee_id(msg.attacker);
        std::string weapon_name = "fists";
        int weapon_n = 1;
        int weapon_d = 4;
        int weapon_mod = 0;
        if (weapon_id != 0) {
            auto weapon_component = entity(weapon_id)->component<item_t>();
            if (weapon_component) {
                auto weapon_finder = item_defs.find(weapon_component->item_tag);
                if (weapon_finder != item_defs.end()) {
                    weapon_name = weapon_finder->second.name;
                    weapon_n = weapon_finder->second.damage_n;
                    weapon_d = weapon_finder->second.damage_d;
                    weapon_mod = weapon_finder->second.damage_mod;
                }
            }
        }
        ss << attacker_name->first_name << " attacks " << defender_name->first_name << " with their " << weapon_name << ". ";
        const int die_roll = rng.roll_dice(1, 20) + stat_modifier(attacker_stats->strength);
        ss << "(Dice roll: " << die_roll << ") ";
        if (die_roll > calculate_armor_class(*defender)) {
            ss << "The attack hits, ";
            const int damage = std::max(1, rng.roll_dice(weapon_n, weapon_d) + weapon_mod + stat_modifier(attacker_stats->strength));
            ss << "for " << damage << " points of damage.\n";
            emit(inflict_damage_message{ msg.victim, damage, weapon_name });
        } else {
            ss << "The attack misses.\n";
        }

        std::cout << ss.str();
    }

    // Creature attacks
    std::queue<creature_attack_message> * cattack = mbox<creature_attack_message>();
    while (!cattack->empty()) {
        creature_attack_message msg = cattack->front();
        cattack->pop();

        // Start by determining the attack
        std::stringstream ss;
        auto attacker = entity(msg.attacker);
        if (!attacker) break;
        auto attack_species = attacker->component<species_t>();
        auto attacker_name = attacker->component<name_t>();
        auto creature = creature_defs.find(attack_species->tag);

        auto defender = entity(msg.victim);
        if (!defender) break;
        auto defender_name = defender->component<name_t>();
        auto defender_stats = defender->component<game_stats_t>();

        if (creature == creature_defs.end()) {
            throw std::runtime_error("Undefined species - " + attack_species->tag);
        }
        for (const creature_attack_t &weapon : creature->second.attacks) {
            ss << attacker_name->first_name << " attacks " << defender_name->first_name << " with its " << weapon.type << ".";
            const int hit_roll = rng.roll_dice(1,20) + weapon.hit_bonus;
            const int target = calculate_armor_class(*defender) + stat_modifier(defender_stats->dexterity);
            if (hit_roll < target) {
                ss << " The attack misses (roll: " << hit_roll << ").\n";
            } else {
                const int damage = std::max(1,rng.roll_dice(weapon.damage_n_dice, weapon.damage_dice) + weapon.damage_mod);
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
        if (!entity(msg.victim)) break;
        auto h = entity(msg.victim)->component<health_t>();
        auto name = entity(msg.victim)->component<name_t>();

        if (h) {
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
                        std::cout << " - head trauma results in unconsciousness.\n";
                        h->unconscious = true;
                    } else if (hit_part->part == "head" && hit_part->current_hitpoints < -9) {
                        std::cout << " - head trauma results in instant death!\n";
                        emit(entity_slain_message{msg.victim, msg.damage_type});
                    } else if (hit_part->current_hitpoints > -10) {
                        std::cout << " - the victim passes out from " << hit_part->part << " trauma.\n";
                        h->unconscious = true;
                    } else {
                        std::cout << " - the victim dies of " << hit_part->part << " trauma.\n";
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

    std::queue<hour_elapsed_message> * hour = mbox<hour_elapsed_message>();
	while (!hour->empty()) {
        hour_elapsed_message msg = hour->front();
        hour->pop();

        each<health_t>([] (entity_t &e, health_t &h) {
            if (h.max_hitpoints > h.current_hitpoints) {
                ++h.current_hitpoints;
                h.unconscious = false;
            }
        });
    }
}
