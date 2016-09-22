#include "region_builder.hpp"
#include "../../raws/raws.hpp"
#include "../constants.hpp"
#include "../planet_builder.hpp"
#include "noise_helper.hpp"
#include "../../components/components.hpp"
#include "settler_builder.hpp"
#include "sentient_builder.hpp"
#include <rltk.hpp>

using namespace rltk;

inline bool has_civilization(planet_t &planet, const int &x, const int &y) {
    for (auto &town : planet.civs.settlements) {
        if (town.world_x == x && town.world_y == y && town.status > 0) {
            return true;
        }
    }
    return false;
}

std::pair<int,int> builder_select_starting_region(planet_t &planet) {
    std::pair<int,int> coords = std::make_pair(WORLD_WIDTH/2, WORLD_HEIGHT/2);

    bool ok = false;
    while (!ok) {
        ok = true;
        if (planet.landblocks[planet.idx(coords.first, coords.second)].type == block_type::WATER) ok = false;
        if (planet.biomes[planet.landblocks[planet.idx(coords.first, coords.second)].biome_idx].mean_altitude < planet.water_height+2) ok = false;
        if (!has_civilization(planet, coords.first, coords.second)) ok = false;

        if (!ok) {
            --coords.first;
            if (coords.first < 0) {
                coords.first = WORLD_WIDTH-1;
                --coords.second;
            }
        }        
    }
    std::cout << biome_defs[planet.biomes[planet.landblocks[planet.idx(coords.first, coords.second)].biome_idx].type].name << "\n";
    
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

inline void build_heightmap_from_noise(std::pair<int,int> &target, FastNoise &noise, std::vector<uint8_t> &heightmap, planet_t &planet) {
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const double nx = noise_x(target.first, x);
            const double ny = noise_y(target.second, y);
            const double nh = noise.GetNoise ( nx, ny );
            const uint8_t altitude = noise_to_planet_height(nh);
            const int cell_idx = (y * REGION_WIDTH) + x;
            heightmap[cell_idx] = altitude - planet.water_height + 5;
        }
    }
}

inline std::vector<int> create_subregions(planet_t &planet, region_t &region, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, random_number_generator &rng) {
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
                const float D = distance2d_squared(x, y, centroid.first, centroid.second);
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
        const int up_variance = rng.roll_dice(1, 2)-1;
        const int down_variance = rng.roll_dice(1, 2)-1;
        variance.push_back( up_variance - down_variance );
    }

    set_worldgen_status("Applying sub-biomes");
    // TODO: Actual variance rather than just altitude when the raws permit it
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const int tile_idx = (y * REGION_WIDTH) + x;
            const int sub_idx = subregion_idx[tile_idx];
            const int delta_z = variance[sub_idx];
            if (distance2d(x,y,REGION_WIDTH/2,REGION_HEIGHT/2) > 20) {
                heightmap[tile_idx] += delta_z;
            } else {
                heightmap[tile_idx] = heightmap[ ( REGION_HEIGHT/2 * REGION_WIDTH) + ((REGION_WIDTH/2) - 20) ];
            }
        }
    }
	return subregion_idx;
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
    std::fill(region.water_level.begin(), region.water_level.end(), 0);
    std::fill(region.blood_stains.begin(), region.blood_stains.end(), false);
}

struct strata_t {
    std::vector<int> strata_map;
    std::vector<std::size_t> material_idx;
    std::vector<std::tuple<int,int,int,int>> counts;
};

