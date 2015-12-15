#include "world_gen.hpp"
#include "world_height_map.hpp"
#include "world_gen_biomes.hpp"
#include "world_gen_hydrology.hpp"
#include "../../../engine/rng.h"
#include "world_gen_layer_cake.hpp"
#include "world_gen_noise.hpp"
#include <iostream>
#include "../../game.h"
#include "../universe.hpp"
#include "../../raws/raws.h"
#include "../../../engine/colors.h"

using engine::entity;
using namespace engine;

void crash_the_ship ( const uint8_t start_x, const uint8_t start_y, const uint8_t start_z, const uint8_t planet_idx, planet_t * planet ) {
    for (uint8_t X = 0; X < start_x+6; ++X) {
	const int half_width = 5 + ( X / 64 );
	for ( uint8_t Y = start_y-half_width; Y<start_y+half_width; ++Y ) {
	    uint8_t Z = 1;
	    bool found = false;
	    while (!found) {
		tile_t * candidate = planet->get_tile( location_t{ planet_idx, X, Y, Z }  );
		if ( candidate->solid ) {
		    ++Z;
		} else {
		    found = true;
		    candidate->covering = tile_covering::BARE;
		}
	    }
	}
    }
    planet->save_region( planet_idx );
}

void add_camera( const uint8_t start_x, const uint8_t start_y, const uint8_t start_z, const uint8_t planet_idx ) {
    entity camera = game_engine->ecs->add_entity();
    game_engine->ecs->add_component(camera, position_component3d({ planet_idx, start_x, start_y, start_z }, OMNI));
    universe->globals.camera_handle = camera.handle;
    world::camera_handle = camera.handle;
}

void add_cordex( const uint8_t start_x, const uint8_t start_y, const uint8_t start_z, const uint8_t planet_idx ) {
    entity cordex = game_engine->ecs->add_entity();
    world::cordex_handle = cordex.handle;
    game_engine->ecs->add_component(cordex, debug_name_component("Cordex"));
    game_engine->ecs->add_component(cordex, position_component3d( location_t{ planet_idx, start_x, start_y, start_z }, OMNI));
    game_engine->ecs->add_component(cordex, viewshed_component(penetrating,16));
    game_engine->ecs->add_component(cordex, calendar_component(0L));
    game_engine->ecs->add_component(cordex, renderable_component(15, cyan, black,17));
    game_engine->ecs->add_component(cordex, obstruction_component());
    game_engine->ecs->add_component(cordex, description_component( "You! The ship-board AI responsible for keeping these barely-functional hairless ape-descendents alive."));
}

void add_solar_collector(const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t planet_idx) {
    raws::create_structure_from_raws("Solar Collector", location_t { planet_idx, x, y, z });
}

void add_food_replicator(const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t planet_idx) {
    raws::create_structure_from_raws("Small Replicator", location_t { planet_idx, x, y, z });
}

void add_storage_unit(const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t planet_idx) {
    int container_id = raws::create_structure_from_raws("Storage Unit", location_t { planet_idx, x, y, z });
    
    for (int i=0; i<3; ++i) {
	int tent_kit = raws::create_item_from_raws("Personal Survival Shelter Kit");
	game_engine->ecs->add_component<item_storage_component>( *game_engine->ecs->get_entity_by_handle( tent_kit ), item_storage_component(container_id) );
    }
    int fire_kit = raws::create_item_from_raws("Camping Fire Kit");
    game_engine->ecs->add_component<item_storage_component>( *game_engine->ecs->get_entity_by_handle( fire_kit ), item_storage_component(container_id) );
    int fire_axe = raws::create_item_from_raws("Fire Axe");
    game_engine->ecs->add_component<item_storage_component>( *game_engine->ecs->get_entity_by_handle( fire_axe ), item_storage_component(container_id) );
}

