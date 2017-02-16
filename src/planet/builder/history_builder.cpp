#include "history_builder.hpp"
#include "../planet_builder.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/species.hpp"
#include "../../raws/string_table.hpp"
#include "../constants.hpp"
#include "../../utils/string_utils.hpp"
#include "../../components/game_stats.hpp"
#include "../../utils/sparsepp.h"
#include <unordered_map>
#include <iostream>
#include <sstream>

namespace history_builder {

    constexpr int n_civs = WORLD_WIDTH;

    struct build_region_t {
        build_region_t() {}
        spp::sparse_hash_map<std::size_t, spp::sparse_hash_set<std::size_t>> units_present;
        std::size_t town_idx = 0;
    };

    struct build_map_t {
        spp::sparse_hash_map<int, build_region_t> regions;
    };

    std::unique_ptr<build_map_t> planet_build_map;

    /* Build map accessors */
    void record_town(const int &pidx, const std::size_t town_idx) {
        planet_build_map->regions[pidx].town_idx = town_idx;
    }

    void record_arrival(const int &pidx, const civ_person_t &peep, const int &i) {
        //std::cout << "Peep " << i << " of civ " << peep.civ_id << " is entering tile " << pidx << "\n";
        assert(peep.civ_id < n_civs);
        planet_build_map->regions[pidx].units_present[peep.civ_id].insert(i);
    }

    void record_departure(const int &pidx, const civ_person_t &peep, const int &i) {
        //std::cout << "Peep " << i << " of civ " << peep.civ_id << " is departing tile " << pidx << "\n";
        assert(peep.civ_id < n_civs);
        planet_build_map->regions[pidx].units_present[peep.civ_id].erase(i);
        if (planet_build_map->regions[pidx].units_present[peep.civ_id].empty()) {
            planet_build_map->regions[pidx].units_present.erase(peep.civ_id);
        }
    }

    /* Utility Functions */
    int hb_stat_mod(const raw_civilized_t &species, const std::string &stat) {
        auto finder = species.stat_mods.find(stat);
        if (finder == species.stat_mods.end()) return 0;
        return finder->second;
    }

    std::string random_species(rltk::random_number_generator &rng, const int desired_tech_level = 0) {
        const std::size_t size = civ_defs.size();
        while (true) {
            const auto random = rng.roll_dice(1, size) - 1;
            auto i = 0;
            for (auto it = civ_defs.begin(); it != civ_defs.end(); ++it) {
                if (i == random && it->second.tech_level == desired_tech_level) {
                    return it->second.tag;
                }
                ++i;
            }
        }
    }

    std::pair<int, int> find_starting_loc(planet_t &planet, random_number_generator &rng) {
        std::pair<int, int> starting_loc;
        bool found = false;
        while (!found) {
            starting_loc = {rng.roll_dice(1, WORLD_WIDTH) - 1, rng.roll_dice(1, WORLD_HEIGHT) - 1};
            if (planet.landblocks[planet.idx(starting_loc.first, starting_loc.second)].type != block_type::WATER) {
                bool dupe = false;
                for (const auto &s : planet.civs.settlements) {
                    if (s.world_x == starting_loc.first && s.world_y == starting_loc.second) dupe = true;
                }
                if (!dupe) found = true;
            }
        }
        return starting_loc;
    }

    settlement_t *build_settlement(planet_t &planet, const std::size_t &i, raw_civilized_t &species,
                                   const std::pair<int, int> &starting_loc) {
        settlement_t town;
        town.civ_id = i;
        if (species.ethics.mentality == "hive") {
            town.name = species.name + std::string(" Hive");
            town.blight_level = 100;
        } else {
            town.name = std::string("Settlement #") + std::to_string(planet.civs.settlements.size());
        }
        town.world_x = starting_loc.first;
        town.world_y = starting_loc.second;
        town.status = 2;
        planet.civs.settlements.push_back(town);

        const int pidx = planet.idx(town.world_x, town.world_y);
        settlement_t * result = &planet.civs.settlements[planet.civs.settlements.size()-1];
        assert(result->name == town.name);
        record_town(pidx, planet.civs.settlements.size()-1);
        return result;
    }

