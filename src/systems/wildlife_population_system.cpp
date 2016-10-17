#include "wildlife_population_system.hpp"
#include "../raws/raws.hpp"
#include "../main/game_globals.hpp"
#include "../raws/health_factory.hpp"
#include "../messages/messages.hpp"
#include "../components/position.hpp"
#include "../components/species.hpp"
#include "../components/wildlife_group.hpp"
#include "../components/game_stats.hpp"
#include "../components/grazer_ai.hpp"
#include "../components/viewshed.hpp"
#include "../components/settler_ai.hpp"
#include "../components/sentient_ai.hpp"
#include "../components/renderable.hpp"
#include <array>

std::array<uint8_t, 4> group_populations;

void wildlife_population_system::wander_randomly(entity_t &entity, position_t &original) {
    emit_deferred(entity_wants_to_move_randomly_message{entity.id});
}

void wildlife_population_system::configure() {
    system_name = "Wildlife Spawner";
    subscribe_mbox<hour_elapsed_message>();
    subscribe<tick_message>([this](tick_message &msg) {
        parallel_each<grazer_ai, position_t, viewshed_t>([this] (entity_t &e, grazer_ai &ai, position_t &pos, viewshed_t &view) {
            if (ai.initiative < 1) {
                // Can we see anything scary?
                bool terrified = false;
                float terror_distance = 1000.0F;
                std::size_t closest_fear = 0;
                for (const std::size_t other_entity : view.visible_entities) {
                    if (!entity(other_entity)) break;
                    if (entity(other_entity)->component<settler_ai_t>() || entity(other_entity)->component<sentient_ai>()) {
                        terrified = true;
                        auto other_pos = entity(other_entity)->component<position_t>();
                        const float d = distance3d(pos.x, pos.y, pos.z, other_pos->x, other_pos->y, other_pos->z);
                        if (d < terror_distance) {
                            terror_distance = d;
                            closest_fear = other_entity;
                        }
                    }
                }
                if (!terrified) {
                    // Handle the AI here
                    const int idx = mapidx(pos.x, pos.y, pos.z);
                    if (current_region->tile_vegetation_type[idx] > 0) {
                        if (rng.roll_dice(1,6)==1) emit_deferred(vegetation_damage_message{idx, 1});                         
                    } else {
                        this->wander_randomly(e, pos);
                    }
                } else {
                    // Poor creature is scared!
                    if (terror_distance < 1.5F) {
                        // Attack the target
                        auto health = entity(e.id)->component<health_t>();
                        if (health) {
                            if (!health->unconscious) emit_deferred(creature_attack_message{e.id, closest_fear});
                        }
                    } else {
                        emit_deferred(entity_wants_to_flee_message{e.id, closest_fear});
                    }
                }

                ai.initiative = std::max(1, rng.roll_dice(1, 12) - ai.initiative_modifier);
            } else {
                --ai.initiative;
            }
        });
    });
}

void wildlife_population_system::count_wildlife_populations() {
    std::fill(group_populations.begin(), group_populations.end(), 0);
    each<wildlife_group>([] (entity_t &e, wildlife_group &w) {
        ++group_populations[w.group_id];
    });
}