strata_t build_strata(region_t &region, std::vector<uint8_t> &heightmap, random_number_generator &rng, std::pair<biome_t, biome_type_t> &biome, planet_t &planet) {
    strata_t result;
    result.strata_map.resize(REGION_TILES_COUNT);

    std::vector<std::size_t> soils;
    std::vector<std::size_t> sedimintaries;
    std::vector<std::size_t> igneouses;
    std::vector<std::size_t> sands;

    std::size_t i = 0;
    for (auto it=material_defs.begin(); it != material_defs.end(); ++it) {
        if (it->spawn_type == soil) soils.push_back(i);
        if (it->spawn_type == sand) sands.push_back(i);
        if (it->spawn_type == rock && it->layer == "sedimentary") sedimintaries.push_back(i);
        if (it->spawn_type == rock && it->layer == "igneous") igneouses.push_back(i);
        ++i;
    }
    std:: cout << soils.size() << "/" << sands.size() << "/" << sedimintaries.size() << "/" << igneouses.size() << "\n";

    set_worldgen_status("Locating strata");
    const int n_strata = (REGION_WIDTH + REGION_HEIGHT)*4 + rng.roll_dice(1,64);
    result.material_idx.resize(n_strata);
    result.counts.resize(n_strata);

    FastNoise biome_noise(planet.perlin_seed + (region.region_y * REGION_WIDTH ) + region.region_x);
    biome_noise.SetNoiseType(FastNoise::Cellular);

    for (int z=0; z<REGION_DEPTH; ++z) {
        const float Z = (float)z*8.0F;
        for (int y=0; y<REGION_WIDTH; ++y) {
            const float Y = (float)y*8.0F;
            for (int x=0; x<REGION_HEIGHT; ++x) {
                const float X = (float)x*8.0F;
                const float biome_ramp = (biome_noise.GetCellular(X,Y,Z) + 1.0F)/2.0F;
                int biome_idx = biome_ramp * n_strata;
                ++std::get<0>(result.counts[biome_idx]);
                std::get<1>(result.counts[biome_idx]) += x;
                std::get<2>(result.counts[biome_idx]) += y;
                std::get<3>(result.counts[biome_idx]) += z;
                const int map_idx = mapidx(x,y,z);
                result.strata_map[map_idx] = biome_idx;
                //std::cout << x << "/" << y << "/" << z << " : " << X << "/" << Y << "/" << Z << " = " << biome_ramp << " : " << biome_idx << "\n";
            }
        }
    }

    int count_used = 0;
    for (int i=0; i<n_strata; ++i) {
        if (std::get<0>(result.counts[i])>0) {
            ++count_used;
            std::get<1>(result.counts[i]) /= std::get<0>(result.counts[i]);
            std::get<2>(result.counts[i]) /= std::get<0>(result.counts[i]);
            std::get<3>(result.counts[i]) /= std::get<0>(result.counts[i]);

            int n,x,y,z;
            std::tie(n,x,y,z) = result.counts[i];
            const uint8_t altitude_at_center = heightmap[(y * REGION_WIDTH) + x] + REGION_DEPTH/2;

            if (z>altitude_at_center-(1+rng.roll_dice(1,4))) {
                // Soil
                int roll = rng.roll_dice(1,100);
                if (roll < biome.second.soil_pct) {
                    const std::size_t soil_idx = rng.roll_dice(1, soils.size())-1;
                    //std::cout << material_defs[soils[soil_idx]].name << "\n";
                    result.material_idx[i] = soils[soil_idx];
                } else {
                    const std::size_t sand_idx = rng.roll_dice(1, sands.size())-1;
                    //std::cout << material_defs[sands[sand_idx]].name << "\n";
                    result.material_idx[i] = sands[sand_idx];
                }
            } else if (z>(altitude_at_center-10)/2) {
                // Sedimentary
                const std::size_t sed_idx = rng.roll_dice(1, sedimintaries.size())-1;
                //std::cout << material_defs[sedimintaries[sed_idx]].name << "\n";
                result.material_idx[i] = sedimintaries[sed_idx];
            } else {
                // Igneous
                const std::size_t ig_idx = rng.roll_dice(1, igneouses.size())-1;
                //std::cout << material_defs[igneouses[ig_idx]].name << "\n";
                result.material_idx[i] = igneouses[ig_idx];
            }
        }
    }
    std::cout << count_used << " strata detected, " << n_strata - count_used << " unused.\n";

    return result;
}

