#include "biome_builder.hpp"
#include "../planet_builder.hpp"
#include "../../raws/biomes.hpp"
#include "../../raws/defs/biome_type_t.hpp"
#include "../../bengine/geometry.hpp"
#include "../../utils/system_log.hpp"

static std::unordered_map<uint8_t, double> biome_membership(planet_t &planet, const std::size_t &idx) noexcept {
	std::unordered_map<uint8_t, double> percents;
	std::unordered_map<uint8_t, long> counts;
	auto n_cells = 0L;
	auto total_temperature = 0L;
	auto total_rainfall = 0L;
	auto total_height = 0L;
	auto total_variance = 0L;
	auto total_x = 0L;
	auto total_y = 0L;

	for (auto y=0; y<WORLD_HEIGHT; ++y) {
		for (auto x=0; x<WORLD_WIDTH; ++x) {
			const auto block_idx = planet.idx(x,y);

			if (planet.landblocks[block_idx].biome_idx == idx) {
				// Increment total counters
				++n_cells;
				total_temperature += planet.landblocks[block_idx].temperature_c;
				total_rainfall += planet.landblocks[block_idx].rainfall;
				total_height += planet.landblocks[block_idx].height;
				total_variance += planet.landblocks[block_idx].variance;
				total_x += x;
				total_y += y;

				// Increment count by cell type
				auto finder = counts.find(planet.landblocks[block_idx].type);
				if (finder == counts.end()) {
					counts[planet.landblocks[block_idx].type] = 1L;
				} else {
					++finder->second;
				}
			}
		}
	}

	// Calculate the averages
	if (n_cells == 0) {
		//std::unordered_map<uint8_t, double>();
		n_cells = 1;
	}

	const auto counter = static_cast<double>(n_cells);
	planet.biomes[idx].mean_altitude = static_cast<uint8_t>(static_cast<double>(total_height) / counter);
	planet.biomes[idx].mean_rainfall = static_cast<uint8_t>(static_cast<double>(total_rainfall) / counter);
	planet.biomes[idx].mean_temperature = static_cast<uint8_t>(static_cast<double>(total_temperature) / counter);
	planet.biomes[idx].mean_variance = static_cast<uint8_t>(static_cast<double>(total_variance) / counter);
	planet.biomes[idx].center_x = total_x / n_cells;
	planet.biomes[idx].center_y = total_y / n_cells;

	const auto distance_from_pole = static_cast<int>(std::min( bengine::distance2d(planet.biomes[idx].center_x,planet.biomes[idx].center_y, WORLD_WIDTH/2, 0),
															  bengine::distance2d(planet.biomes[idx].center_x, planet.biomes[idx].center_y, WORLD_WIDTH/2, WORLD_HEIGHT) ));
	const auto distance_from_center = static_cast<int>(bengine::distance2d(planet.biomes[idx].center_x,planet.biomes[idx].center_y, WORLD_WIDTH/2, WORLD_HEIGHT/2));

	if (distance_from_pole > 200) {
		planet.biomes[idx].warp_mutation = 0;
	} else {
		planet.biomes[idx].warp_mutation = (200 - distance_from_pole)/2;
	}
	planet.biomes[idx].savagery = std::min(100, distance_from_center);

	for (auto i=0; i<=block_type::MAX_BLOCK_TYPE; ++i) {
		const auto finder = counts.find(i);
		if (finder == counts.end()) {
			percents[i] = 0.0;
		} else {
			const auto pct = static_cast<double>(finder->second) / counter;
			percents[i] = pct;
		}

	}

	return percents;
}

static std::vector<std::pair<double,std::size_t>> find_possible_biomes(std::unordered_map<uint8_t, double> &percents, const biome_t &biome)  noexcept 
{
	std::vector<std::pair<double,std::size_t>> result;

	std::size_t idx = 0;
	each_biome([&biome, &idx, &result, &percents] (biome_type_t *bt) {
        if (biome.mean_temperature >= bt->min_temp && biome.mean_temperature <= bt->max_temp
            && biome.mean_rainfall >= bt->min_rain && biome.mean_rainfall <= bt->max_rain
            && biome.warp_mutation >= bt->min_mutation && biome.warp_mutation <= bt->max_mutation) {

            // It's possible, so check to see if tile types are available
            for (const uint8_t &occur : bt->occurs) {
                auto finder = percents.find(occur);
                if (finder != percents.end() && finder->second > 0) {
                    result.emplace_back(std::make_pair( finder->second * 100.0, idx ));
                }
            }
        }
        ++idx;
    });

	return result;
} 

