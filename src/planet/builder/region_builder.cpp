#include "region_builder.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/biomes.hpp"
#include "../constants.hpp"
#include "../planet_builder.hpp"
#include "settler_builder.hpp"
#include "sentient_builder.hpp"
#include "regions/starting_point.hpp"
#include "regions/heightmap.hpp"
#include "regions/water_features.hpp"
#include "regions/strata.hpp"
#include "regions/decorators.hpp"
#include "regions/trees.hpp"
#include "regions/buildings.hpp"
#include "../../raws/materials.hpp"
#include "../../components/sentient_ai.hpp"
#include "regions/blight_builder.hpp"

#include <rltk.hpp>

using namespace rltk;

inline std::pair<biome_t, biome_type_t> get_biome_for_region(planet_t &planet, const std::pair<int,int> &region) {
    const int idx = planet.idx(region.first, region.second);
    const int biome_idx = planet.landblocks[idx].biome_idx;
    return std::make_pair( planet.biomes[biome_idx], get_biome_def(planet.biomes[biome_idx].type) );
}

inline void zero_map(region_t &region) {
    region.next_tree_id = 1;
    std::fill(region.visible.begin(), region.visible.end(), false);
    std::fill(region.solid.begin(), region.solid.end(), false);
    std::fill(region.opaque.begin(), region.opaque.end(), false);
    std::fill(region.revealed.begin(), region.revealed.end(), false);
    std::fill(region.tile_type.begin(), region.tile_type.end(), tile_type::OPEN_SPACE);
    std::fill(region.tile_material.begin(), region.tile_material.end(), 0);
    std::fill(region.tile_hit_points.begin(), region.tile_hit_points.end(), 0);
    std::fill(region.building_id.begin(), region.building_id.end(), 0);
    std::fill(region.tree_id.begin(), region.tree_id.end(), 0);
    std::fill(region.tile_vegetation_type.begin(), region.tile_vegetation_type.end(), 0);
    std::fill(region.tile_vegetation_ticker.begin(), region.tile_vegetation_ticker.end(), 0);
    std::fill(region.tile_vegetation_lifecycle.begin(), region.tile_vegetation_lifecycle.end(), 0);
    std::fill(region.water_level.begin(), region.water_level.end(), 0);
    std::fill(region.blood_stains.begin(), region.blood_stains.end(), false);
    std::fill(region.stockpile_id.begin(), region.stockpile_id.end(), 0);
    std::fill(region.bridge_id.begin(), region.bridge_id.end(), 0);
}

