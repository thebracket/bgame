#include "biome_builder.hpp"
#include "../planet_builder.hpp"
#include "../../raws/raws.hpp"
#include <unordered_map>
#include <boost/optional.hpp>

boost::optional<std::unordered_map<uint8_t, double>> biome_membership(planet_t &planet, const std::size_t &idx) {
	std::unordered_map<uint8_t, double> percents;
	std::unordered_map<uint8_t, long> counts;
	long n_cells = 0L;
	long total_temperature = 0L;
	long total_rainfall = 0L;
	long total_height = 0L;
	long total_variance = 0L;
	long total_x = 0L;
	long total_y = 0L;

	for (int y=0; y<WORLD_HEIGHT; ++y) {
		for (int x=0; x<WORLD_WIDTH; ++x) {
			const int block_idx = planet.idx(x,y);

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
	if (n_cells == 0) return boost::optional<std::unordered_map<uint8_t, double>>();

	double counter = static_cast<double>(n_cells);
	planet.biomes[idx].mean_altitude = static_cast<uint8_t>((double)total_height / counter);
	planet.biomes[idx].mean_rainfall = static_cast<uint8_t>((double)total_rainfall / counter);
	planet.biomes[idx].mean_temperature = static_cast<uint8_t>((double)total_temperature / counter);
	planet.biomes[idx].mean_variance = static_cast<uint8_t>((double)total_variance / counter);
	planet.biomes[idx].center_x = total_x / n_cells;
	planet.biomes[idx].center_y = total_y / n_cells;

	const int distance_from_pole = std::min( rltk::distance2d(planet.biomes[idx].center_x,planet.biomes[idx].center_y, WORLD_WIDTH/2, 0), 
		rltk::distance2d(planet.biomes[idx].center_x, planet.biomes[idx].center_y, WORLD_WIDTH/2, WORLD_HEIGHT) );
	const int distance_from_center = rltk::distance2d(planet.biomes[idx].center_x,planet.biomes[idx].center_y, WORLD_WIDTH/2, WORLD_HEIGHT/2);

	if (distance_from_pole > 200) {
		planet.biomes[idx].warp_mutation = 0;
	} else {
		planet.biomes[idx].warp_mutation = (200 - distance_from_pole)/2;
	}
	planet.biomes[idx].savagery = std::min(100, distance_from_center);

	for (int i=0; i<=block_type::MAX_BLOCK_TYPE; ++i) {
		auto finder = counts.find(i);
		if (finder == counts.end()) {
			percents[i] = 0.0;
		} else {
			const double pct = (double)finder->second / counter;
			percents[i] = pct;
		}

	}

	return percents;
}

std::vector<std::pair<double,std::size_t>> find_possible_biomes(std::unordered_map<uint8_t, double> &percents, const biome_t &biome) {
	std::vector<std::pair<double,std::size_t>> result;

	std::size_t idx = 0;
	for (const biome_type_t &bt : biome_defs) {
		if (biome.mean_temperature >= bt.min_temp && biome.mean_temperature <= bt.max_temp 
			&& biome.mean_rainfall >= bt.min_rain && biome.mean_rainfall <= bt.max_rain
			&& biome.warp_mutation >= bt.min_mutation && biome.warp_mutation <= bt.max_mutation) {
			
			// It's possible, so check to see if tile types are available
			for (const uint8_t &occur : bt.occurs) {
				auto finder = percents.find(occur);
				if (finder != percents.end() && finder->second > 0) {
					result.push_back(std::make_pair( finder->second * 100.0, idx ));
				}
			}
		}
		++idx;
	}

	return result;
} 

void build_biomes(planet_t &planet, rltk::random_number_generator &rng) {
    set_worldgen_status("Growing Biomes");

    const int n_biomes = WORLD_HEIGHT * WORLD_WIDTH / (32 + rng.roll_dice(1,32));

	// Randomly place biome centers
	std::vector<std::pair<int,int>> centroids;
	for (int i=0; i<n_biomes; ++i) {
		centroids.push_back(std::make_pair(rng.roll_dice(1, WORLD_WIDTH), rng.roll_dice(1, WORLD_HEIGHT)));
		planet.biomes.push_back(biome_t{});
	}

    // Assign each cell based on proximity
    set_worldgen_status("Scanning " + std::to_string(n_biomes) + " Biomes");
    for (int y=0; y<WORLD_HEIGHT; ++y) {
		for (int x = 0; x<WORLD_WIDTH; ++x) {
			int distance = std::numeric_limits<int>::max();
			int closest_index = -1;

			for (int i=0; i<n_biomes; ++i) {
				const int biome_x = centroids[i].first;
				const int biome_y = centroids[i].second;
				const int dx = std::abs(biome_x - x);
				const int dy = std::abs(biome_y - y);
				const int biome_distance = (dx*dx) + (dy*dy);
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
	for (biome_t &biome : planet.biomes) {
		auto membership_count = biome_membership(planet, count);
		if (membership_count) {
			auto possible_types = find_possible_biomes(membership_count.get(), biome);
			if (!possible_types.empty()) {

				double max_roll = 0.0;
				for (const auto &possible : possible_types) {
					max_roll += possible.first;
				}
				int dice_roll = rng.roll_dice(1, (int)max_roll);
				for (const auto &possible : possible_types) {
					dice_roll -= possible.first;
					if (dice_roll < 0) {
						biome.type = possible.second;
						break;
					}
				}
				if (biome.type == -1) biome.type = possible_types[possible_types.size()-1].second;
				// Name that biome!
			} else {
				++ no_match;
			}
		}
		planet_display_update_zoomed(planet, biome.center_x, biome.center_y);

		// Update the status
		double pct = (double)count / planet.biomes.size() * 100.0; 
		std::stringstream ss;
		ss << "Biome design: " << pct << "%";
		set_worldgen_status(ss.str());
		++count;
	}
}
