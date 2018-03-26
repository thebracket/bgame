#include "stdafx.h"
#include "ai_status_effects.hpp"
#include "../../global_assets/game_planet.hpp"
#include "../../raws/biomes.hpp"
#include "../../raws/defs/biome_type_t.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/creatures.hpp"
#include "../../raws/defs/raw_creature_t.hpp"
#include "../../global_assets/rng.hpp"
#include "../../raws/health_factory.hpp"
#include "../../bengine/telemetry.hpp"
#include "distance_map_system.hpp"
#include "../../bengine/color_t.hpp"

namespace systems {
	namespace wildlife_population {

		using namespace bengine;

		std::array<uint8_t, 4> group_populations;
		bool first_run = true;

		void count_wildlife_populations() {
			std::fill(group_populations.begin(), group_populations.end(), 0);
			each<wildlife_group>([](entity_t &e, wildlife_group &w) {
				++group_populations[w.group_id];
			});
		}

		void spawn_wildlife() {
			for (uint8_t i = 0; i<4; ++i) {
				if (group_populations[i] == 0) {
					const std::size_t biome_type = planet.biomes[region::get_biome_idx()].type;
					const std::size_t n_critters = get_biome_def(biome_type)->wildlife.size();
					const std::size_t critter_idx = rng.roll_dice(1, (int)n_critters) - 1;
					const std::string critter_tag = get_biome_def(biome_type)->wildlife[critter_idx];
					auto critter_def = *get_creature_def(critter_tag);
					const int n_spawn = rng.roll_dice(critter_def.group_size_n_dice, critter_def.group_size_dice) + critter_def.group_size_mod;

					int edge = rng.roll_dice(1, 4) - 1;
					int base_x, base_y, base_z;

					int try_count = 0;

					while (try_count < 4) {
						switch (edge) {
						case 0: { base_x = REGION_WIDTH / 2; base_y = 1; } break;
						case 1: { base_x = REGION_WIDTH / 2; base_y = REGION_HEIGHT - 2; } break;
						case 2: { base_x = 1; base_y = REGION_HEIGHT / 2; } break;
						case 3: { base_x = REGION_WIDTH - 2; base_y = REGION_HEIGHT / 2; } break;
						}
						base_z = region::ground_z(base_x, base_y);
						const auto idx = mapidx(base_x, base_y, base_z);
						if (region::water_level(idx) > 0) {
							++try_count;
							edge = rng.roll_dice(1, 4) - 1;
						}
						else {
							try_count = 20;
						}
					}
					if (try_count < 20) {
						break;
					}

					for (int j = 0; j<n_spawn; ++j) {
						// Critters have: appropriate AI component, wildlife_group, position, renderable, name, species, stats
						bool male = true;
						if (rng.roll_dice(1, 4) <= 2) male = false;

						position_t pos{ base_x, base_y, base_z };
						renderable_t render{ critter_def.glyph, critter_def.glyph_ascii, critter_def.fg, color_t(0.0F,0.0F,0.0F), critter_def.vox };
						name_t name{};
						name.first_name = critter_def.name;
						if (male) {
							name.last_name = critter_def.male_name;
						}
						else {
							name.last_name = critter_def.female_name;
						}
						species_t species{};
						species.tag = critter_def.tag;
						if (male) { species.gender = MALE; }
						else { species.gender = FEMALE; }

						game_stats_t stats;
						stats.profession_tag = "Wildlife";
						stats.age = 1;
						for (auto it = critter_def.stats.begin(); it != critter_def.stats.end(); ++it) {
							if (it->first == "str") stats.strength = it->second;
							if (it->first == "dex") stats.dexterity = it->second;
							if (it->first == "con") stats.constitution = it->second;
							if (it->first == "int") stats.intelligence = it->second;
							if (it->first == "wis") stats.wisdom = it->second;
							if (it->first == "cha") stats.charisma = it->second;
						}

						if (critter_def.ai == creature_grazer) {
							auto new_entity = create_entity();
							std::cout << new_entity->id << "\n";
							new_entity->assign(std::move(pos));
							new_entity->assign(std::move(render));
							new_entity->assign(std::move(name));
							new_entity->assign(std::move(species));
							new_entity->assign(create_health_component_creature(critter_def.tag));
							new_entity->assign(grazer_ai{});
							new_entity->assign(std::move(stats));
							new_entity->assign(viewshed_t(6, false, false));
							new_entity->assign(wildlife_group{ i });
							new_entity->assign(initiative_t{});
							new_entity->assign(ai_mode_idle_t{});
							//std::cout << "Spawning " << critter_tag << " on edge " << edge << "\n";
							call_home("Spawn", "Creature", critter_tag);
						}
					}
				}
			}
		}

		void run(const double &duration_ms) {
			if (first_run) {
				// Check existing population groups
				count_wildlife_populations();

				// If there are none, then run the re-stocker
				spawn_wildlife();

				first_run = false;
			}

			count_wildlife_populations();
			spawn_wildlife();
		}
	}
}