void build_region(planet_t &planet, std::pair<int,int> &target_region, rltk::random_number_generator &rng, FastNoise &noise) {
    // Builder region
    region_t region;
	region.region_x = target_region.first;
	region.region_y = target_region.second;
    region.biome_idx = planet.landblocks[planet.idx(target_region.first, target_region.second)].biome_idx;
    
    // Lookup the biome
    set_worldgen_status("Looking up biome");
    auto biome = get_biome_for_region(planet, target_region);
    std::cout << biome.second.name << ", variance " << +planet.landblocks[planet.idx(target_region.first, target_region.second)].variance << "\n";

    // Build height map based on region noise
    set_worldgen_status("Establishing ground altitude");
    std::vector<uint8_t> heightmap = create_empty_heightmap();
    build_heightmap_from_noise(target_region, noise, heightmap, planet);
    const int water_level = 5;

    // Wet places
    std::vector<uint8_t> pooled_water;
    pooled_water.resize(REGION_HEIGHT * REGION_WIDTH);
    std::fill(pooled_water.begin(), pooled_water.end(), 0);
    std::vector<std::pair<int, uint8_t>> water_spawners;

    // Sub-biome map
    auto subregions = create_subregions(planet, region, heightmap, biome, rng, pooled_water, water_spawners);

    // Hydrology
    just_add_water(planet, region, pooled_water, heightmap, biome, rng, noise, water_spawners);

    // Strata map
    set_worldgen_status("Dividing strata");
    auto strata = build_strata(region, heightmap, rng, biome, planet);

    // Lay down rock strata, soil, top tile coverage
    set_worldgen_status("Laying down layers");
    zero_map(region);
    lay_strata(region, heightmap, biome, strata, rng, pooled_water, water_spawners);

    // Build ramps and beaches
    build_ramps(region);
    build_beaches(region);

    // Plant trees
    set_worldgen_status("Planting trees");     

    // Determine crash site
    set_worldgen_status("Crashing the ship");
    const int crash_x = REGION_WIDTH / 2;
	const int crash_y = REGION_HEIGHT / 2;
	const int crash_z = get_ground_z(region, crash_x, crash_y);
    
    // Add game components
    build_game_components(region, crash_x, crash_y, crash_z);

    // Trail of debris
    build_debris_trail(region, crash_x, crash_y);
  
    // Build the ship super-structure
    build_escape_pod(region, crash_x, crash_y, crash_z);

    // Add settlers
    create_settler(planet, crash_x - 3, crash_y - 2, crash_z+1, rng, 0);
	create_settler(planet, crash_x - 2, crash_y - 2, crash_z+1, rng, 0);
	create_settler(planet, crash_x - 1, crash_y - 2, crash_z+1, rng, 0);
	create_settler(planet, crash_x, crash_y - 2, crash_z+1, rng, 1);
	create_settler(planet, crash_x + 1, crash_y - 2, crash_z+1, rng, 1);
	create_settler(planet, crash_x - 3, crash_y, crash_z+1, rng, 1);
	create_settler(planet, crash_x - 2, crash_y, crash_z+1, rng, 1);
	create_settler(planet, crash_x - 1, crash_y, crash_z+1, rng, 2);
	create_settler(planet, crash_x, crash_y, crash_z+1, rng, 2);
	create_settler(planet, crash_x + 1, crash_y, crash_z+1, rng, 2);

    // Add features
    bool has_settlement = false;
    bool settlement_active = false;
    int max_size = 0;
    std::size_t civ_id;
    int blight_level = 0;
    for (auto &town : planet.civs.settlements) {
        if (town.world_x == region.region_x && town.world_y == region.region_y) {
            std::cout << "A settlement of type " << +town.status << " should be here.\n";
            std::cout << "It had a peak population of " << town.max_size << "\n";
            std::cout << "It belonged to " << planet.civs.civs[town.civ_id].name << " (" << planet.civs.civs[town.civ_id].species_tag << ")\n";

            has_settlement = true;
            if (town.status > 0) settlement_active = true;
            max_size += town.max_size;
            civ_id = town.civ_id;
            blight_level = std::max((int)town.blight_level, (int)blight_level);
        }
    }

    std::vector<std::tuple<int,int,int>> spawn_points;
    if (has_settlement) {
        const int n_buildings = max_size * 5;
        std::cout << "Spawning " << n_buildings << " buildings. \n";
        if (!settlement_active) std::cout << "The buildings are ruined.\n";
        build_buildings(region, rng, n_buildings, settlement_active, spawn_points, civ_id, planet);
    }

    // Add anyone who is still here from world-gen
    int count = 0;
    std::size_t peep_id = 0;
    for (auto &peep : planet.civs.population) {
        if (!peep.deceased && peep.world_x == region.region_x && peep.world_y == region.region_y) {
            std::cout << "Spawn a " << peep.species_tag << ", of the " << planet.civs.civs[peep.civ_id].name << "!\n";

            if (count < spawn_points.size()) {
                create_sentient(std::get<0>(spawn_points[count]), std::get<1>(spawn_points[count]), std::get<2>(spawn_points[count]),
                    rng, planet, region, peep_id);
            } else {
                const int x = rng.roll_dice(1,REGION_WIDTH-10)+5;
                const int y = rng.roll_dice(1,REGION_HEIGHT-10)+5;
                const int z = get_ground_z(region, x, y);
                create_sentient(x, y, z, rng, planet, region, peep_id);
            }
            ++count;
        }
        ++peep_id;
    }

    if (blight_level < 100) {
        build_trees(region, biome, rng);
    } else {
        just_add_blight(region, rng);
    }

    // Build connectivity graphs
    set_worldgen_status("Looking for the map");
    region.tile_recalc_all();

    // Save the region
    set_worldgen_status("Saving region to disk");
	save_region(region);
	const std::string save_filename = "world/savegame.dat";
	std::unique_ptr<std::ofstream> lbfile = std::make_unique<std::ofstream>(save_filename, std::ios::out | std::ios::binary);
	ecs_save(lbfile);
}
