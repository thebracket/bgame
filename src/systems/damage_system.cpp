#include "damage_system.hpp"
#include "../messages/inflict_damage_message.hpp"
#include "../components/health.hpp"
#include "../components/name.hpp"
#include "../game_globals.hpp"
#include <rltk.hpp>
#include <iostream>

void damage_system::configure() {
    subscribe_mbox<inflict_damage_message>();
}

void damage_system::update(const double ms) {
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

            std::cout << name->first_name << " suffers " << msg.damage_amount << ". Source: " << msg.damage_type << "\n";
            if (h->current_hitpoints < 0) std::cout << " - the victim would be unconscious or dead.\n";

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
                if (hit_part->current_hitpoints < 0) std::cout << " - the " << hit_part->part << " would be impaired/gone.\n";
            }

        }

		damage->pop();
	}
}
