#include "history_builder.hpp"
#include "../planet_builder.hpp"
#include "../../raws/raws.hpp"
#include "../constants.hpp"
#include <iostream>
#include <sstream>

const std::string random_species(rltk::random_number_generator &rng) {
    const std::size_t n_species = species_defs.size();
    int roll = rng.roll_dice(1, n_species)-1;
    auto it = species_defs.begin();
    for (int i=0; i<roll; ++i) ++it;
    return it->first;
}

void planet_build_initial_civs(planet_t &planet, rltk::random_number_generator &rng) {
    set_worldgen_status("Initializing starting settlements");

    const int n_civs = WORLD_WIDTH;
    for (int i=0; i<n_civs; ++i) {
        civ_t civ;

        // Define the initial species
        civ.name = std::string("Civ #") + std::to_string(i);
        civ.species_tag = random_species(rng);
        civ.r = rng.roll_dice(1,255);
        civ.g = rng.roll_dice(1,255);
        civ.b = rng.roll_dice(1,255);
        //std::cout << civ.name << " - " << civ.species_tag << "\n";

        // Find a starting location
        std::pair<int,int> starting_loc;
        bool found = false;
        while (!found) {
            starting_loc = { rng.roll_dice(1, WORLD_WIDTH)-1, rng.roll_dice(1, WORLD_HEIGHT)-1 };
            if (planet.landblocks[planet.idx(starting_loc.first, starting_loc.second)].type != block_type::WATER)
            {
                bool dupe = false;
                for (const auto &s : planet.civs.settlements) {
                    if (s.world_x == starting_loc.first && s.world_y == starting_loc.second) dupe = true;
                }
                if (!dupe) found = true;
            }
        }
        //std::cout << "Starting at " << starting_loc.first << "," << starting_loc.second << "\n";

        // Build the initial settlement
        settlement_t town;
        town.civ_id = i;
        town.name = std::string("Settlement #") + std::to_string(planet.civs.settlements.size());
        town.world_x = starting_loc.first;
        town.world_y = starting_loc.second;
        planet.civs.settlements.push_back(town);
        //std::cout << "They have founded the town, " << town.name << "\n";

        // Generate an initial population of unimportant people
        const int n_peeps = rng.roll_dice(6,20);
        for (int j=0; j<n_peeps; ++j) {
            unimportant_person_t peep;
            peep.civ_id = i;
            peep.species_tag = civ.species_tag;
            peep.world_x = starting_loc.first;
            peep.world_y = starting_loc.second;
            if (rng.roll_dice(1,6)<=3) {
                peep.male = true;
            } else {
                peep.male = false;
            }
            peep.married = false;
            peep.married_to = 0;            
            peep.deceased = false;
            peep.age = 12 + rng.roll_dice(1,30);

            planet.civs.unimportant_people.push_back(peep);
        }
        //std::cout << "They started with a population of " << n_peeps << "\n\n";

        planet.civs.civs.push_back(civ);
        planet_display_update_zoomed(planet, starting_loc.first, starting_loc.second);
    }

    std::cout << "Seeded the world with " << n_civs << " civilizations and " << planet.civs.unimportant_people.size() << " folk.\n";
}

