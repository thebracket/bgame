#include "history_builder.hpp"
#include "../planet_builder.hpp"
#include "../../raws/species.hpp"
#include "../../raws/string_table.hpp"
#include "../../bengine/string_utils.hpp"
#include "../../raws/lua_bridge.hpp"
#include "../../raws/defs/civilization_t.hpp"
#include <map>
#include <set>
#include "../../utils/system_log.hpp"

constexpr int N_CIVS = WORLD_WIDTH;

static std::string get_random_species(bengine::random_number_generator &rng, const int tech_level=0) noexcept {
    std::vector<std::string> eligible;

    each_civilization_def([&eligible, &tech_level] (const std::string ctag, const civilization_t * it) {
        if (it->tech_level == tech_level) eligible.push_back(ctag);
    });

    //if (eligible.empty()) throw std::runtime_error("No suitable civs available!");
    return eligible[rng.roll_dice(1, static_cast<int>(eligible.size()))-1];
}

void planet_build_initial_civs(planet_t &planet, bengine::random_number_generator &rng) noexcept {
    set_worldgen_status("Initializing starting settlements");

    for (auto i=1; i<N_CIVS; ++i) {
        civ_t civ;

        start_over:
		const auto wx = rng.roll_dice(1, WORLD_WIDTH-1);
		const auto wy = rng.roll_dice(1, WORLD_HEIGHT-1);
		const auto pidx = planet.idx(wx, wy);
        if (planet.landblocks[pidx].type == block_type::WATER) goto start_over;
        if (planet.civs.region_info[pidx].owner_civ > 0) goto start_over;

        const std::string loc_name = planet.biomes[planet.landblocks[pidx].biome_idx].name;

        civ.species_tag = get_random_species(rng, 0);
        civ.tech_level = 0;
        civ.extinct = false;
        civ.r = rng.roll_dice(1,255);
        civ.g = rng.roll_dice(1,255);
        civ.b = rng.roll_dice(1,255);
        civ.startx = wx;
        civ.starty = wy;

        // Name generation
        using namespace string_tables;
        const auto civ_finder = get_civ_def(civ.species_tag);
        const auto civ_name_func = "civ_name_gen_" + civ_finder->name_generator;
        const auto civ_leader_func = "leader_name_gen_" + civ_finder->name_generator;
        civ.name = lua_str_func(civ_name_func, rng.roll_dice(1,1000)) + std::string(" of the ") + loc_name;
        if (str_contains(civ.name, "{LASTNAME}")) {
            civ.name = str_replace(civ.name, "{LASTNAME}", string_table(LAST_NAMES)->random_entry(rng));
        }
        civ.leader_name = lua_str_func(civ_leader_func, rng.roll_dice(1,1000));
        if (str_contains(civ.leader_name, "{LASTNAME}")) {
            civ.leader_name = str_replace(civ.leader_name, "{LASTNAME}", to_proper_noun_case(string_table(LAST_NAMES)->random_entry(rng)));
        }
        if (str_contains(civ.leader_name, "{FIRSTNAME_M}")) {
            civ.leader_name = str_replace(civ.leader_name, "{FIRSTNAME_M}", to_proper_noun_case(string_table(FIRST_NAMES_MALE)->random_entry(rng)));
        }
        if (str_contains(civ.leader_name, "{FIRSTNAME_F}")) {
            civ.leader_name = str_replace(civ.leader_name, "{FIRSTNAME_F}", to_proper_noun_case(string_table(FIRST_NAMES_FEMALE)->random_entry(rng)));
        }
        civ.origin = loc_name;
		glog(log_target::GAME, log_severity::info, "Welcome: {0}, lead by {1}", civ.name, civ.leader_name);
        civ.glyph = get_species_def(civ_finder->species_tag)->worldgen_glyph;

        // Appearance
        if (get_species_def(civ_finder->species_tag)->render_composite) {
            civ.skin_color = get_species_def(civ_finder->species_tag)->skin_colors[rng.roll_dice(1, static_cast<int>(get_species_def(civ_finder->species_tag)->skin_colors.size()))-1];
            civ.hair_color = get_species_def(civ_finder->species_tag)->hair_colors[rng.roll_dice(1, static_cast<int>(get_species_def(civ_finder->species_tag)->hair_colors.size()))-1];

            civ.hair_style = BALD;
            const int style_roll = rng.roll_dice(1, 4);
            switch (style_roll)
            {
                case 1:
                    civ.hair_style = SHORT_HAIR;
                    break;
                case 2:
                    civ.hair_style = LONG_HAIR;
                    break;
                case 3:
                    civ.hair_style = PIGTAILS;
                    break;
                case 4:
                    civ.hair_style = TRIANGLE;
                    break;
            }
        }

        planet.civs.civs.push_back(civ);

        // Place the civilization start
        planet.civs.region_info[pidx].owner_civ = i;
        planet.civs.region_info[pidx].blight_level = 0;
        planet.civs.region_info[pidx].settlement_size = 1;

        // Create an initial garrison type unit
        unit_t starter;
        starter.owner_civ = i;
        starter.unit_type = "garrison";
        starter.world_x = wx;
        starter.world_y = wy;
        planet.civs.units.push_back(starter);

        planet_display_update_zoomed(planet, WORLD_WIDTH/2, WORLD_HEIGHT/2);
    }
    //std::cout << "Created " << N_CIVS << " civs\n";
}

