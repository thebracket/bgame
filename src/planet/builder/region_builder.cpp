#include "region_builder.hpp"
#include "../../raws/raws.hpp"
#include "../constants.hpp"
#include "../planet_builder.hpp"
#include "noise_helper.hpp"
#include "../../components/components.hpp"

using namespace rltk;

inline int get_ground_z(region_t &region, const int x, const int y) {
	int z = REGION_DEPTH-1;
	bool hit_ground = false;
	while (!hit_ground) {
		const int idx = mapidx(x, y, z);
		if (region.tile_type[idx] == tile_type::SOLID) {
			hit_ground = true;
			++z;
		} else {
			--z;
		}
	}
	return z;
}

std::pair<int,int> builder_select_starting_region(planet_t &planet) {
    std::pair<int,int> coords = std::make_pair(WORLD_WIDTH/2, WORLD_HEIGHT/2);

    while (planet.landblocks[planet.idx(coords.first, coords.second)].type != block_type::WATER) {
        --coords.first;
    }

    return coords;
}

inline std::pair<biome_t, biome_type_t> get_biome_for_region(planet_t &planet, const std::pair<int,int> &region) {
    const int idx = planet.idx(region.first, region.second);
    const int biome_idx = planet.landblocks[idx].biome_idx;
    return std::make_pair( planet.biomes[biome_idx], biome_defs[planet.biomes[biome_idx].type] );
}

inline std::vector<uint8_t> create_empty_heightmap() {
    std::vector<uint8_t> heightmap;
    heightmap.resize(REGION_HEIGHT * REGION_WIDTH);
    std::fill(heightmap.begin(), heightmap.end(), (uint8_t)0);
    return heightmap;
}

inline void build_heightmap_from_noise(std::pair<int,int> &target, perlin_noise &noise, std::vector<uint8_t> &heightmap, planet_t &planet) {
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const double nx = noise_x(target.first, x);
            const double ny = noise_y(target.second, y);
            const double nh = noise.noise_octaves ( nx, ny, 0.0, octaves, persistence, frequency );
            const uint8_t altitude = noise_to_planet_height(nh);
            const int cell_idx = (y * REGION_WIDTH) + x;
            heightmap[cell_idx] = altitude - planet.water_height + 5;
        }
    }
}

inline void create_subregions(planet_t &planet, region_t &region, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, random_number_generator &rng) {
    const int n_subregions = 64 + rng.roll_dice(1,20);
    const int region_variance = planet.landblocks[planet.idx(region.region_x, region.region_y)].variance;

    set_worldgen_status("Finding sub-biomes");
    std::vector<std::pair<int,int>> centroids(n_subregions);
    for (std::size_t i=0; i<n_subregions; ++i) {
        centroids[i] = std::make_pair( rng.roll_dice(1, REGION_WIDTH)-1, rng.roll_dice(1, REGION_HEIGHT)-1 );
    }
    std::vector<int> subregion_idx(REGION_HEIGHT * REGION_WIDTH);

    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            float distance = 20000.0;
            int sub_idx = -1;
            int i=0;
            for (const std::pair<int,int> &centroid : centroids) {
                const float D = distance2d(x, y, centroid.first, centroid.second);
                if (D < distance) {
                    distance = D;
                    sub_idx = i;
                }
                ++i;
            }
            subregion_idx[(y*REGION_WIDTH) + x] = sub_idx;

        }
    }

    set_worldgen_status("Making sub-biomes");
    std::vector<int> variance;
    for (int i=0; i<n_subregions; ++i) {
        variance.push_back( rng.roll_dice(1, region_variance+2) - rng.roll_dice(1, region_variance+2)  );
    }

    set_worldgen_status("Applying sub-biomes");
    // TODO: Actual variance rather than just altitude when the raws permit it
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const int tile_idx = (y * WORLD_WIDTH) + x;
            const int sub_idx = subregion_idx[tile_idx];
            const int delta_z = variance[sub_idx];
            heightmap[tile_idx] += delta_z;
        }
    }
}