    void planet_spawn_minimums(planet_t &planet, random_number_generator &rng, settlement_t &town,
                               raw_civilized_t &species) {
        //if (town.civ_id == 0) return;
        for (auto it = species.castes.begin(); it != species.castes.end(); ++it) {
            if (it->second.min_per_occupied_region > 0) {
                for (int i = 0; i < it->second.min_per_occupied_region; ++i) {
                    civ_person_t peep;
                    peep.civ_id = town.civ_id;
                    peep.world_x = town.world_x;
                    peep.world_y = town.world_y;
                    peep.species_tag = planet.civs.civs[town.civ_id].species_tag;
                    peep.behavior = species.ethics.behavior;
                    peep.caste = it->first;
                    if (rng.roll_dice(1, 2) == 1) {
                        peep.male = true;
                    } else {
                        peep.male = false;
                    }
                    peep.married = false;
                    peep.married_to = 0;
                    peep.deceased = false;
                    peep.age = rng.roll_dice(1, species.max_age - 1);
                    peep.level = species.castes[peep.caste].starting_level;
                    peep.str = rng.roll_dice(3, 6) + hb_stat_mod(species, "str");
                    peep.dex = rng.roll_dice(3, 6) + hb_stat_mod(species, "dex");
                    peep.con = rng.roll_dice(3, 6) + hb_stat_mod(species, "con");
                    peep.intelligence = rng.roll_dice(3, 6) + hb_stat_mod(species, "int");
                    peep.wis = rng.roll_dice(3, 6) + hb_stat_mod(species, "wis");
                    peep.cha = rng.roll_dice(3, 6) + hb_stat_mod(species, "cha");
                    peep.com = rng.roll_dice(3, 6) + hb_stat_mod(species, "com");
                    peep.ethics = rng.roll_dice(3, 6) + hb_stat_mod(species, "eth");

                    peep.hit_points = 10 * peep.level;

                    planet.civs.population.push_back(peep);
                    const std::size_t idx = planet.civs.population.size()-1;
                    const int pidx = planet.idx(peep.world_x, peep.world_y);
                    record_arrival(pidx, peep, idx);
                }
            }
        }
    }

    void planet_build_spawn_sentient(planet_t &planet, random_number_generator &rng, std::size_t &i, civ_t &civ,
                                     raw_civilized_t &species,
                                     std::vector<std::tuple<int, int, int, std::string>> &available_castes,
                                     std::pair<int, int> &starting_loc, bool newborn = false) {
        civ_person_t peep;
        peep.civ_id = i;
        peep.species_tag = civ.species_tag;
        peep.behavior = species.ethics.behavior;

        bool picked_caste = false;
        while (!picked_caste) {
            int random = rng.roll_dice(1, available_castes.size()) - 1;

            const int n_present = std::get<0>(available_castes[random]);
            const int min_present = std::get<1>(available_castes[random]);
            const int max_present = std::get<2>(available_castes[random]);

            //std::cout << "Evaluating " << std::get<3>(available_castes[random]) << ", " << n_present << "/" << min_present << "\n";

            if (min_present > 0) {
                if (n_present < min_present && n_present <= max_present) {
                    ++std::get<0>(available_castes[random]);
                    picked_caste = true;
                    peep.caste = std::get<3>(available_castes[random]);
                }
            } else {
                ++std::get<0>(available_castes[random]);
                picked_caste = true;
                peep.caste = std::get<3>(available_castes[random]);
            }
            if (!picked_caste) random = rng.roll_dice(1, available_castes.size()) - 1;
        }

        peep.world_x = starting_loc.first;
        peep.world_y = starting_loc.second;
        if (rng.roll_dice(1, 2) == 1) {
            peep.male = true;
        } else {
            peep.male = false;
        }
        peep.married = false;
        peep.married_to = 0;
        peep.deceased = false;
        if (!newborn) {
            peep.age = rng.roll_dice(1, species.max_age - 1);
        } else {
            peep.age = 0;
        }
        peep.level = species.castes[peep.caste].starting_level;
        peep.str = rng.roll_dice(3, 6) + hb_stat_mod(species, "str");
        peep.dex = rng.roll_dice(3, 6) + hb_stat_mod(species, "dex");
        peep.con = rng.roll_dice(3, 6) + hb_stat_mod(species, "con");
        peep.intelligence = rng.roll_dice(3, 6) + hb_stat_mod(species, "int");
        peep.wis = rng.roll_dice(3, 6) + hb_stat_mod(species, "wis");
        peep.cha = rng.roll_dice(3, 6) + hb_stat_mod(species, "cha");
        peep.com = rng.roll_dice(3, 6) + hb_stat_mod(species, "com");
        peep.ethics = rng.roll_dice(3, 6) + hb_stat_mod(species, "eth");

        peep.hit_points = 10 * peep.level;

        planet.civs.population.push_back(peep);
        const int pidx = planet.idx(peep.world_x, peep.world_y);
        const int idx = planet.civs.population.size()-1;
        record_arrival(pidx, peep, idx);
    }