static std::string name_biome(planet_t &planet, bengine::random_number_generator &rng, biome_t &biome) noexcept {
	std::string name = "Nameless";

	std::vector<std::string> adjectives;

	// Location-based adjective
	if (std::abs(biome.center_x - WORLD_WIDTH/2) < WORLD_WIDTH/10 && std::abs(biome.center_y - WORLD_HEIGHT/2) < WORLD_HEIGHT/10) {
		adjectives.emplace_back("Central");
	} else {
		if (biome.center_x < WORLD_WIDTH / 2) adjectives.emplace_back("Western");
		if (biome.center_x > WORLD_WIDTH / 2) adjectives.emplace_back("Eastern");
		if (biome.center_y < WORLD_HEIGHT / 2) adjectives.emplace_back("Northern");
		if (biome.center_y > WORLD_WIDTH / 2) adjectives.emplace_back("Southern");
	}

	// Water-based adjectives
	if (biome.mean_rainfall < 10) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Dry");
				break;
			case 2 :
				adjectives.emplace_back("Arid");
				break;
			case 3 :
				adjectives.emplace_back("Parched");
				break;
			case 4 :
				adjectives.emplace_back("Cracked");
				break;
		}
	} else if (biome.mean_rainfall < 30) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Dusty");
				break;
			case 2 :
				adjectives.emplace_back("Withered");
				break;
			case 3 :
				adjectives.emplace_back("Droughty");
				break;
			case 4 :
				adjectives.emplace_back("Dehydrated");
				break;
		}
	} else if (biome.mean_rainfall < 50) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Pleasant");
				break;
			case 2 :
				adjectives.emplace_back("Kind");
				break;
			case 3 :
				adjectives.emplace_back("Gentle");
				break;
			case 4 :
				adjectives.emplace_back("Timid");
				break;
		}
	} else if (biome.mean_rainfall < 70) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Damp");
				break;
			case 2 :
				adjectives.emplace_back("Dank");
				break;
			case 3 :
				adjectives.emplace_back("Moist");
				break;
			case 4 :
				adjectives.emplace_back("Fresh");
				break;
		}
	} else {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Wet");
				break;
			case 2 :
				adjectives.emplace_back("Soggy");
				break;
			case 3 :
				adjectives.emplace_back("Soaked");
				break;
			case 4 :
				adjectives.emplace_back("Drenched");
				break;
		}
	}

	// Temperature based adjectives
	if (biome.mean_temperature < 10) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Frozen");
				break;
			case 2 :
				adjectives.emplace_back("Cold");
				break;
			case 3 :
				adjectives.emplace_back("Icy");
				break;
			case 4 :
				adjectives.emplace_back("Biting");
				break;
		}
	} else if (biome.mean_temperature < 20) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Chilly");
				break;
			case 2 :
				adjectives.emplace_back("Frigid");
				break;
			case 3 :
				adjectives.emplace_back("Chilling");
				break;
			case 4 :
				adjectives.emplace_back("Shivering");
				break;
		}
	} else if (biome.mean_temperature < 30) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Pleasant");
				break;
			case 2 :
				adjectives.emplace_back("Nice");
				break;
			case 3 :
				adjectives.emplace_back("Temperate");
				break;
			case 4 :
				adjectives.emplace_back("Comfortable");
				break;
		}
	} else if (biome.mean_temperature < 40) {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Warm");
				break;
			case 2 :
				adjectives.emplace_back("Toasty");
				break;
			case 3 :
				adjectives.emplace_back("Cozy");
				break;
			case 4 :
				adjectives.emplace_back("Snug");
				break;
		}
	} else  {
		switch (rng.roll_dice(1,4)) {
			case 1 :
				adjectives.emplace_back("Hot");
				break;
			case 2 :
				adjectives.emplace_back("Scorching");
				break;
			case 3 :
				adjectives.emplace_back("Burning");
				break;
			case 4 :
				adjectives.emplace_back("Fuming");
				break;
		}
	}

	std::string noun;
	const auto bt = get_biome_def(biome.type);
	if (bt->nouns.empty()) {
		glog(log_target::GAME, log_severity::warning, "No nouns defined for {0}", bt->name);
	} else {
		noun = bt->nouns[rng.roll_dice(1, bt->nouns.size())-1];
	}

	name = noun;
	if (!adjectives.empty()) {
		const auto adj1 = rng.roll_dice(1, adjectives.size())-1;
		name = adjectives[adj1] + " " + noun;
		if (adjectives.size() > 1 && rng.roll_dice(1,6)>2) {
			auto adj2 = rng.roll_dice(1, adjectives.size())-1;
			while (adj1 == adj2) {
				adj2 = rng.roll_dice(1, adjectives.size())-1;
			}
			name = adjectives[adj2] + " " + name;
		}
	}

	//std::cout << "Biome: " << name << "\n";
	return name;
}