inline void zero_map(region_t &region) {
    region.next_tree_id = 1;
    std::fill(region.visible.begin(), region.visible.end(), false);
    std::fill(region.solid.begin(), region.solid.end(), false);
    std::fill(region.opaque.begin(), region.opaque.end(), false);
    std::fill(region.revealed.begin(), region.revealed.end(), true);
    std::fill(region.tile_type.begin(), region.tile_type.end(), tile_type::OPEN_SPACE);
    std::fill(region.tile_material.begin(), region.tile_material.end(), 0);
    std::fill(region.tile_hit_points.begin(), region.tile_hit_points.end(), 0);
    std::fill(region.building_id.begin(), region.building_id.end(), 0);
    std::fill(region.tree_id.begin(), region.tree_id.end(), 0);
    std::fill(region.tile_vegetation_type.begin(), region.tile_vegetation_type.end(), 0);
    std::fill(region.water_level.begin(), region.water_level.end(), 0);
}

inline void lay_strata(region_t &region, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome) {

    // Lay down layers
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const int cell_idx = (y * REGION_WIDTH) + x;
            const uint8_t altitude = heightmap[cell_idx];
            bool wet = false;
            if (altitude < 5) wet = true;
            const int soil_height = 4; // Replace this

            // The bottom layer is always SMR to avoid spill-through
            region.tile_type[mapidx(x,y,0)] = tile_type::SEMI_MOLTEN_ROCK;

            int z = 1;
            while (z < altitude) {
                // Place Lava area - SMR for now
                region.tile_type[mapidx(x,y,z)] = tile_type::SEMI_MOLTEN_ROCK;
                region.tile_material[mapidx(x,y,z)] = 0;
                ++z;
            }

            // Next up is rock, until the soil layer
            while(z < altitude + 64 - soil_height) {
                // Place rock
                region.tile_type[mapidx(x,y,z)] = tile_type::SOLID;
                // Select a tile material for the appropriate rock
                ++z;
            }

            // Next up we place the soil layer
            while (z<altitude+64) {
                // Place soil
                region.tile_type[mapidx(x,y,z)] = tile_type::SOLID;
                // Select a tile material for the appropriate soil
                ++z;
            }
            
            // Populate the surface tile at z-1
            region.tile_type[mapidx(x,y,z-1)] = tile_type::FLOOR;
            if (wet) {
                region.water_level[mapidx(x,y,z-1)] = 10; // Below the water line; flood it!
            } else {
                region.water_level[mapidx(x,y,z-1)] = 0;
                // Surface coverage
            }

            while (z<REGION_DEPTH) {
                // Place sky
                region.tile_type[mapidx(x,y,z)] = tile_type::OPEN_SPACE;
                if (z < 69) region.water_level[mapidx(x,y,z)] = 10; // Below the water line; flood it!
                ++z;
            }
        }
    }
}

void build_game_components(region_t &region, const int crash_x, const int crash_y, const int crash_z) {
    calendar_t calendar;
	calendar.defined_shifts.push_back(shift_t{"Early Shift", {
		WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT }
	});
	calendar.defined_shifts.push_back(shift_t{"Day Shift", {
		SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT,	WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT }
	});
	calendar.defined_shifts.push_back(shift_t{"Late Shift", {
		LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT,	WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT }
	});
    
    auto camera = create_entity()
		->assign(world_position_t{region.region_x, region.region_y, crash_x, crash_y, crash_z+1})
		->assign(std::move(calendar))
		->assign(designations_t{});
}

void build_region(planet_t &planet, std::pair<int,int> &target_region, rltk::random_number_generator &rng, perlin_noise &noise) {
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

    // Sub-biome map
    create_subregions(planet, region, heightmap, biome, rng);

    // Lay down rock strata, soil, top tile coverage
    set_worldgen_status("Laying down layers");
    zero_map(region);
    lay_strata(region, heightmap, biome);

    // Build ramps and beaches

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

  
    // Build the ship super-structure
    

    // Add settlers
    // Build connectivity graphs
    set_worldgen_status("Looking for the map");
    region.tile_recalc_all();

    // Save the region
    set_worldgen_status("Saving region to disk");
	save_region(region);
	const std::string save_filename = "world/savegame.dat";
	std::fstream lbfile(save_filename, std::ios::out | std::ios::binary);
	ecs_save(lbfile);
}