    void add_civ(planet_t &planet, rltk::random_number_generator &rng, std::size_t &i) {
        civ_t civ;

        civ.species_tag = random_species(rng);
        auto species = civ_defs.find(civ.species_tag)->second;
        civ.name = species.name;
        civ.tech_level = 0;
        civ.met_cordex = false;

        civ.r = rng.roll_dice(1, 254);
        civ.g = rng.roll_dice(1, 254);
        civ.b = rng.roll_dice(1, 254);
        civ.glyph = species.glyph;

        auto starting_loc = find_starting_loc(planet, rng);

        // Build the initial settlement
        build_settlement(planet, i, species, starting_loc);

        // Generate an initial population
        std::vector<std::tuple<int, int, int, std::string>> available_castes;
        for (auto &caste : species.castes) {
            available_castes.push_back(
                    std::make_tuple(0, caste.second.min_per_occupied_region, caste.second.max_per_region, caste.first));
        }

        const int n_peeps = rng.roll_dice(10, 20);
        for (int j = 0; j < n_peeps; ++j) {
            planet_build_spawn_sentient(planet, rng, i, civ, species, available_castes, starting_loc);
        }
        planet.civs.civs.push_back(civ);
    }

    void evaluate_marriage(planet_t &planet, const int pidx, civ_person_t &peep, const raw_civilized_t &species,
                           const std::size_t &i) {
        auto region_finder = planet_build_map->regions.find(pidx);
        if (region_finder == planet_build_map->regions.end()) return;
        auto civ_finder = region_finder->second.units_present.find(peep.civ_id);
        if (civ_finder == region_finder->second.units_present.end()) return;

        for (auto it = civ_finder->second.begin(); it != civ_finder->second.end(); ++it) {
            auto &other_peep = planet.civs.population[*it];
            if (!other_peep.deceased && !other_peep.married && other_peep.age > species.child_age &&
                peep.male != other_peep.male) {
                peep.married = true;
                other_peep.married = true;
                peep.married_to = *it;
                other_peep.married_to = i;
                return;
            }
        }
    }

    spp::sparse_hash_map<std::string, std::vector<std::tuple<int, int, int, std::string>>> caste_cache;

    std::vector<std::tuple<int, int, int, std::string>> get_available_castes(planet_t &planet, const raw_civilized_t &species_def, civ_person_t &peep)
    {
        auto cache_finder = caste_cache.find(species_def.tag);
        if (cache_finder != caste_cache.end()) return cache_finder->second;

        std::vector<std::tuple<int, int, int, std::string>> available_castes;
        for (auto &caste : species_def.castes) {
            available_castes.push_back(
                    std::make_tuple(0, caste.second.min_per_occupied_region, caste.second.max_per_region,
                                    caste.first));
        }
        for (const auto &p : planet.civs.population) {
            if (!p.deceased && p.civ_id == peep.civ_id & p.world_x == peep.world_x &&
                p.world_y == peep.world_y) {
                for (auto &c : available_castes) {
                    if (std::get<3>(c) == p.caste) std::get<0>(c)++;
                }
            }
        }
        caste_cache[species_def.tag] = available_castes;
        return available_castes;
    }