void lay_strata(region_t &region, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, strata_t &strata, random_number_generator &rng) {
    // For vegetation
    int max_veg_probability = 0;
    for (const auto &vegprob : biome.second.plants) max_veg_probability += vegprob.second;

    // Lay down layers
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const int cell_idx = (y * REGION_WIDTH) + x;
            const uint8_t altitude = heightmap[cell_idx];
            bool wet = false;
            if (altitude < 5) wet = true;

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
            while(z < std::min(altitude + 64, REGION_DEPTH-20)) {
                // Place rock and soil
                region.tile_type[mapidx(x,y,z)] = tile_type::SOLID;
                const int strata_idx = strata.strata_map[mapidx(x,y,z)];
                const std::size_t material_idx = strata.material_idx[strata_idx];
                region.tile_material[mapidx(x,y,z)] = material_idx;
                if (material_idx < material_defs.size()) {
                    region.tile_hit_points[mapidx(x,y,z)] = material_defs[material_idx].hit_points;
                } else {
                    region.tile_hit_points[mapidx(x,y,z)] = 10;
                }
                ++z;
            }
            
            // Populate the surface tile at z-1
            region.revealed[mapidx(x,y,z-1)] = true;
            region.tile_type[mapidx(x,y,z-1)] = tile_type::FLOOR;
            if (wet) {
                region.water_level[mapidx(x,y,z-1)] = 10; // Below the water line; flood it!
            } else {
                region.water_level[mapidx(x,y,z-1)] = 0;
                
                // Surface coverage
                std::string veg_type = "";
                int die_roll = rng.roll_dice(1, max_veg_probability);
                for (const auto &veg : biome.second.plants) {
                    die_roll -= veg.second;
                    if (die_roll < 1) {
                        veg_type = veg.first;
                        break;
                    }
                }
                if (veg_type == "") veg_type = "none";

                if (veg_type != "none") {
                    auto finder = plant_defs_idx.find(veg_type);
                    region.tile_vegetation_type[mapidx(x,y,z-1)] = finder->second;
                    region.tile_hit_points[mapidx(x,y,z-1)] = 10;
                }
            }

            while (z<REGION_DEPTH) {
                // Place sky
                region.revealed[mapidx(x,y,z)] = true;
                region.tile_type[mapidx(x,y,z)] = tile_type::OPEN_SPACE;
                if (z < 69) region.water_level[mapidx(x,y,z)] = 10; // Below the water line; flood it!
                ++z;
            }
        }
    }
}

void build_ramps(region_t &region) {
    for (int y=1; y<REGION_HEIGHT-1; ++y) {
        for (int x=1; x<REGION_WIDTH-1; ++x) {
            const int z = get_ground_z(region,x,y);
            if (region.tile_type[mapidx(x,y,z)] == tile_type::FLOOR) {
                bool is_ramp = false;
                if (region.tile_type[mapidx(x,y-1,z+1)] == tile_type::FLOOR) is_ramp = true;
                if (region.tile_type[mapidx(x,y+1,z+1)] == tile_type::FLOOR) is_ramp = true;
                if (region.tile_type[mapidx(x-1,y,z+1)] == tile_type::FLOOR) is_ramp = true;
                if (region.tile_type[mapidx(x+1,y,z+1)] == tile_type::FLOOR) is_ramp = true;

                if (is_ramp) {
                    const int idx = mapidx(x,y,z);
                    region.tile_type[idx]=tile_type::RAMP;
                    region.revealed[idx] = true;
                }
            }
        }
    }

    for (int z=1; z<REGION_DEPTH-1; ++z) {
        for (int y=1; y<REGION_HEIGHT-1; ++y) {
            for (int x=1; x<REGION_WIDTH-1; ++x) {
                if (region.tile_type[mapidx(x,y,z)] == tile_type::SOLID) {
                    bool is_edge = false;
                    if (region.tile_type[mapidx(x,y-1,z)] == tile_type::FLOOR || region.tile_type[mapidx(x,y-1,z)] == tile_type::OPEN_SPACE ||region.tile_type[mapidx(x,y-1,z)] == tile_type::RAMP) is_edge = true;
                    if (region.tile_type[mapidx(x,y+1,z)] == tile_type::FLOOR || region.tile_type[mapidx(x,y+1,z)] == tile_type::OPEN_SPACE || region.tile_type[mapidx(x,y+1,z)] == tile_type::RAMP) is_edge = true;
                    if (region.tile_type[mapidx(x-1,y,z)] == tile_type::FLOOR || region.tile_type[mapidx(x-1,y,z)] == tile_type::OPEN_SPACE || region.tile_type[mapidx(x-1,y,z)] == tile_type::RAMP) is_edge = true;
                    if (region.tile_type[mapidx(x+1,y,z)] == tile_type::FLOOR || region.tile_type[mapidx(x+1,y,z)] == tile_type::OPEN_SPACE || region.tile_type[mapidx(x+1,y,z)] == tile_type::RAMP) is_edge = true;

                    if (is_edge) {
                        region.revealed[mapidx(x,y,z)] = true;
                    }
                }
            }
        }
    }
}

