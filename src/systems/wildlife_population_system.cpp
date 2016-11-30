#include "wildlife_population_system.hpp"
#include "../raws/raws.hpp"
#include "../raws/biomes.hpp"
#include "../raws/creatures.hpp"
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

void wildlife_population_system::configure() {
    system_name = "Wildlife Spawner";
    subscribe_mbox<hour_elapsed_message>();
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
            const std::size_t n_critters = get_biome_def(biome_type).wildlife.size();
            const std::size_t critter_idx = rng.roll_dice(1, (int)n_critters)-1;
            const std::string critter_tag = get_biome_def(biome_type).wildlife[critter_idx];
            auto critter_def = get_creature_def( critter_tag ).get();
            const int n_spawn = rng.roll_dice(critter_def.group_size_n_dice, critter_def.group_size_dice) + critter_def.group_size_mod;

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
                const auto idx = mapidx(base_x, base_y, base_z);
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
                renderable_t render{ critter_def.glyph, critter_def.fg, rltk::colors::BLACK };
                name_t name{};
                name.first_name = critter_def.name;
                if (male) {
                    name.last_name = critter_def.male_name;
                } else {
                    name.last_name = critter_def.female_name;
                }
                species_t species{};
                species.tag = critter_def.tag;
                if (male) { species.gender = MALE; } else { species.gender = FEMALE; }

                game_stats_t stats;
                stats.profession_tag = "Wildlife";
                stats.age = 1;
                for (auto it=critter_def.stats.begin(); it!=critter_def.stats.end(); ++it) {
                    if (it->first == "str") stats.strength = it->second;
                    if (it->first == "dex") stats.dexterity = it->second;
                    if (it->first == "con") stats.constitution = it->second;
                    if (it->first == "int") stats.intelligence = it->second;
                    if (it->first == "wis") stats.wisdom = it->second;
                    if (it->first == "cha") stats.charisma = it->second;
                }

                if (critter_def.ai == creature_grazer) {
                    auto critter = create_entity()
                        ->assign(std::move(pos))
                        ->assign(std::move(render))
                        ->assign(std::move(name))
                        ->assign(std::move(species))
                        ->assign(std::move(create_health_component_creature(critter_def.tag)))
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
        result[(int)distance3d_squared(hunter_pos.x, hunter_pos.y, hunter_pos.z, pos.x, pos.y, pos.z)] = pos;
    });

    return result;
}