    void hatch_eggs(planet_t &planet, random_number_generator &rng, raw_civilized_t &species_def, civ_person_t &peep) {
        std::vector<std::tuple<int, int, int, std::string>> available_castes = get_available_castes(planet, species_def,
                                                                                                    peep);

        const int max_hatchlings = species_def.clutch_size_max;
        const int min_hatchlings = species_def.clutch_size_min;
        const int hatch_range = max_hatchlings - min_hatchlings;
        const int n_hatchlings = rng.roll_dice(1, hatch_range) + min_hatchlings;

        // Now we need to create n_hatchlings
        for (int j = 0; j < n_hatchlings; ++j) {
            std::size_t civ_id = peep.civ_id;
            std::pair<int, int> loc = std::make_pair(peep.world_x, peep.world_y);
            planet_build_spawn_sentient(planet, rng, civ_id, planet.civs.civs[peep.civ_id],
                                        species_def, available_castes, loc, true);
        }
    }

    void give_birth(planet_t &planet, random_number_generator &rng, raw_civilized_t &species_def, civ_person_t &peep,
                    spp::sparse_hash_set<std::size_t> &dead_peeps, const std::size_t peep_id)
    {
        if (rng.roll_dice(1,4)>1) return;

        const int chance_of_death = peep.con + planet.civs.civs[peep.civ_id].tech_level;
        const int roll = rng.roll_dice(1,20);
        if (roll > chance_of_death) dead_peeps.insert(peep_id);

        std::vector<std::tuple<int, int, int, std::string>> available_castes = get_available_castes(planet, species_def,
                                                                                                    peep);
        const int n_births = std::max(1, rng.roll_dice(1, 4) - 1);
        for (int j = 0; j < n_births; ++j) {
            std::size_t civ_id = peep.civ_id;
            std::pair<int, int> loc = std::make_pair(peep.world_x, peep.world_y);
            planet_build_spawn_sentient(planet, rng, civ_id, planet.civs.civs[peep.civ_id],
                                        species_def, available_castes, loc, true);
        }
    }

    void planet_build_evaluate_move_option(std::vector<int> &candidates, planet_t &planet, const int &idx) {
        if (planet.landblocks[idx].type == block_type::WATER) return;

        int weight = 1;
        if (planet.landblocks[idx].type == block_type::PLAINS) weight = 4;
        if (planet.landblocks[idx].type == block_type::HILLS) weight = 2;
        if (planet.landblocks[idx].type == block_type::MOUNTAINS) weight = 1;
        if (planet.landblocks[idx].type == block_type::MARSH) weight = 2;
        if (planet.landblocks[idx].type == block_type::PLATEAU) weight = 4;
        if (planet.landblocks[idx].type == block_type::HIGHLANDS) weight = 3;
        if (planet.landblocks[idx].type == block_type::COASTAL) weight = 4;
        if (planet.landblocks[idx].type == block_type::SALT_MARSH) weight = 1;

        for (int i = 0; i < weight; ++i) candidates.push_back(idx);
    }

    void move_peep(planet_t &planet, rltk::random_number_generator &rng,
                   civ_person_t &peep, std::size_t &peep_id) {
        std::vector<int> candidates;
        if (peep.world_y > 2)
            planet_build_evaluate_move_option(candidates, planet, planet.idx(peep.world_x, peep.world_y - 1));
        if (peep.world_y < WORLD_HEIGHT - 2)
            planet_build_evaluate_move_option(candidates, planet, planet.idx(peep.world_x, peep.world_y + 1));
        if (peep.world_x > 2)
            planet_build_evaluate_move_option(candidates, planet, planet.idx(peep.world_x - 1, peep.world_y));
        if (peep.world_x < WORLD_WIDTH - 2)
            planet_build_evaluate_move_option(candidates, planet, planet.idx(peep.world_x + 1, peep.world_y));

        if (!candidates.empty()) {
            const int roll = rng.roll_dice(1, (int) candidates.size()) - 1;
            const int destination = candidates[roll];
            const int prev_idx = planet.idx(peep.world_x, peep.world_y);
            peep.world_x = destination % WORLD_WIDTH;
            peep.world_y = destination / WORLD_WIDTH;
            const int pidx = planet.idx(peep.world_x, peep.world_y);
            record_departure(prev_idx, peep, peep_id);
            record_arrival(pidx, peep, peep_id);
        }
    }

