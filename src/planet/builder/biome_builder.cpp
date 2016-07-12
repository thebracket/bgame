#include "biome_builder.hpp"
#include "../planet_builder.hpp"


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
}