void build_debris_trail(region_t &region, const int crash_x, const int crash_y) {
    set_worldgen_status("Crashing the ship");
    for (int x=crash_x - (REGION_WIDTH/4); x<crash_x; ++x) {
		for (int y=crash_y - 3; y<crash_y+4; ++y) {
			int z = get_ground_z(region, x, y);
			region.tile_vegetation_type[mapidx(x,y,z)] = 0;
		}
	}
}

void build_beaches(region_t &region) {
    auto finder = material_defs_idx.find("yellow_sand");
    for (int y=1; y<WORLD_HEIGHT-1; ++y) {
        for (int x=1; x<WORLD_WIDTH; ++x) {
            const int z = get_ground_z(region,x,y);
            if (region.tile_type[mapidx(x,y,z)] == tile_type::FLOOR && region.water_level[mapidx(x,y,z)]==0) {
                bool is_beach = false;
                if (region.water_level[mapidx(x,y-1,z-1)] > 0) is_beach = true;
                if (region.water_level[mapidx(x,y+1,z-1)] > 0) is_beach = true;
                if (region.water_level[mapidx(x-1,y,z-1)] > 0) is_beach = true;
                if (region.water_level[mapidx(x+1,y,z-1)] > 0) is_beach = true;

                if (is_beach) {
                    region.tile_material[mapidx(x,y,z)] = finder->second;
                    region.tile_vegetation_type[mapidx(x,y,z)] = 0;
                }
            }
        }
    }
}

inline void set_tree_trunk(region_t &region, const int x, const int y, const int z, const int tree_id) {
	if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-2) {
		const int idx = mapidx(x,y,z);
        region.tile_type[idx] = tile_type::TREE_TRUNK;
        region.tree_id[idx] = tree_id;
        region.solid[idx] = true;
	}
}

inline void set_tree_foliage(region_t &region, const int x, const int y, const int z, const int tree_id) {
	if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-2) {
		const int idx = mapidx(x,y,z);
        region.tile_type[idx] = tile_type::TREE_LEAF;
        region.tree_id[idx] = tree_id;
        region.solid[idx] = true;
	}
}

inline void plant_tree_deciduous(region_t &region, const int x, const int y, const int z, random_number_generator &rng) {
	// Trees get their own entity
	const int tree_height = 1 + rng.roll_dice(2,4);
	for (int i=0; i<tree_height; ++i) {
		set_tree_trunk(region, x, y, z+i, region.next_tree_id);
		if ( i > tree_height/2) {
			const int radius = (tree_height - i) + 1;
			for (int X=x-radius; X<x+radius; ++X) {
				for (int Y=y-radius; Y<y+radius; ++Y) {
					const float distance = distance2d(x,y,X,Y);
					if (distance <= radius && (X!=x || Y!=y)) {
						set_tree_foliage(region, X, Y, z+i, region.next_tree_id);
					}
				}
			}
		}
	}
	++region.next_tree_id;
}

inline void plant_tree_evergreen(region_t &region, const int x, const int y, const int z, random_number_generator &rng) {
	// Trees get their own entity
	const int tree_height = 1 + rng.roll_dice(2,3);
	for (int i=0; i<tree_height; ++i) {
		set_tree_trunk(region, x, y, z+i, region.next_tree_id);
		if ( i > 0) {
			const int radius = (tree_height - i)/2 + 1;
			for (int X=x-radius; X<x+radius; ++X) {
				for (int Y=y-radius; Y<y+radius; ++Y) {
					const float distance = distance2d(x,y,X,Y);
					if (distance <= radius && (X!=x || Y!=y)) {
						set_tree_foliage(region, X, Y, z+i, region.next_tree_id);
					}
				}
			}
		}
	}
	++region.next_tree_id;
}

void build_trees(region_t &region, std::pair<biome_t, biome_type_t> &biome, random_number_generator &rng) {
    const int d_chance = biome.second.deciduous_tree_chance;
    const int e_chance = biome.second.evergreen_tree_chance;

    for (int y=10; y<REGION_HEIGHT-10; ++y) {
        for (int x=10; x<REGION_WIDTH-10; ++x) {
            const int z = get_ground_z(region,x,y);
            const int idx = mapidx(x,y,z);
            const int crash_distance = distance2d(x,y,REGION_WIDTH/2,REGION_HEIGHT/2);
            if (crash_distance > 20 && region.tile_type[idx] == tile_type::FLOOR && region.water_level[idx]==0) {
                int dice_roll = rng.roll_dice(1,1000);
                if (dice_roll <= d_chance) {
                    // Grow a decidous tree
                    plant_tree_deciduous(region, x, y, z, rng);
                } else {
                    dice_roll = rng.roll_dice(1,1000);
                    if (dice_roll < e_chance) {
                        plant_tree_evergreen(region, x, y, z, rng);
                    }
                }
            }
        }
    }
}