    bool evaluate_movement(planet_t &planet, random_number_generator &rng, raw_civilized_t &species,
                           civ_person_t &peep, const caste_t &caste, const int &pidx, std::size_t &i) {
        auto region_finder = planet_build_map->regions.find(pidx);
        if (region_finder == planet_build_map->regions.end()) return false;

        const std::size_t town_idx = region_finder->second.town_idx;
        settlement_t *town = nullptr;
        if (town_idx > 0) town = &planet.civs.settlements[town_idx];
        if (town) {
            // We are in a town
            if (caste.spreads_blight) {
                if (town->blight_level < 99) {
                    // Blight the land!
                    ++town->blight_level;
                } else {
                    if (!caste.guard_only && rng.roll_dice(1, 100) < 50) {
                        move_peep(planet, rng, peep, i);
                        return true;
                    }
                }
            } else {
                if (town->blight_level > 0) {
                    // Clean up!
                    --town->blight_level;
                } else {
                    if (!caste.guard_only && rng.roll_dice(1, 100) < 50) {
                        move_peep(planet, rng, peep, i);
                        return true;
                    } else if (caste.researcher) {
                        ++planet.civs.civs[peep.civ_id].research_level;
                        return true;
                    }
                }
            }
        } else {
            // We are not in a town - so we need to create one!
            std::pair<int, int> loc = std::make_pair(peep.world_x, peep.world_y);
            town = build_settlement(planet, peep.civ_id, species, loc);
            planet_spawn_minimums(planet, rng, *town, species);
        }

        return false;
    }

    void build_improvements(planet_t &planet, caste_t &caste_def, const int &pidx) {
        if (caste_def.builds.empty()) return;
        auto region_finder = planet_build_map->regions.find(pidx);
        if (region_finder == planet_build_map->regions.end()) return;
        const std::size_t town_idx = region_finder->second.town_idx;
        settlement_t * town = nullptr;
        if (town_idx > 0) town = &planet.civs.settlements[town_idx];
        if (!town) return;
        //std::cout << "Improving " << town->name << "\n";

        // We can build things!
        bool done_building = false;
        for (const auto &b : caste_def.builds) {
            //std::cout << b.first << "\n";
            if (!done_building) {
                int n_present = 0;
                for (const auto &eb : town->improvements) {
                    //std::cout << eb << "\n";
                    if (eb == b.first) ++n_present;
                }
                if (n_present < b.second || b.second == 0) {
                    done_building = true;
                    town->improvements.push_back(b.first);
                    //std::cout << "Build: " << b.first << "\n";
                }
            }
        }
    }

    int town_improvement_defense(settlement_t *town) {
        if (!town) return 0;
        int bonus = 0;
        for (const auto &i : town->improvements) {
            if (i == "ant_mound") {
                bonus += 20;
            } else if (i == "hovel_wood") {
                bonus += 1;
            } else if (i == "wood-palisade") {
                bonus += 20;
            } else if (i == "wood-tower") {
                bonus += 20;
            } else if (i == "earthworks") {
                bonus += 20;
            }
        }
        return bonus;
    }

    int peep_strength(planet_t &planet, const std::size_t &id, const civ_t &civ) {
        int bonus = 0;

        const auto &peep = planet.civs.population[id];
        if (peep.deceased) return 0;

        bonus += peep.level * civ.tech_level;

        return bonus;
    }