static std::string random_unit_type(const civilization_t &civ, bengine::random_number_generator &rng) noexcept {
    std::vector<std::string> available;
	for (auto &it : civ.units) {
        if (it.second.tag != "garrison") available.push_back(it.first);
    }

    const auto roll = rng.roll_dice(1, static_cast<int>(available.size()))-1;
    return available[roll];
}

static void planet_build_civ_year(const int year, planet_t &planet, bengine::random_number_generator &rng, civ_t &civ, const std::size_t &id) noexcept {
    //std::cout << year << ", " << civ.name << "\n";

    auto civ_f = get_civ_def(civ.species_tag);
    auto species_f = get_species_def(civ_f->species_tag);

    // Total build points, find settlements
	auto bp = 0;
    std::set<std::size_t> towns;
    std::size_t i=0;
	auto unit_count = 0;
    for (const auto &settlement : planet.civs.region_info) {
        if (settlement.owner_civ == id) {
            bp += settlement.settlement_size*10;
            towns.insert(i);
        }
        ++i;
    }
    for (auto &unit : planet.civs.units) {
        if (unit.owner_civ == id) {
            const auto unit_finder = civ_f->units.find(unit.unit_type);
            if (unit_finder == civ_f->units.end()) {
                //throw std::runtime_error(std::string("Unable to find: ") + civ.species_tag + std::string("/") + unit.unit_type);
            } else {
                if (unit_finder->second.bp_per_turn > 0) bp += unit_finder->second.bp_per_turn;
                ++unit_count;
            }
        }
    }
    //std::cout << "We have " << bp << " build points available\n";

    // If blight-spreader, then spread some blight
    if (species_f->spreads_blight) {
        for (auto &pidx : towns) {
            if (planet.civs.region_info[pidx].blight_level < 100 && bp > 10) {
                planet.civs.region_info[pidx].blight_level = 100;
                bp -= 10;
                //std::cout << "Spread some blight for 10 bp\n";
            }
        }
    } else {
        for (auto &pidx : towns) {
            if (planet.civs.region_info[pidx].blight_level > 0 && bp > 10) {
                planet.civs.region_info[pidx].blight_level = 0;
                bp -= 10;
                //std::cout << "Clear some blight for 10 bp\n";
            }
        }
    }

    // Build improvements
    if (bp > 9 && !civ_f->can_build.empty()) {
        for (auto &pidx : towns) {
            if (bp > 9) {
                for (auto &build : civ_f->can_build) {
					auto has_one = false;
                    for (const auto &i : planet.civs.region_info[pidx].improvements) {
                        if (i == build) has_one = true;
                    }
                    if (!has_one) {
                        bp -= 10;
                        planet.civs.region_info[pidx].improvements.push_back(build);
                        //std::cout << "Built a " << build << " for 10 bp.\n";
                    }
                }
            }

            // Consider bigger towns
            if (bp > civ.tech_level*20 && planet.civs.region_info[pidx].settlement_size < civ.tech_level) {
                bp -= civ.tech_level * 20;
                ++planet.civs.region_info[pidx].settlement_size;
                //std::cout << "Expanded settlement to size " << planet.civs.region_info[pidx].settlement_size << "\n";
            }
        }
    }

    // Tech-level improvement
    if (!civ_f->evolves_into.empty() && bp > civ.tech_level*15 && rng.roll_dice(1,25)==1) {
        // Evolve!
        bp = 0;
        ++civ.tech_level;
        const auto roll = rng.roll_dice(1, static_cast<int>(civ_f->evolves_into.size()))-1;
        civ.species_tag = civ_f->evolves_into[roll];
        const auto civ2_f = get_civ_def(civ.species_tag);
        const auto civ_name_func = "civ_name_gen_" + civ2_f->name_generator;
        //std::cout << civ.name << " evolves to tech level " << +civ.tech_level << " and takes the name: " << civ.name << "\n";
        civ.name = lua_str_func( civ_name_func, rng.roll_dice(1, 1000) ) + std::string(" of the ") + civ.origin;
        civ_f = civ2_f;
        species_f = get_species_def(civ_f->species_tag);
        civ.glyph = species_f->worldgen_glyph;
    }

    // Consider new units
    const auto unit_cap = static_cast<int>(towns.size()) + civ.tech_level + 1;
    //std::cout << "Unit count: " << unit_count << ", cap " << unit_cap << "\n";
    while (bp > 5 && unit_count < unit_cap) {
        unit_t unit;
        unit.owner_civ = id;
        //std::cout << "(Civ " << id << "), " << planet.civs.civs[id].species_tag << "\n";
        unit.unit_type = random_unit_type(*civ_f, rng);
        unit.world_x = civ.startx;
        unit.world_y = civ.starty;
        planet.civs.units.push_back(unit);
        //std::cout << "Built a new " << unit.unit_type << ", for 5 bp\n";
        bp -= 5;
        ++unit_count;
    }

    // Movement goes here
    for (auto &unit : planet.civs.units) {
        if (unit.owner_civ == id) {
            int moves = civ_f->units[unit.unit_type].speed;
            while (moves > 0) {
                std::set<std::size_t> candidates;
                if (unit.world_x > 1 && planet.landblocks[planet.idx(unit.world_x - 1, unit.world_y)].type != block_type::WATER) candidates.insert(planet.idx(unit.world_x - 1, unit.world_y));
                if (unit.world_x < WORLD_WIDTH-2 && planet.landblocks[planet.idx(unit.world_x + 1, unit.world_y)].type != block_type::WATER) candidates.insert(planet.idx(unit.world_x + 1, unit.world_y));
                if (unit.world_y > 1 && planet.landblocks[planet.idx(unit.world_x, unit.world_y-1)].type != block_type::WATER) candidates.insert(planet.idx(unit.world_x, unit.world_y - 1));
                if (unit.world_y < WORLD_HEIGHT-2 && planet.landblocks[planet.idx(unit.world_x, unit.world_y+1)].type != block_type::WATER) candidates.insert(planet.idx(unit.world_x, unit.world_y + 1));

                if (!candidates.empty()) {
                    const int roll = rng.roll_dice(1, static_cast<int>(candidates.size()))-1;
                    int i=0;
                    for (auto it = candidates.begin(); it!=candidates.end(); ++it) {
                        if (i == roll) {
                            const size_t pidx = *it;
                            unit.world_x = pidx % WORLD_WIDTH;
                            unit.world_y = pidx / WORLD_WIDTH;
                        }
                        ++i;
                    }
                }

                --moves;
            }
        }
    }
}