void build_biomes(planet_t &planet, bengine::random_number_generator &rng) noexcept {
    set_worldgen_status("Growing Biomes");

    const auto n_biomes = WORLD_HEIGHT * WORLD_WIDTH / (32 + rng.roll_dice(1,32));

	// Randomly place biome centers
	std::vector<std::pair<int,int>> centroids;
	for (auto i=0; i<n_biomes; ++i) {
		centroids.emplace_back(std::make_pair(rng.roll_dice(1, WORLD_WIDTH), rng.roll_dice(1, WORLD_HEIGHT)));
		planet.biomes.emplace_back(biome_t{});
	}

    // Assign each cell based on proximity
    set_worldgen_status("Scanning " + std::to_string(n_biomes) + " Biomes");
    for (auto y=0; y<WORLD_HEIGHT; ++y) {
		for (auto x = 0; x<WORLD_WIDTH; ++x) {
			auto distance = std::numeric_limits<int>::max();
			auto closest_index = -1;

			for (auto i=0; i<n_biomes; ++i) {
				const auto biome_x = centroids[i].first;
				const auto biome_y = centroids[i].second;
				const auto dx = std::abs(biome_x - x);
				const auto dy = std::abs(biome_y - y);
				const auto biome_distance = (dx*dx) + (dy*dy);
				if (biome_distance < distance) {
					distance = biome_distance;
					closest_index = i;
				}
			}

			planet.landblocks[planet.idx(x,y)].biome_idx = closest_index;
		}
		planet_display_update_altitude(planet);
	}

    // Process each biome and determine important facts defining it
	set_worldgen_status("Hand-crafting fyords");
	std::size_t count = 0;
	std::size_t no_match = 0;
	for (auto &biome : planet.biomes) {
		auto membership_count = biome_membership(planet, count);
		if (!membership_count.empty()) {
			auto possible_types = find_possible_biomes(membership_count, biome);
			if (!possible_types.empty()) {

				auto max_roll = 0.0;
				for (const auto &possible : possible_types) {
					max_roll += possible.first;
				}
				auto dice_roll = rng.roll_dice(1, static_cast<int>(max_roll));
				for (const auto &possible : possible_types) {
					dice_roll -= static_cast<int>(possible.first);
					if (dice_roll < 0) {
						biome.type = possible.second;
						break;
					}
				}
				if (biome.type == -1) biome.type = possible_types[possible_types.size()-1].second;
				biome.name = name_biome(planet, rng, biome);
			} else {
				++ no_match;
			}
		}
		planet_display_update_zoomed(planet, biome.center_x, biome.center_y);

		// Update the status
		const auto pct = static_cast<double>(count) / planet.biomes.size() * 100.0;
		fmt::MemoryWriter ss;
		ss << "Biome design: " << pct << "%";
		set_worldgen_status(ss.str());
		++count;
	}
}