    void civ_interactions(planet_t &planet, random_number_generator &rng, const int pidx,
                          spp::sparse_hash_set<std::size_t> &dead_peeps) {
        auto rf = planet_build_map->regions.find(pidx);
        if (rf == planet_build_map->regions.end()) return;
        const std::size_t town_idx = rf->second.town_idx;
        settlement_t * town = nullptr;
        if (town_idx > 0) town = &planet.civs.settlements[town_idx];

        // How do they feel about one another?
        for (auto it = rf->second.units_present.begin(); it != rf->second.units_present.end(); ++it) {
            auto &civ_a = planet.civs.civs[it->first];
            auto &species_a = civ_defs[civ_a.species_tag];

            for (auto it2 = rf->second.units_present.begin(); it2 != rf->second.units_present.end(); ++it2) {
                if (it->first != it2->first) {
                    auto &civ_b = planet.civs.civs[it2->first];
                    auto &species_b = civ_defs[civ_b.species_tag];
                    bool war = false;
                    auto relation_finder = civ_a.relations.find(it2->first);
                    if (civ_a.relations.find(it2->first) == civ_a.relations.end()) {
                        if (species_a.ethics.behavior == "eat_world" || species_b.ethics.behavior == "eat_world") {
                            civ_a.relations[it2->first] = -100;
                            civ_b.relations[it->first] = -100;
                            war = true;
                        } else {
                            const int feel = rng.roll_dice(1, 20) - 10;
                            civ_a.relations[it2->first] = feel;
                            civ_b.relations[it->first] = feel;
                            if (feel < 0) war = true;
                        }
                    } else {
                        if (relation_finder->second < 0) war = true;
                    }

                    if (war) {
                        int strength1 = 0;
                        int strength2 = 0;

                        if (town && town->civ_id == it->first) {
                            strength1 += 25 * civ_a.tech_level;
                            strength1 += town_improvement_defense(town);
                        }
                        if (town && town->civ_id == it2->first) {
                            strength2 += 25 * civ_b.tech_level;
                            strength2 += town_improvement_defense(town);
                        }

                        for (const auto &p : it->second) {
                            strength1 += peep_strength(planet, p, civ_a);
                        }
                        for (const auto &p : it2->second) {
                            strength2 += peep_strength(planet, p, civ_b);
                        }

                        if (strength1 > strength2) {
                            for (const auto &p : it2->second) {
                                dead_peeps.insert(p);
                            }
                        } else {
                            for (const auto &p : it->second) {
                                dead_peeps.insert(p);
                            }
                        }
                    }
                }
            }
        }
    }

    void apply_deaths(planet_t &planet, spp::sparse_hash_set<std::size_t> &dead_peeps) {
        for (const auto &id : dead_peeps) {
            //std::cout << "Peep #" << id << " is dead!\n";
            auto &peep = planet.civs.population[id];
            const std::size_t civ_id = peep.civ_id;
            peep.deceased = true;
            if (peep.married) planet.civs.population[peep.married_to].married = false;
            const int pidx = planet.idx(peep.world_x, peep.world_y);

            record_departure(pidx, peep, id);
        }
        dead_peeps.clear();
    }

    void planet_extinctions(planet_t &planet, random_number_generator &rng) {
        std::size_t i = 0;
        for (auto &civ : planet.civs.civs) {
            if (!civ.extinct) {
                int n_surviving = 0;
                for (const auto &peep : planet.civs.population) {
                    if (peep.civ_id == i) ++n_surviving;
                }
                if (n_surviving == 0) {
                    civ.extinct = true;
                }
            }
            ++i;
        }
    }

    void planet_evolution(planet_t &planet, random_number_generator &rng) {
        for (auto &civ : planet.civs.civs) {
            if (civ.research_level > civ.tech_level*100 && civ.tech_level < 10) {
                civ.research_level = 0;
                ++civ.tech_level;
                auto species = civ_defs.find(civ.species_tag)->second;
                if (!species.evolves_into.empty()) {
                    const int roll = rng.roll_dice(1, species.evolves_into.size())-1;
                    civ.species_tag = species.evolves_into[roll];
                }
            }
        }
    }

    void planet_settlement_cull(planet_t &planet) {
        for (auto &town : planet.civs.settlements) {
            const int pidx = planet.idx(town.world_x, town.world_y);
            auto finder = planet_build_map->regions.find(pidx);
            if (finder == planet_build_map->regions.end()) {
                // Not found - we don't need this settlement
                town.status = 0;
            } else {
                auto civ_finder = finder->second.units_present.find(town.civ_id);
                if (civ_finder == finder->second.units_present.end() || civ_finder->second.empty()) {
                    std::size_t max_pop = 0;
                    std::size_t civ_id = 0;
                    for (auto it=finder->second.units_present.begin(); it!=finder->second.units_present.end(); ++it) {
                        if (it->second.size() > max_pop) {
                            civ_id = it->first;
                            max_pop = it->second.size();
                        }
                    }
                    if (civ_id > 0) {
                        town.civ_id = civ_id;
                    } else {
                        town.status = 0;
                    }
                }
            }
        }
    }
}

