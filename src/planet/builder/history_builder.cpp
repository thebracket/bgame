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

    const int n_civs = WORLD_WIDTH - rng.roll_dice(1,32) + rng.roll_dice(1,32);
    for (int i=0; i<n_civs; ++i) {
        civ_t civ;

        // Define the initial species
        civ.name = std::string("Civ #") + std::to_string(i);
        civ.species_tag = random_species(rng);
        std::cout << civ.name << " - " << civ.species_tag << "\n";

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
        std::cout << "Starting at " << starting_loc.first << "," << starting_loc.second << "\n";

        // Build the initial settlement
        settlement_t town;
        town.civ_id = i;
        town.name = std::string("Settlement #") + std::to_string(planet.civs.settlements.size());
        town.world_x = starting_loc.first;
        town.world_y = starting_loc.second;
        planet.civs.settlements.push_back(town);
        std::cout << "They have founded the town, " << town.name << "\n";

        // Generate an initial population of unimportant people
        const int n_peeps = rng.roll_dice(3,100);
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
        std::cout << "They started with a population of " << n_peeps << "\n\n";

        planet.civs.civs.push_back(civ);
        planet_display_update_zoomed(planet, starting_loc.first, starting_loc.second);
    }

    std::cout << "Seeded the world with " << n_civs << " civilizations and " << planet.civs.unimportant_people.size() << " folk.\n";
}

void planet_build_initial_history(planet_t &planet, rltk::random_number_generator &rng) {
    for (int year=0; year<2525; ++year) {
        set_worldgen_status("Running Year " + std::to_string(year));

        for (auto &civ : planet.civs.civs) {
            std::cout << "Year " << year << " for " << civ.name << "\n";

            // Natural deaths
            // Natural births
            // Marriages
            // Each unimportant person...
        }
    }
}