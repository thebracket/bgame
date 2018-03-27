#include "region_builder.hpp"
#include "../../raws/biomes.hpp"
#include "../planet_builder.hpp"
#include "settler_builder.hpp"
#include "sentient_builder.hpp"
#include "regions/heightmap.hpp"
#include "regions/water_features.hpp"
#include "regions/strata.hpp"
#include "regions/decorators.hpp"
#include "regions/trees.hpp"
#include "regions/buildings.hpp"
#include "regions/blight_builder.hpp"
#include "../../bengine/filesystem.hpp"
#include "../region/region.hpp"
#include "regions/game_objects.hpp"

inline std::pair<biome_t, biome_type_t> get_biome_for_region(planet_t &planet, const std::pair<int,int> &region) {
    const int idx = planet.idx(region.first, region.second);
    const int biome_idx = planet.landblocks[idx].biome_idx;
    return std::make_pair( planet.biomes[biome_idx], *get_biome_def(planet.biomes[biome_idx].type) );
}

void build_region(planet_t &planet, std::pair<int,int> &target_region, bengine::random_number_generator &rng, FastNoise &noise, const bool &ascii_mode) {
    // Builder region
    region::new_region(target_region.first, target_region.second, planet.landblocks[planet.idx(target_region.first, target_region.second)].biome_idx);

    // Lookup the biome
    set_worldgen_status("Looking up biome");
    auto biome = get_biome_for_region(planet, target_region);
    //std::cout << biome.second.name << ", variance " << +planet.landblocks[planet.idx(target_region.first, target_region.second)].variance << "\n";

    // Build height map based on region noise
    set_worldgen_status("Establishing ground altitude");
    std::vector<uint8_t> heightmap = create_empty_heightmap();
    build_heightmap_from_noise(target_region, noise, heightmap, planet);

    // Wet places
    std::vector<uint8_t> pooled_water;
    pooled_water.resize(REGION_HEIGHT * REGION_WIDTH);
    std::fill(pooled_water.begin(), pooled_water.end(), 0);
    std::vector<std::pair<int, uint8_t>> water_spawners;

    // Sub-biome map
    auto subregions = create_subregions(planet, heightmap, biome, rng, pooled_water, water_spawners);

    // Hydrology
    just_add_water(planet, pooled_water, heightmap, biome, rng, noise, water_spawners);

    // Strata map
    set_worldgen_status("Dividing strata");
    auto strata = build_strata(heightmap, rng, biome, planet);

    // Lay down rock strata, soil, top tile coverage
    set_worldgen_status("Laying down layers");
    lay_strata(heightmap, biome, strata, rng, pooled_water, water_spawners);

    // Build ramps and beaches
    build_ramps(pooled_water);
    build_beaches();

    // Determine crash site
    set_worldgen_status("Crashing the ship");
    const int crash_x = REGION_WIDTH / 2;
	const int crash_y = REGION_HEIGHT / 2;
	const int crash_z = region::ground_z(crash_x, crash_y);
    
    // Add game components
    build_game_components(crash_x, crash_y, crash_z, ascii_mode);

	// Trees and blight
	set_worldgen_status("Planting trees");
	int blight_level = 0;
	const int pidx = planet.idx(region::region_x(), region::region_y());
	blight_level = planet.civs.region_info[pidx].blight_level;
	if (blight_level < 100) {
		build_trees(biome, rng);
	}
	else {
		just_add_blight(rng);
	}

    // Trail of debris
    build_debris_trail(crash_x, crash_y);
  
    // Build the ship super-structure
    build_escape_pod(crash_x, crash_y, crash_z);

    // Add settlers
    std::vector<std::tuple<int,int,int>> settler_spawn_points;
    settler_spawn_points.push_back(std::make_tuple(crash_x - 3, crash_y - 2, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x - 2, crash_y - 2, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x - 1, crash_y - 2, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x, crash_y - 2, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x + 1, crash_y - 2, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x - 3, crash_y, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x - 2, crash_y, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x - 1, crash_y, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x, crash_y, crash_z+1));
    settler_spawn_points.push_back(std::make_tuple(crash_x + 1, crash_y, crash_z+1));

    for (int i=0; i<planet.starting_settlers; ++i) {
		auto &[sx, sy, sz] = settler_spawn_points[i % settler_spawn_points.size()];
        create_settler(planet, sx, sy, sz, rng, i % 3);
    }

    // Add features
    std::vector<std::tuple<int,int,int>> spawn_points;
    int spawn_counter = 0;
    if (planet.civs.region_info[pidx].settlement_size > 0) {
        //std::cout << "There is a settlement of size " << planet.civs.region_info[pidx].settlement_size << " here.\n";
        for (const auto &i : planet.civs.region_info[pidx].improvements) {
            std::cout << "Build a: " << i << "\n";
            // Spawn it
            if (i == "ant_mound") build_ant_mound(rng, spawn_points);
        }
        //std::cout << "Free Garrison of " << planet.civs.civs[planet.civs.region_info[pidx].owner_civ].name << "\n";
        create_sentient_unit(planet, rng, planet.civs.region_info[pidx].owner_civ, "garrison", spawn_points,
                            spawn_counter, false, crash_x, crash_y, crash_z);
    }
    for (const auto &unit : planet.civs.units) {
        if (unit.world_y == region::region_y() && unit.world_x == region::region_x()) {
            //std::cout << "Spawn a unit: " << unit.unit_type << "\n";
            create_sentient_unit(planet, rng, planet.civs.region_info[pidx].owner_civ, unit.unit_type, spawn_points,
                                spawn_counter, false, crash_x, crash_y, crash_z);
        }
    }

    // Build connectivity graphs
    set_worldgen_status("Looking for the map");
    region::tile_recalc_all();

    // Save the region
    set_worldgen_status("Saving region to disk");
	region::save_current_region();
	std::unique_ptr<std::ofstream> lbfile = std::make_unique<std::ofstream>(save_filename(), std::ios::out | std::ios::binary);
	bengine::ecs_save(lbfile);
}