void make_entities( planet_t * planet ) {
    const uint8_t planet_idx = planet->planet_idx( WORLD_WIDTH/2, WORLD_HEIGHT-1);
    
    // Determine starting location
    uint8_t start_x = REGION_WIDTH/2;
    uint8_t start_y = REGION_HEIGHT/2;
    uint8_t start_z = 1;
    
    planet->load_region( planet_idx );
    bool found = false;
    while (!found) {
	tile_t * candidate = planet->get_tile( location_t{ planet_idx, start_x, start_y, start_z }  );
	if ( candidate->solid ) {
	  ++start_z;
	} else {
	  found = true;
	}
    }
    std::cout << "Starting at: " << +start_x << "/" << +start_y << "/" << +start_z << "\n";
    
    // Clear a crash trail
    crash_the_ship( start_x, start_y, start_z, planet_idx, planet );
    
    // Add ship hull, superstructure, doors, power, sensors, cordex
    add_cordex ( start_x, start_y, start_z, planet_idx );
    // Solar collector at (x-1,y-1), (x+1,y-1), (x-1,y+1), (x+1,y+1)
    add_solar_collector(static_cast<uint8_t>(start_x-1), static_cast<uint8_t>(start_y-1), static_cast<uint8_t>(start_z+1), planet_idx);
    add_solar_collector(static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y-1), static_cast<uint8_t>(start_z+1), planet_idx);
    add_solar_collector(static_cast<uint8_t>(start_x-1), static_cast<uint8_t>(start_y+1), static_cast<uint8_t>(start_z+1), planet_idx);
    add_solar_collector(static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y+1), static_cast<uint8_t>(start_z+1), planet_idx);
    
    // Console constructions at (x-1,y), (x+1,y), (x,y-1), (x,y+1)
    raws::create_structure_from_raws("Education Console", location_t {static_cast<uint8_t>(start_x-1), start_y, start_z});
    raws::create_structure_from_raws("Scanner Console", location_t {static_cast<uint8_t>(start_x+1), start_y, start_z});
    raws::create_structure_from_raws("Defense Console", location_t {start_x, static_cast<uint8_t>(start_y-1), start_z});
    raws::create_structure_from_raws("Communications Console", location_t {start_x, static_cast<uint8_t>(start_y+1), start_z});
    raws::create_structure_from_raws("Water Purifier", location_t {static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y-2), start_z} );
    raws::create_structure_from_raws("Food Dispenser", location_t {static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y+2), start_z} );
    
    // Refridgerator/Food Replicator at (x+4,y)
    add_food_replicator(start_x+4, start_y, start_z, planet_idx);
    
    // Storage unit at (x+4,y-1) and (x+4,y+1)
    add_storage_unit(start_x+4, start_y-1, start_z, planet_idx);
    add_storage_unit(start_x+4, start_y+1, start_z, planet_idx);
    
    // Add random settlers    
    
    add_camera( start_x, start_y, start_z, planet_idx );
    world::stored_power = 25;
  
    game_engine->ecs->save_game("world/savegame3d.dat");
}

void world_gen_phase_1()
{
    std::cout << "World gen running\n";
    engine::random_number_generator rng;
  
    std::unique_ptr<heightmap_t> base_map = get_height_map();
    std::cout << "Zero Height Map\n";
    zero_height_map( base_map.get() );
    
    std::cout << "Making a noise-based height-map\n";
    make_noisy_heightmap( base_map.get(), &rng );
    smooth_height_map( base_map.get() );
    
    std::cout << "Hydrology\n";
    std::unique_ptr<water_level_map_t> water = perform_hydrology( base_map.get(), &rng );
        
    std::cout << "Biomes\n";    
    biome_map_t biomes = make_biome_map( base_map.get(), &rng, water.get() );
    
    std::cout << "Layercake\n";
    std::unique_ptr<planet_t> planet = make_world_layers( base_map.get(), rng, water.get(), &biomes );
    
    std::cout << "Making starting entites\n";    
    // Make the camera
    make_entities( planet.get() );
    
    std::cout << "World gen done\n";
}
