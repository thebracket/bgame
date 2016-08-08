#include "wildlife_population_system.hpp"
#include "../raws/raws.hpp"
#include "../game_globals.hpp"
#include "../components/components.hpp"
#include "../raws/health_factory.hpp"
#include "../messages/messages.hpp"
#include <array>

std::array<uint8_t, 4> group_populations;

void wildlife_population_system::wander_randomly(entity_t &entity, position_t &original) {
	position_t pos = original;
	const int tile_index = mapidx(pos.x, pos.y, pos.z);
	const int direction = rng.roll_dice(1,6);
	switch (direction) {
		case 1 : if (current_region->tile_flags[tile_index].test(CAN_GO_UP)) pos.z++; break;
		case 2 : if (current_region->tile_flags[tile_index].test(CAN_GO_DOWN)) pos.z--; break;
		case 3 : if (current_region->tile_flags[tile_index].test(CAN_GO_NORTH)) pos.y--; break;
		case 4 : if (current_region->tile_flags[tile_index].test(CAN_GO_SOUTH)) pos.y++; break;
		case 5 : if (current_region->tile_flags[tile_index].test(CAN_GO_EAST)) pos.x++; break;
		case 6 : if (current_region->tile_flags[tile_index].test(CAN_GO_WEST)) pos.x--; break;
	}
	if (current_region->solid[tile_index]) { 
		pos = original;
	} else {
		move_to(entity, original, pos);
	}
}

void wildlife_population_system::move_to(entity_t &e, position_t &pos, position_t &destination) {
	pos.x = destination.x;
	pos.y = destination.y;
	pos.z = destination.z;
	emit(entity_moved_message{e.id, destination});
	emit(renderables_changed_message{});
}

void wildlife_population_system::configure() {
    subscribe<tick_message>([this](tick_message &msg) {
        each<grazer_ai, position_t>([this] (entity_t &e, grazer_ai &ai, position_t &pos) {
            if (ai.initiative < 1) {
                // Handle the AI here
                this->wander_randomly(e, pos);

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
    for (std::size_t i=0; i<4; ++i) {
        if (group_populations[i] == 0) {
            const std::size_t biome_type = planet.biomes[current_region->biome_idx].type;
            const std::size_t n_critters = biome_defs[biome_type].wildlife.size();
            const std::size_t critter_idx = rng.roll_dice(1, n_critters)-1;
            const std::string critter_tag = biome_defs[biome_type].wildlife[critter_idx];
            std::cout << "Looking for [" << critter_tag << "]";
            auto critter_def = creature_defs.find( critter_tag );
            if (critter_def == creature_defs.end()) throw std::runtime_error("Could not find " + biome_defs[biome_type].wildlife[critter_idx]);
            const int n_spawn = rng.roll_dice(critter_def->second.group_size_n_dice, critter_def->second.group_size_dice) + critter_def->second.group_size_mod;
            std::cout << "Spawning " << n_spawn << " " << critter_def->second.tag << ", ";

            int edge = rng.roll_dice(1,4);
            int base_x, base_y;

            switch (edge) {
                case 0 : { base_x = REGION_WIDTH/2; base_y = 1; } break;
                case 1 : { base_x = REGION_WIDTH/2; base_y = REGION_HEIGHT-1; } break;
                case 2 : { base_x = 1; base_y = REGION_HEIGHT/2; } break;
                case 3 : { base_x = REGION_WIDTH-1; base_y = REGION_HEIGHT/2; } break;
            }
            std::cout << "on edge " << edge << "\n";

            for (int j=0; j<n_spawn; ++j) {
                // Critters have: appropriate AI component, wildlife_group, position, renderable, name, species, stats
                bool male = true;
                if (rng.roll_dice(1,4)<=2) male = false;

                position_t pos{base_x, base_y, get_ground_z(*current_region, base_x, base_y)};
                renderable_t render{ critter_def->second.glyph, rltk::colors::LIGHT_GREY, rltk::colors::BLACK };
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
                        ->assign(std::move(stats));
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
}
