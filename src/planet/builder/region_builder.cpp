#include "region_builder.hpp"
#include "../../raws/raws.hpp"
#include "../constants.hpp"
#include "../planet_builder.hpp"
#include "settler_builder.hpp"
#include "sentient_builder.hpp"
#include "../../components/building.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/lightsource.hpp"
#include "../../components/construct_power.hpp"
#include "../../components/construct_provides_sleep.hpp"
#include "../../components/smoke_emitter.hpp"
#include "../../components/calendar.hpp"
#include "../../components/world_position.hpp"
#include "../../components/logger.hpp"
#include "../../components/designations.hpp"
#include "regions/starting_point.hpp"
#include "regions/heightmap.hpp"
#include "regions/water_features.hpp"
#include "regions/strata.hpp"
#include "regions/decorators.hpp"

#include <rltk.hpp>

using namespace rltk;

inline std::pair<biome_t, biome_type_t> get_biome_for_region(planet_t &planet, const std::pair<int,int> &region) {
    const int idx = planet.idx(region.first, region.second);
    const int biome_idx = planet.landblocks[idx].biome_idx;
    return std::make_pair( planet.biomes[biome_idx], biome_defs[planet.biomes[biome_idx].type] );
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

inline void set_tree_trunk(region_t &region, const int x, const int y, const int z, const int tree_id) {
	if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-2) {
		const auto idx = mapidx(x,y,z);
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
            const auto idx = mapidx(x,y,z);
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
    const auto idx = mapidx(x,y,z);
    region.water_level[idx] = 0;
    region.solid[idx] = false;
    region.tile_type[idx] = tile_type::FLOOR;
    region.tile_flags[idx].set(CONSTRUCTION);
    const auto plasteel = get_material_by_tag("plasteel");
    region.tile_material[idx] = plasteel;
    region.tile_vegetation_type[idx] = 0;

    const auto wood = get_material_by_tag("wood");

    if (type == "ship_wall") {
        region.tile_type[idx] = tile_type::WALL;
        region.solid[idx] = true;
        region.opaque[idx] = true;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel;
    } else if (type == "ship_window") {
        region.tile_type[idx] = tile_type::WINDOW;
        region.solid[idx] = true;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel;
    } else if (type == "ship_floor") {
        region.tile_type[idx] = tile_type::FLOOR;
        region.solid[idx] = false;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel;    
    } else if (type == "hut_wall") {
        region.tile_type[idx] = tile_type::WALL;
        region.solid[idx] = true;
        region.opaque[idx] = true;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = wood;
    } else if (type == "hut_floor") {
        region.tile_type[idx] = tile_type::FLOOR;
        region.solid[idx] = false;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = wood; 
    } else if (type == "ship_up") {
        region.tile_type[idx] = tile_type::STAIRS_UP;
        region.solid[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel;    
    } else if (type == "ship_down") {
        region.tile_type[idx] = tile_type::STAIRS_DOWN;
        region.solid[idx] = false;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel;    
    } else if (type == "ship_updown") {
        region.tile_type[idx] = tile_type::STAIRS_UPDOWN;
        region.solid[idx] = false;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = plasteel;    
    } else if (type == "hut_upstairs") {
        region.tile_type[idx] = tile_type::STAIRS_UP;
        region.solid[idx] = false;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = wood;    
    } else if (type == "hut_downstairs") {
        region.tile_type[idx] = tile_type::STAIRS_DOWN;
        region.solid[idx] = false;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = wood;    
    } else if (type == "hut_updownstairs") {
        region.tile_type[idx] = tile_type::STAIRS_UPDOWN;
        region.solid[idx] = false;
        region.opaque[idx] = false;
        region.tile_flags[idx].set(CONSTRUCTION);
        region.tile_material[idx] = wood;
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
                if (output != nullptr && output->glyph != 32) region.revealed[mapidx(x,y, z)] = true;
				if (output != nullptr && !xp::is_transparent(output)) {
                    if (output->glyph == 219) {
                        add_construction(region, x, y, z, "ship_wall", true);
                    } else if (output->glyph == 177) {
                        add_construction(region, x, y, z, "ship_window", true);
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
		->assign(designations_t{})
        ->assign(logger_t{});
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
                    //if (output->foreground.r != 0 && output->foreground.g != 0 && output->foreground.b != 0)
                    //    std::cout << +output->foreground.r << "," << +output->foreground.g << "," << +output->foreground.b << "\n";
                }

                if (output->glyph == 219 || output->glyph == 177) {
                    add_construction(region, x+X, y+Y, z+layer, "hut_wall", true);
                } else if (output->glyph == 176 || output->glyph == 197) {
                    add_construction(region, x+X, y+Y, z+layer, "hut_floor");
                } else if (output->glyph == '<') {
                    add_construction(region, x+X, y+Y, z+layer, "hut_upstairs");
                } else if (output->glyph == 'X') {
                    add_construction(region, x+X, y+Y, z+layer, "hut_updownstairs");
                } else if (output->glyph == '>') {
                    add_construction(region, x+X, y+Y, z+layer, "hut_downstairs");
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

xp::rex_sprite get_building_template(const std::size_t civ_id, planet_t &planet, rltk::random_number_generator &rng) {
    const auto tech_level = planet.civs.civs[civ_id].tech_level;
    std::vector<std::string> available_buildings;

    // Stone Age
    if (tech_level == TECH_AGE_STONE) {
        available_buildings.push_back("rex/mud-hut.xp");
        available_buildings.push_back("rex/hovel-wood.xp");
        available_buildings.push_back("rex/longhall-wood.xp");
        available_buildings.push_back("rex/henge-wood.xp");
    } else {
        // Bronze age and beyond
        available_buildings.push_back("rex/hovel-wood.xp");
        available_buildings.push_back("rex/mud-hut.xp");
        available_buildings.push_back("rex/hovel-wood.xp");
        available_buildings.push_back("rex/tower-wood.xp");
        available_buildings.push_back("rex/henge-wood.xp");
    }

    const int roll = rng.roll_dice(1, available_buildings.size())-1;
    const std::string building_template = available_buildings[roll];
    std::cout << "Building: " << building_template << "\n";
    return xp::rex_sprite(building_template);
}

void build_buildings(region_t &region, rltk::random_number_generator &rng, const int n_buildings, const bool active, 
    std::vector<std::tuple<int,int,int>> &spawn_points, const std::size_t &civ_id, planet_t &planet) 
{
    int i=0;
    while (i<n_buildings) {
        auto hut = get_building_template(civ_id, planet, rng);
        xp::rex_sprite * building = &hut;
        bool ok = false;
        int tries = 0;
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
                        const auto idx = mapidx(X+x, Y+y, Z+z);
                        if (region.solid[idx]) ok = false;
                        if (region.water_level[idx] > 0) ok = false;
                        if (region.tile_type[idx] == tile_type::SOLID) ok = false;
                    }
                }
            }

            // Not too close to the escape pod!
            if (distance2d(x,y, REGION_WIDTH/2, REGION_HEIGHT/2) < 15.0F) ok = false;

            if (!ok) ++tries;
            if (tries > 50) ok = true;
        }

        // Spawn the hut
        if (tries < 51) {
            const int n_spawn = build_building(*building, x, y, z, region, spawn_points, active);
            i += (n_spawn -1);
        }
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
    lay_strata(region, heightmap, biome, *strata, rng, pooled_water, water_spawners);

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
    std::size_t civ_id;
    for (auto &town : planet.civs.settlements) {
        if (town.world_x == region.region_x && town.world_y == region.region_y) {
            std::cout << "A settlement of type " << +town.status << " should be here.\n";
            std::cout << "It had a peak population of " << town.max_size << "\n";
            std::cout << "It belonged to " << planet.civs.civs[town.civ_id].name << " (" << planet.civs.civs[town.civ_id].species_tag << ")\n";
            std::cout << "With a tech level of " << TECH_LEVELS[planet.civs.civs[town.civ_id].tech_level] << "\n";

            has_settlement = true;
            if (town.status > 0) settlement_active = true;
            max_size += town.max_size;
            civ_id = town.civ_id;
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