void wildlife_population_system::spawn_wildlife() {
    for (uint8_t i=0; i<4; ++i) {
        if (group_populations[i] == 0) {
            const std::size_t biome_type = planet.biomes[current_region->biome_idx].type;
            const std::size_t n_critters = biome_defs[biome_type].wildlife.size();
            const std::size_t critter_idx = rng.roll_dice(1, n_critters)-1;
            const std::string critter_tag = biome_defs[biome_type].wildlife[critter_idx];
            auto critter_def = creature_defs.find( critter_tag );
            if (critter_def == creature_defs.end()) throw std::runtime_error("Could not find " + biome_defs[biome_type].wildlife[critter_idx]);
            const int n_spawn = rng.roll_dice(critter_def->second.group_size_n_dice, critter_def->second.group_size_dice) + critter_def->second.group_size_mod;

            int edge = rng.roll_dice(1,4)-1;
            int base_x, base_y, base_z;

            int try_count=0;

            while (try_count < 4) {
                switch (edge) {
                    case 0 : { base_x = REGION_WIDTH/2; base_y = 1; } break;
                    case 1 : { base_x = REGION_WIDTH/2; base_y = REGION_HEIGHT-2; } break;
                    case 2 : { base_x = 1; base_y = REGION_HEIGHT/2; } break;
                    case 3 : { base_x = REGION_WIDTH-2; base_y = REGION_HEIGHT/2; } break;
                }
                base_z = get_ground_z(*current_region, base_x, base_y);
                const int idx = mapidx(base_x, base_y, base_z);
                if (current_region->water_level[idx] > 0) {
                    ++try_count;
                    edge = rng.roll_dice(1,4)-1;
                } else {
                    try_count = 20;
                }
            }
            if (try_count < 20) {
                break;
            }

            for (int j=0; j<n_spawn; ++j) {
                // Critters have: appropriate AI component, wildlife_group, position, renderable, name, species, stats
                bool male = true;
                if (rng.roll_dice(1,4)<=2) male = false;

                position_t pos{base_x, base_y, base_z};
                renderable_t render{ critter_def->second.glyph, critter_def->second.fg, rltk::colors::BLACK };
                name_t name{};
                name.first_name = critter_def->second.name;
                if (male) {
                    name.last_name = critter_def->second.male_name;
                } else {
                    name.last_name = critter_def->second.female_name;
                }
                species_t species{};
                species.tag = critter_def->second.tag;
                if (male) { species.gender = MALE; } else { species.gender = FEMALE; }

                game_stats_t stats;
                stats.profession_tag = "Wildlife";
                stats.age = 1;
                for (auto it=critter_def->second.stats.begin(); it!=critter_def->second.stats.end(); ++it) {
                    if (it->first == "str") stats.strength = it->second;
                    if (it->first == "dex") stats.dexterity = it->second;
                    if (it->first == "con") stats.constitution = it->second;
                    if (it->first == "int") stats.intelligence = it->second;
                    if (it->first == "wis") stats.wisdom = it->second;
                    if (it->first == "cha") stats.charisma = it->second;
                }

                if (critter_def->second.ai == creature_grazer) {
                    auto critter = create_entity()
                        ->assign(std::move(pos))
                        ->assign(std::move(render))
                        ->assign(std::move(name))
                        ->assign(std::move(species))
                        ->assign(std::move(create_health_component_creature(critter_def->second.tag)))
                        ->assign(grazer_ai{ stat_modifier(stats.dexterity) })
                        ->assign(std::move(stats))
                        ->assign(viewshed_t(6, false, false))
                        ->assign(wildlife_group{i});
                        std::cout << "Spawning " << critter_tag << " on edge " << edge << "\n";
                    }
                }
        }
    }
}

void wildlife_population_system::update(const double ms) {
    if (first_run) {
        // Check existing population groups
        count_wildlife_populations();

        // If there are none, then run the re-stocker
        spawn_wildlife();

        first_run = false;
    }

    std::queue<hour_elapsed_message> * hour = mbox<hour_elapsed_message>();
	while (!hour->empty()) {
        hour_elapsed_message msg = hour->front();
        hour->pop();

        count_wildlife_populations();
        spawn_wildlife();
    }
}

std::map<int, position_t> get_hunting_candidates(position_t &hunter_pos) {
    std::map<int, position_t> result;

    each<grazer_ai, position_t>([&result, &hunter_pos] (entity_t &e, grazer_ai &ai, position_t &pos) {
        result[distance3d_squared(hunter_pos.x, hunter_pos.y, hunter_pos.z, pos.x, pos.y, pos.z)] = pos;
    });

    return result;
}