void add_building(std::string tag, const int x, const int y, const int z) {
    auto building = building_defs.find(tag);
    if (building == building_defs.end()) std::cout << "Warning: do not know how to build " << tag << "\n";

    auto new_building = create_entity()
        ->assign(position_t{x, y, z})
        ->assign(building_t{ tag, building->second.width, building->second.height, building->second.glyphs, true });

    for (const building_provides_t &provides : building->second.provides) {
        if (provides.provides == provides_sleep) new_building->assign(construct_provides_sleep_t{});
    }

    if (tag == "storage_locker") {
        spawn_item_in_container(new_building->id, "personal_survival_shelter_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "personal_survival_shelter_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "personal_survival_shelter_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "camp_fire_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "fire_axe", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "pickaxe", get_material_by_tag("plasteel"));
    } else if (tag == "cordex") {
        new_building->assign(viewshed_t{16, false})
            ->assign(lightsource_t{16, rltk::colors::WHITE, true});
    } else if (tag == "battery") {
        new_building->assign(construct_power_t{20,0,0});
    } else if (tag == "rtg") {
        new_building->assign(construct_power_t{0,1,0});
    } else if (tag == "solar_panel") {
        new_building->assign(construct_power_t{00,0,1});
    } else if (tag == "camp_fire") {
        new_building->assign(lightsource_t{5, rltk::colors::YELLOW});
        new_building->assign(smoke_emitter_t{});
    }
}