static void planet_build_run_year(const int year, planet_t &planet, bengine::random_number_generator &rng) noexcept {
    // All civs get a turn
    std::size_t i=0;
    for (auto &civ : planet.civs.civs) {
        if (!civ.extinct) {
            planet_build_civ_year(year, planet, rng, civ, i);
        }
        ++i;
    }

    // Unit combat - units in the same region but of different civs kill one another
    //std::cout << "Considering combat\n";

	auto killed = 0;
    for (auto y=0; y<WORLD_HEIGHT-1; ++y) {
        for (auto x=0; x<WORLD_WIDTH-1; ++x) {
            const auto pidx = planet.idx(x,y);
            std::map<std::size_t, std::vector<std::size_t>> occupants;

            std::size_t i=0;
            for (auto &unit : planet.civs.units) {
                if (unit.world_x == x && unit.world_y == y) {
                    occupants[unit.owner_civ].push_back(i);
                }
            }

            if (occupants.size()>1) {
                // Fight!
                std::map<std::size_t, int> strengths;
				for (auto &cit : occupants) {
					auto str = 0;
                    // Defense bonus
                    if (planet.civs.region_info[pidx].owner_civ == cit.first) {
                        str += 1; // Minimal bonus for home ground
                        for (auto &imp : planet.civs.region_info[pidx].improvements) {
                            if (imp == "ant_mound") str += 2;
                            if (imp == "ant_tunnel") str += 1;
                            if (imp == "earthworks") str += 3;
                            if (imp == "wood-pallisade") str += 5;
                        }
                    }

                    for (auto &uid : cit.second) {
						if (planet.civs.civs.size() < cit.first) {
							const std::string ut = planet.civs.units[uid].unit_type;
							const std::string st = planet.civs.civs[cit.first].species_tag;
							//std::cout << st << ":" << ut << "\n";
							auto civ_f = get_civ_def(st);
							if (civ_f != nullptr) {
								auto u_f = civ_f->units.find(ut);
								if (u_f != civ_f->units.end()) {
									str += u_f->second.worldgen_strength;
								}
							}
						}
                    }
                    strengths[cit.first] = str + rng.roll_dice(2, 6);
                }

				auto max = 0; std::size_t winner = 0;
				for (auto &it : strengths) {
                    if (it.second > max) {
                        max = it.second;
                        winner = it.first;
                    }
                }
                //std::cout << "Fight!\n";

				for (auto &it : occupants) {
                    if (it.first != winner) {
                        for (auto &uid : it.second) {
                            planet.civs.units[uid].dead = true;
                            ++killed;
                        }
                    }
                }

                planet.civs.region_info[pidx].owner_civ = winner;
            } else if (!occupants.empty()) {
                planet.civs.region_info[pidx].owner_civ = occupants.begin()->first;
                if (planet.civs.region_info[pidx].settlement_size == 0) planet.civs.region_info[pidx].settlement_size = 1;
            }
        }
    }
    //std::cout << "War has killed " << killed << " units this turn.\n";
    planet.civs.units.erase(
            std::remove_if(
                    planet.civs.units.begin(),
                    planet.civs.units.end(),
                    [] (unit_t &u) { return u.dead; }
            ), planet.civs.units.end()
    );

    // Units in an unclaimed region build there and claim it
    //std::cout << "Considering expansion\n";
    //std::vector<unit_t> new_units;
    for (const auto &unit : planet.civs.units) {
        const int pidx = planet.idx(unit.world_x, unit.world_y);
        if (planet.civs.region_info[pidx].owner_civ == 0) {
            planet.civs.region_info[pidx].owner_civ = unit.owner_civ;
            planet.civs.region_info[pidx].settlement_size = 1;
        }
    }
    /*for (const auto unit : new_units) {
        planet.civs.units.push_back(unit);
    }*/

    // Remove all extinct civilizations
    //std::cout << "Extinctions\n";
    std::size_t I=0;
    for (auto &civ : planet.civs.civs) {
        if (!civ.extinct) {
            bool found = false;
            for (const auto &u : planet.civs.units) {
                if (u.owner_civ == I) found = true;
            }
            if (!found) {
                //std::cout << "Removing extinct civ\n";
                civ.extinct = true;
                for (auto &t : planet.civs.region_info) {
                    if (t.owner_civ == I) t.owner_civ = 0;
                }
            }
        }
        ++I;
    }
}

void planet_build_initial_history(planet_t &planet, bengine::random_number_generator &rng) noexcept {
    constexpr int STARTING_YEAR = 2425;
    for (int year=STARTING_YEAR; year<2525; ++year) {
        set_worldgen_status(std::string("Running year ") + std::to_string(year));
        planet_display_update_zoomed(planet, WORLD_WIDTH/2, WORLD_HEIGHT/2);
        planet_build_run_year(year, planet, rng);
        planet_display_update_zoomed(planet, WORLD_WIDTH/2, WORLD_HEIGHT/2);
    }
}