void planet_build_run_year(const int &year, planet_t &planet, rltk::random_number_generator &rng) {
    set_worldgen_status("Running Year " + std::to_string(year));

    std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> newborns;
    std::unordered_map<int, std::vector<int>> civlocs;
    std::unordered_map<int, int> civpops;
    std::vector<std::size_t> peep_killer;

    // Technological advancement
    for (auto &civ : planet.civs.civs) {
        if (!civ.extinct) {
            if (civ.tech_level < 10) {
                const int tech_roll = rng.roll_dice(1,100);
                if (tech_roll > 97) {
                    ++civ.tech_level;
                } else if (civ.tech_level > 1 && tech_roll == 1) {
                    -- civ.tech_level;
                }
            }
        }
    }

    std::size_t peep_id = 0;
    for (auto &peep : planet.civs.unimportant_people) {
        if (!peep.deceased) {
            // Lookups
            auto species = species_defs.find(peep.species_tag);

            // Age and natural death
            ++peep.age;
            if (peep.age > species->second.max_age) peep_killer.push_back(peep_id);

            // Births
            if (!peep.male && peep.age > species->second.child_age) {
                int birth_chance = 12 - planet.civs.civs[peep.civ_id].tech_level;
                if (peep.married) birth_chance += 10;
                if (rng.roll_dice(1,100) < birth_chance) {
                    newborns.push_back({peep.civ_id, peep_id, peep.married_to});
                }
            }

            // Random movement
            if (peep.age > species->second.child_age && rng.roll_dice(1,10)>5) {
                int direction = rng.roll_dice(1,4);
                switch (direction) {
                    case 1 : if (peep.world_y>2 && planet.landblocks[planet.idx(peep.world_x, peep.world_y-1)].type != block_type::WATER) --peep.world_y; break;
                    case 2 : if (peep.world_y<WORLD_HEIGHT-2 && planet.landblocks[planet.idx(peep.world_x, peep.world_y+1)].type != block_type::WATER) ++peep.world_y; break;
                    case 3 : if (peep.world_x>2 && planet.landblocks[planet.idx(peep.world_x-1, peep.world_y)].type != block_type::WATER) --peep.world_x; break;
                    case 4 : if (peep.world_x<WORLD_WIDTH-2 && planet.landblocks[planet.idx(peep.world_x+1, peep.world_y)].type != block_type::WATER) ++peep.world_x; break;
                }
            }

            // Marriages
            if (peep.age > species->second.child_age && peep.married == false && rng.roll_dice(1,10)>7) {
                int peep2_id = 0;
                for (auto &peep2 : planet.civs.unimportant_people) {
                    if (!peep2.deceased && peep.male == !peep2.male && !peep2.married && peep.world_x == peep2.world_x && peep.world_y == peep2.world_y) {
                        peep.married = true;
                        peep2.married = true;
                        peep2.married_to = peep_id;
                        peep.married_to = peep2_id;
                    }
                    ++peep2_id;
                }
            }

            // Registration - civ pop
            auto civpopfinder = civpops.find(peep.civ_id);
            if (civpopfinder == civpops.end()) {
                civpops[peep.civ_id] = 1;
            } else {
                ++civpopfinder->second;
            }

            // Registration - location
            const int idx = planet.idx(peep.world_x, peep.world_y);
            auto civlocfinder = civlocs.find(idx);
            if (civlocfinder == civlocs.end()) {
                civlocs[idx] = std::vector<int>(planet.civs.civs.size());
                civlocfinder = civlocs.find(idx);
            }
            ++civlocfinder->second[peep.civ_id];
        }

        ++peep_id;
    }

    // Second pass - put children with a parent
    peep_id = 0;
    for (auto &peep : planet.civs.unimportant_people) {
        if (!peep.deceased && !peep.male && peep.deceased) {
            if (!planet.civs.unimportant_people[peep.married_to].deceased) {
                peep.world_x = planet.civs.unimportant_people[peep.married_to].world_x;
                peep.world_y = planet.civs.unimportant_people[peep.married_to].world_y;
            }
        } else if (!peep.deceased) {
            auto species = species_defs.find(peep.species_tag);
            if (peep.age < species->second.child_age) {
                if (!planet.civs.unimportant_people[peep.mother_id].deceased) {
                    peep.world_x = planet.civs.unimportant_people[peep.mother_id].world_x;
                    peep.world_y = planet.civs.unimportant_people[peep.mother_id].world_y;
                } else if (!planet.civs.unimportant_people[peep.father_id].deceased) {
                    peep.world_x = planet.civs.unimportant_people[peep.father_id].world_x;
                    peep.world_y = planet.civs.unimportant_people[peep.father_id].world_y;
                } else {
                    peep_killer.push_back(peep_id);
                }
            }
        }
        ++peep_id;
    }

    // Check for extinct civilizations
    for (std::size_t i=0; i<planet.civs.civs.size(); ++i) {
        if (!planet.civs.civs[i].extinct) {
            if (civpops.find(i) == civpops.end()) {
                planet.civs.civs[i].extinct = true;
                std::cout << planet.civs.civs[i].name << " is now extinct.\n";
            }
        }
    }

    // Check for new and empty settlements
    for (auto it=civlocs.begin(); it!=civlocs.end(); ++it) {
        bool settlement_here = false;
        std::vector<std::size_t> deletable_settlements;
        std::size_t town_id = 0;
        for (const auto &town : planet.civs.settlements) {
            if (planet.idx(town.world_x, town.world_y) == it->first) {
                if (town.status != 0) {
                    settlement_here = true;
                } else {
                    deletable_settlements.push_back(town_id);
                }
            }
            ++town_id;
        }

        if (!settlement_here) {
            if (deletable_settlements.size() > 0) {
                for (const std::size_t &id : deletable_settlements) {
                    planet.civs.settlements[id].deleted = true;
                }
            }

            int civ_id = -1;
            int count=0;
            for (const int &n : it->second) {
                if (n > 0) civ_id = count;
                ++count;
            }

            int x = it->first % WORLD_WIDTH;
            int y = it->first / WORLD_WIDTH;
            settlement_t town;
            town.civ_id = civ_id;
            town.name = std::string("Settlement #") + std::to_string(planet.civs.settlements.size());
            town.world_x = x;
            town.world_y = y;
            town.status = 2;
            planet.civs.settlements.push_back(town);
        }
    }

    // Empty settlements
    for (auto &town : planet.civs.settlements) {
        const int map_index = planet.idx(town.world_x, town.world_y);
        auto finder = civlocs.find(map_index);
        if (finder == civlocs.end()) {
            town.status = 0; // Nobody here!
        } else {
            const int pop = finder->second[town.civ_id];
            if (pop < 1) town.status = 0; // Nobody here - abandoned
        }
    }

    // Check for interactions
    for (auto it=civlocs.begin(); it!=civlocs.end(); ++it) {
        int n_civs = 0;
        std::vector<int> interacting_civs;
        int civ_id = 0;
        for (int &popcount : it->second) {
            if (popcount > 0) {
                ++n_civs;
                interacting_civs.push_back(civ_id);
            }
            ++civ_id;
        }

        if (n_civs > 1) {
            int x = it->first % WORLD_WIDTH;
            int y = it->first / WORLD_WIDTH;

            //std::cout << "Civ interaction detected (at " << x << "/" << y << "), " << n_civs << " involved!\n";
            for (int &civ_id : interacting_civs) {

                for (int &other_civ : interacting_civs) {
                    if (civ_id != other_civ) {
                        //std::cout << " -- " << planet.civs.civs[civ_id].name << " and " << planet.civs.civs[other_civ].name << "\n";

                        int friendliness = -1;

                        auto relation_finder = planet.civs.civs[civ_id].relations.find(other_civ);
                        if (relation_finder == planet.civs.civs[civ_id].relations.end()) {
                            auto relation_finder2 = planet.civs.civs[other_civ].relations.find(civ_id);
                            if (relation_finder2 == planet.civs.civs[other_civ].relations.end()) {
                                // No known relations! Determine
                                //std::cout << " -- First contact!\n";
                                friendliness = rng.roll_dice(1,10)-5; // TODO: Modify by ethics
                                planet.civs.civs[civ_id].relations[other_civ] = friendliness;
                                planet.civs.civs[other_civ].relations[civ_id] = friendliness;
                            } else {
                                --relation_finder2->second;
                                friendliness = relation_finder2->second;
                            }
                        } else {
                            --relation_finder->second;
                            friendliness = relation_finder->second;
                        }

                        //std::cout << " -- Friendliness: " << friendliness << "\n";

                        if (friendliness < 0) {
                            //std::cout << " -- FIGHT! ";
                            int our_strength = (it->second[civ_id] * planet.civs.civs[civ_id].tech_level) + rng.roll_dice(1,10);
                            int their_strength = (it->second[other_civ] * planet.civs.civs[civ_id].tech_level) + rng.roll_dice(1,10);
                            //std::cout << our_strength << " - " << their_strength << "\n";

                            if (our_strength > their_strength) {
                                // Kill all of them!
                                peep_id = 0;
                                for (auto &peep : planet.civs.unimportant_people) {
                                    if (!peep.deceased && peep.civ_id == other_civ && peep.world_x == x && peep.world_y == y) {
                                        if (rng.roll_dice(1,6)>3) {
                                           peep_killer.push_back(peep_id);
                                        } else {
                                            peep.civ_id = civ_id;
                                        }
                                    } else if (!peep.deceased && peep.civ_id == civ_id && peep.world_x == x && peep.world_y == y && rng.roll_dice(1,6)>3) {
                                        peep_killer.push_back(peep_id);
                                    }
                                    ++peep_id;
                                }
                                //std::cout << " -- Exterminations: " << exterminated << "\n";
                            } else {
                                // Kill all of us!
                                peep_id = 0;
                                for (auto &peep : planet.civs.unimportant_people) {
                                    if (!peep.deceased && peep.civ_id == civ_id && peep.world_x == x && peep.world_y == y) {
                                        if (rng.roll_dice(1,6)>3) {
                                            peep_killer.push_back(peep_id);
                                        } else {
                                            peep.civ_id = other_civ;
                                        }
                                    } else if (!peep.deceased && peep.civ_id == other_civ && peep.world_x == x && peep.world_y == y && rng.roll_dice(1,6)>3) {
                                        peep_killer.push_back(peep_id);
                                    }
                                }
                                ++peep_id;
                                //std::cout << " -- Exterminations: " << exterminated << "\n";
                            }
                        }
                    }
                }
            }
        }
    }

    // Perform births
    for (auto &birth : newborns) {
        if (!planet.civs.civs[std::get<0>(birth)].extinct) {
            unimportant_person_t newbie;
            newbie.civ_id = std::get<0>(birth);
            newbie.species_tag = planet.civs.civs[newbie.civ_id].species_tag;
            newbie.married = false;
            newbie.married_to = 0;
            newbie.deceased = false;
            newbie.age = 0;
            newbie.mother_id = std::get<1>(birth);
            newbie.father_id = std::get<2>(birth);
            newbie.world_x = planet.civs.unimportant_people[std::get<1>(birth)].world_x;
            newbie.world_y = planet.civs.unimportant_people[std::get<1>(birth)].world_y;
            planet.civs.unimportant_people.push_back(newbie);
        }
    }

    // Perform deaths
    for (auto &id : peep_killer) {
        planet.civs.unimportant_people[id].deceased = true;
        if (planet.civs.unimportant_people[id].married) {
            int married_to = planet.civs.unimportant_people[id].married_to;
            if (!planet.civs.unimportant_people[married_to].deceased) planet.civs.unimportant_people[married_to].married = false;
        }
    }

    civ_cull_settlements(planet.civs.settlements);
    planet_display_update_zoomed(planet, WORLD_WIDTH/2, WORLD_HEIGHT/2);
    std::cout << "Year summary: " << newborns.size() << " births, " << peep_killer.size() << " deaths.\n";
}

void planet_build_initial_history(planet_t &planet, rltk::random_number_generator &rng) {
    for (int year=2000; year<2525; ++year) {        
        planet_build_run_year(year, planet, rng);
    }
}