void add_construction(region_t &region, const int x, const int y, const int z, const std::string type, bool solid=false) {
    const int idx = mapidx(x,y,z);
    region.water_level[idx] = 0;
    region.solid[idx] = false;
    region.tile_type[idx] = tile_type::FLOOR;
    region.tile_flags[idx].set(CONSTRUCTION);
    auto plasteel = material_defs_idx.find("plasteel");
    if (plasteel == material_defs_idx.end()) std::cout << "Warning: Unable to locate plasteel\n";
    region.tile_material[idx] = plasteel->second;
    region.tile_vegetation_type[idx] = 0;

    auto wood = material_defs_idx.find("wood");

    if (type == "ship_wall") {
        region.tile_type[idx] = tile_type::WALL;
        region.solid[idx] = true;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel->second;
    } else if (type == "ship_floor") {
        region.tile_type[idx] = tile_type::FLOOR;
        region.solid[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel->second;    
    } else if (type == "hut_wall") {
        region.tile_type[idx] = tile_type::WALL;
        region.solid[idx] = true;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = wood->second;
    } else if (type == "hut_floor") {
        region.tile_type[idx] = tile_type::FLOOR;
        region.solid[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = wood->second; 
    }else if (type == "ship_up") {
        region.tile_type[idx] = tile_type::STAIRS_UP;
        region.solid[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel->second;    
    } else if (type == "ship_down") {
        region.tile_type[idx] = tile_type::STAIRS_DOWN;
        region.solid[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel->second;    
    } else if (type == "ship_updown") {
        region.tile_type[idx] = tile_type::STAIRS_UPDOWN;
        region.solid[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel->second;    
    } else if (type == "cordex") {
        add_building("cordex", x, y, z);
    } else if (type == "solar_panel") {
        add_building("solar_panel", x, y, z);
    } else if (type == "cryo_bed") {
        region.tile_flags[idx].reset(CONSTRUCTION);
        add_building("cryo_bed", x, y, z);
    } else if (type == "storage_locker") {
        add_building("storage_locker", x, y, z);
    } else if (type == "battery") {
        add_building("battery", x, y, z);
    } else if (type == "rtg") {
        add_building("rtg", x, y, z);
    } else if (type == "small_replicator") {
        add_building("small_replicator", x, y, z);
    } else if (type == "campfire") {
        add_building("camp_fire", x, y, z);
    } else {
        std::cout << "Don't know how to build a " << type << "\n";
    }
}

void build_escape_pod(region_t &region, const int crash_x, const int crash_y, const int crash_z) {
    xp::rex_sprite ship("rex/spaceship.xp");
    int z=-2+crash_z;
    for (int layer = 0; layer<5; ++layer) {
        for (int Y=0; Y<ship.get_height(); ++Y) {
            for (int X=0; X<ship.get_width(); ++X) {
                const int x = X-5+crash_x;
                const int y = Y-11+crash_y;

                const vchar * output = ship.get_tile(layer,X,Y);
				if (output != nullptr && !xp::is_transparent(output)) {
                    if (output->glyph == 219 || output->glyph == 177) {
                        add_construction(region, x, y, z, "ship_wall", true);
                    } else if (output->glyph == 176 || output->glyph == 197) {
                        add_construction(region, x, y, z, "ship_floor");
                    } else if (output->glyph == 'X') {
                        add_construction(region, x, y, z, "ship_updown");
                    } else if (output->glyph == '<') {
                        add_construction(region, x, y, z, "ship_up");
                    } else if (output->glyph == '>') {
                        add_construction(region, x, y, z, "ship_down");
                    } else if (output->glyph == 178) {
                        add_construction(region, x, y, z, "solar_panel");
                    } else if (output->glyph == 241) {
                        add_construction(region, x, y, z, "battery");
                    } else if (output->glyph == 'X') {
                        add_construction(region, x, y, z, "ship_updown");
                    } else if (output->glyph == '0') {
                        add_construction(region, x, y, z, "cryo_bed");
                    } else if (output->glyph == 'X') {
                        add_construction(region, x, y, z, "ship_updown");
                    } else if (output->glyph == 236) {
                        add_construction(region, x, y, z, "storage_locker");
                    } else if (output->glyph == 'C') {
                        add_construction(region, x, y, z, "cordex");
                    } else if (output->glyph == 251) {
                        add_construction(region, x, y, z, "small_replicator");
                    } else if (output->glyph == 232) {
                        add_construction(region, x, y, z, "rtg");
                    } else {
                        if (output->glyph != 32)
                            std::cout << "Warning: No handler for " << (char)output->glyph << " (" << +output->glyph << ")\n";
                    }
                }
            }
        }
        ++z;
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

inline int build_building(xp::rex_sprite &sprite, const int x, const int y, const int z, region_t &region, std::vector<std::tuple<int,int,int>> &spawn_points, const bool active) {
    int n_spawn = 0;
    for (int layer = 0; layer < sprite.get_num_layers(); ++layer) {
        for (int Y=0; Y < sprite.get_height(); ++Y) {
            for (int X = 0; X < sprite.get_width(); ++X) {
                const vchar * output = sprite.get_tile(layer,X,Y);
                int material = 0;
                if (output->foreground.r == 102 && output->foreground.g == 82 && output->foreground.b == 51) {
                    material = 1; // Wood
                } else {
                    if (output->foreground.r != 0 && output->foreground.g != 0 && output->foreground.b != 0)
                        std::cout << +output->foreground.r << "," << +output->foreground.g << "," << +output->foreground.b << "\n";
                }

                if (output->glyph == 219 || output->glyph == 177) {
                    add_construction(region, x+X, y+Y, z+layer, "hut_wall", true);
                } else if (output->glyph == 176 || output->glyph == 197) {
                    add_construction(region, x+X, y+Y, z+layer, "hut_floor");
                } else if (output->glyph == 's') {
                    add_construction(region, x+X, y+Y, z+layer, "hut_floor");
                    spawn_points.push_back(std::make_tuple(x+X, y+Y, z+layer));
                    ++n_spawn;
                } else if (output->glyph == 234) {
                    add_construction(region, x+X, y+Y, z+layer, "hut_floor");
                    if (active) {
                        add_construction(region, x+X, y+Y, z+layer, "campfire");
                    }
                }
            }
        }
    }
    return n_spawn;
}

void build_buildings(region_t &region, rltk::random_number_generator &rng, const int n_buildings, const bool active, std::vector<std::tuple<int,int,int>> &spawn_points) {
    // Temporary code - we're just going to spawn mud huts with no regard to sanity. :-X
    auto hut = xp::rex_sprite("rex/mud-hut.xp");

    xp::rex_sprite * building = &hut;

    for (int i=0; i<n_buildings; ++i) {
        bool ok = false;
        int x,y,z;
        while (!ok) {
            ok = true;
            x = rng.roll_dice(1, REGION_WIDTH - 20) + 10;
            y = rng.roll_dice(1, REGION_HEIGHT - 20) + 10;
            z = get_ground_z(region, x, y);

            // Find lowest point
            for (int Y=0; Y < building->get_height(); ++Y) {
                for (int X = 0; X < building->get_width(); ++X) {
                    const int tmp_z = get_ground_z(region, x+X, y+Y);
                    if (z > tmp_z) z = tmp_z;
                }
            }

            // Check for solid or underwater
            for (int Y=0; Y < building->get_height(); ++Y) {
                for (int X = 0; X < building->get_width(); ++X) {
                    for (int Z = 0; Z < building->get_num_layers(); ++Z) {
                        const int idx = mapidx(X+x, Y+y, Z+z);
                        if (region.solid[idx]) ok = false;
                        if (region.water_level[idx] > 0) ok = false;
                        if (region.tile_type[idx] == tile_type::SOLID) ok = false;
                    }
                }
            }

            // Not too close to the escape pod!
            if (distance2d(x,y, REGION_WIDTH/2, REGION_HEIGHT/2) < 15.0F) ok = false;
        }

        // Spawn the hut
        const int n_spawn = build_building(*building, x, y, z, region, spawn_points, active);
        i += (n_spawn -1);
    }
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

    // Sub-biome map
    auto subregions = create_subregions(planet, region, heightmap, biome, rng);

    // Strata map
    set_worldgen_status("Dividing strata");
    auto strata = build_strata(region, heightmap, rng, biome, planet);

    // Lay down rock strata, soil, top tile coverage
    set_worldgen_status("Laying down layers");
    zero_map(region);
    lay_strata(region, heightmap, biome, strata, rng);

    // Build ramps and beaches
    build_ramps(region);
    build_beaches(region);

    // Plant trees
    set_worldgen_status("Planting trees");
    build_trees(region, biome, rng);    

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
    create_settler(crash_x - 3, crash_y - 2, crash_z+1, rng, 0);
	create_settler(crash_x - 2, crash_y - 2, crash_z+1, rng, 0);
	create_settler(crash_x - 1, crash_y - 2, crash_z+1, rng, 0);
	create_settler(crash_x, crash_y - 2, crash_z+1, rng, 1);
	create_settler(crash_x + 1, crash_y - 2, crash_z+1, rng, 1);
	create_settler(crash_x - 3, crash_y, crash_z+1, rng, 1);
	create_settler(crash_x - 2, crash_y, crash_z+1, rng, 1);
	create_settler(crash_x - 1, crash_y, crash_z+1, rng, 2);
	create_settler(crash_x, crash_y, crash_z+1, rng, 2);
	create_settler(crash_x + 1, crash_y, crash_z+1, rng, 2);

    // Add features
    bool has_settlement = false;
    bool settlement_active = false;
    int max_size = 0;
    for (auto &town : planet.civs.settlements) {
        if (town.world_x == region.region_x && town.world_y == region.region_y) {
            std::cout << "A settlement of type " << +town.status << " should be here.\n";
            std::cout << "It had a peak population of " << town.max_size << "\n";
            std::cout << "It belonged to " << planet.civs.civs[town.civ_id].name << " (" << planet.civs.civs[town.civ_id].species_tag << ")\n";
            std::cout << "With a tech level of " << TECH_LEVELS[planet.civs.civs[town.civ_id].tech_level] << "\n";

            has_settlement = true;
            if (town.status > 0) settlement_active = true;
            max_size += town.max_size;
        }
    }

    std::vector<std::tuple<int,int,int>> spawn_points;
    if (has_settlement) {
        const int n_buildings = max_size * 5;
        std::cout << "Spawning " << n_buildings << " buildings. \n";
        if (!settlement_active) std::cout << "The buildings are ruined.\n";
        build_buildings(region, rng, n_buildings, settlement_active, spawn_points);
    }

    // Add anyone who is still here from world-gen
    int count = 0;
    std::size_t peep_id = 0;
    for (auto &peep : planet.civs.unimportant_people) {
        if (!peep.deceased && peep.world_x == region.region_x && peep.world_y == region.region_y) {
            for (int i=0; i<5; ++i) {
                std::cout << "Spawn a " << TECH_LEVELS[planet.civs.civs[peep.civ_id].tech_level] << " " << peep.species_tag << " " << OCCUPATION_NAMES[peep.occupation] << ", of the " << planet.civs.civs[peep.civ_id].name << "!\n";

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
        }
        ++peep_id;
    }

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