using namespace history_builder;

void planet_build_initial_civs(planet_t &planet, rltk::random_number_generator &rng) {
    set_worldgen_status("Initializing starting settlements");
    planet_build_map = std::make_unique<build_map_t>(); // Initialize spatial helper

    for (std::size_t i=0; i<n_civs; ++i) {
        add_civ(planet, rng, i);
    }
}

void planet_build_run_year(const int &year, planet_t &planet, random_number_generator &rng) {
    spp::sparse_hash_set<std::size_t> dead_peeps;

    const std::size_t max_peep_id = planet.civs.population.size();
    for (std::size_t i=0; i<max_peep_id; ++i) {
        auto &peep = planet.civs.population[i];
        auto &species = civ_defs.find(peep.species_tag)->second;
        auto &caste = species.castes.find(peep.caste)->second;
        const int pidx = planet.idx(peep.world_x, peep.world_y);

        if (!peep.deceased) {
            bool can_move = true;

            // Age and natural death
            ++peep.age;
            if (peep.age > species.max_age) {
                peep.deceased = true;
                dead_peeps.insert(i);
            }

            // Marriages
            if (!peep.deceased && species.breed_type == "marriage"
                && !peep.married && peep.age > species.child_age) {
                evaluate_marriage(planet, pidx, peep, species, i);
            }

            // Births
            if (!peep.deceased && caste.fertile) {
                if (species.breed_type == "marriage" && peep.married && !peep.male) {
                    // Can spawn a child (and may die in childbirth)
                    give_birth(planet, rng, species, peep, dead_peeps, i);
                    can_move = false;
                } else if (species.breed_type == "hatch") {
                    // Can spawn lots of children
                    hatch_eggs(planet, rng, species, peep);
                    can_move = false;
                }
            }

            // Exploration
            if (!peep.deceased && can_move) {
                const bool moved = evaluate_movement(planet, rng, species, peep, caste, pidx, i);
                if (!moved && !caste.builds.empty()) {
                    // We can build something here!
                    build_improvements(planet, caste, pidx);
                }
            }

            //planet_display_update_zoomed(planet, peep.world_x, peep.world_y);
        }
    }

    // Apply deaths
    apply_deaths(planet, dead_peeps);

    // Interactions
    for (int y=0; y<WORLD_HEIGHT; ++y) {
        for (int x=0; x<WORLD_WIDTH; ++x) {
            const int pidx = planet.idx(x,y);
            auto region_finder = planet_build_map->regions.find(pidx);
            if (region_finder != planet_build_map->regions.end()) {
                if (region_finder->second.units_present.size() > 1) {
                    // There are multiple civs here
                    //std::cout << "Interactions - " << region_finder->second.units_present.size() << " present\n";
                    civ_interactions(planet, rng, pidx, dead_peeps);
                }
            }
        }
    }

    // Apply deaths again - post interactions
    apply_deaths(planet, dead_peeps);

    // Cull extinct civs
    planet_extinctions(planet, rng);

    // Promote researching civs
    planet_evolution(planet, rng);

    // Settlement cull
    planet_settlement_cull(planet);
}

void planet_build_initial_history(planet_t &planet, rltk::random_number_generator &rng) {
    constexpr int STARTING_YEAR = 2500;
    for (int year=STARTING_YEAR; year<2525; ++year) {
        set_worldgen_status(std::string("Running year ") + std::to_string(year));
        planet_display_update_zoomed(planet, WORLD_WIDTH/2, WORLD_HEIGHT/2);
        planet_build_run_year(year, planet, rng);
        planet_display_update_zoomed(planet, WORLD_WIDTH/2, WORLD_HEIGHT/2);
    }
    planet_build_map.reset();
}
