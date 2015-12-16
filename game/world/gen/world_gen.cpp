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
#include "../tables.h"
#include "../../raws/raw_clothing.h"

using engine::entity;
using namespace engine;

inline void really_hollow(const location_t &loc) {
    tile_t * tile = world::planet->get_tile(loc);
    tile->solid = false;
    tile->base_tile_type = tile_type::EMPTY_SPACE;
}

void hollow(const location_t &loc) {
  really_hollow(loc);
  
  location_t above = { loc.region, loc.x, loc.y, loc.z-1 };
  location_t below = { loc.region, loc.x, loc.y, loc.z+1 };
  
  really_hollow(above);
  really_hollow(below);
  raws::create_structure_from_raws("Ship Superstructure", above);
  raws::create_structure_from_raws("Ship Superstructure", below);
}

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
    hollow(location_t{ planet_idx, start_x, start_y, start_z });
}

void add_solar_collector(const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t planet_idx) {
    raws::create_structure_from_raws("Solar Collector", location_t { planet_idx, x, y, z });
    //hollow(location_t { planet_idx, x, y, z-1 });
}

void add_food_replicator(const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t planet_idx) {
    raws::create_structure_from_raws("Small Replicator", location_t { planet_idx, x, y, z });
    hollow(location_t { planet_idx, x, y, z });
}

void add_storage_unit(const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t planet_idx) {
    int container_id = raws::create_structure_from_raws("Storage Unit", location_t { planet_idx, x, y, z });
    hollow(location_t { planet_idx, x, y, z });
    
    for (int i=0; i<3; ++i) {
	int tent_kit = raws::create_item_from_raws("Personal Survival Shelter Kit");
	game_engine->ecs->add_component<item_storage_component>( *game_engine->ecs->get_entity_by_handle( tent_kit ), item_storage_component(container_id) );
    }
    int fire_kit = raws::create_item_from_raws("Camping Fire Kit");
    game_engine->ecs->add_component<item_storage_component>( *game_engine->ecs->get_entity_by_handle( fire_kit ), item_storage_component(container_id) );
    int fire_axe = raws::create_item_from_raws("Fire Axe");
    game_engine->ecs->add_component<item_storage_component>( *game_engine->ecs->get_entity_by_handle( fire_axe ), item_storage_component(container_id) );
}

void add_structural_element(const location_t &loc, unsigned char glyph, bool block=true) {
  hollow(loc);
  switch (glyph) {
    case 16 : raws::create_structure_from_raws("Ship Front", loc); break;
    case 186 : raws::create_structure_from_raws("Ship Wall NS", loc); break;
    case 200 : raws::create_structure_from_raws("Ship Wall NE", loc); break;
    case 201 : raws::create_structure_from_raws("Ship Wall SE", loc); break;
    case 219 : raws::create_structure_from_raws("Ship Superstructure", loc); break;
    case 46 : raws::create_structure_from_raws("Ship Floor", loc); break;
    case 205 : raws::create_structure_from_raws("Ship Wall EW", loc); break;
    default : std::cout << "Oops - missed a structure, code " << +glyph << "\n";
  }
}

entity make_settler(const location_t &loc)
{
    entity test = game_engine->ecs->add_entity();

    game_engine->ecs->add_component(test, debug_name_component("Test"));
    game_engine->ecs->add_component(test, position_component3d(loc, OMNI));
    //engine::globals::ecs->add_component(test, obstruction_component(true,false));
    game_engine->ecs->add_component(test, renderable_component('@', yellow, black,34,1,false,true));
    game_engine->ecs->add_component(test, viewshed_component(visibility,12));
    settler_ai_component ai;
    
    std::pair<string, raws::base_raw *> profession = raws::get_random_starting_profession();    
        
    // Create some basic attributes
    game_stats_component stats;
    stats.strength = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Strength");
    stats.dexterity = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Dexterity");
    stats.constitution = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Constitution");
    stats.intelligence = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Intelligence");
    stats.wisdom = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Wisdom");
    stats.charisma = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Charisma");
    stats.comeliness = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Comeliness");
    stats.ethics = game_engine->rng.roll_dice(3,6) + profession.second->get_modifier("Ethics");
    stats.age = 16 + game_engine->rng.roll_dice(3,6);   
    
    game_species_component species;
    species.species = "Human";
    
    int gender_roll = game_engine->rng.roll_dice(1,101);
    if (gender_roll <=50) {
      species.gender = gender_t::MALE;
    } else if (gender_roll <=100) {
      species.gender = gender_t::FEMALE;
    } else {
      species.gender = gender_t::HERMAPHRODITE;
    }
    
    int preference_roll = game_engine->rng.roll_dice(1,100);
    if (preference_roll < 92) {
      species.sexual_preference = preference_t::HETEROSEXUAL;
    } else if (preference_roll < 94) {
      species.sexual_preference = preference_t::BISEXUAL;
    } else if (preference_roll < 99) {
      species.sexual_preference = preference_t::HOMOSEXUAL;
    } else {
      species.sexual_preference = preference_t::ASEXUAL;
    }
    
    float height_cm, weight_kg;
    
    if (species.gender == gender_t::MALE or species.gender == gender_t::HERMAPHRODITE) {
      height_cm = 147.0F + (game_engine->rng.roll_dice(2,10)*2.5F);
      weight_kg = 54.0F + (game_engine->rng.roll_dice(2,8)*0.45);
      ai.calories = 4400 + game_engine->rng.roll_dice(1,200);
      ai.calorie_burn_at_rest = 1 + game_engine->rng.roll_dice(1,2);
    } else {
      height_cm = 134.0F + (game_engine->rng.roll_dice(2,10)*2.5F);
      weight_kg = 38.0F + (game_engine->rng.roll_dice(2,4)*0.45);
      ai.calories = 3600 + game_engine->rng.roll_dice(1,200);
      ai.calorie_burn_at_rest =  game_engine->rng.roll_dice(1,2);
    }
    species.height_cm = height_cm;
    species.weight_kg = weight_kg;
    ai.wakefulness = 1800 + game_engine->rng.roll_dice(1,100) + (stat_modifier(stats.constitution)*3);
    ai.thirst = 960 + game_engine->rng.roll_dice(1,20);
    
    ai.first_name = raws::get_random_first_name(species.gender);
    ai.last_name = raws::get_random_last_name();
    ai.profession_tag = profession.first;
    
    game_health_component health;
    health.max_hit_points = 8 + stat_modifier(stats.constitution); // First level, they get max!
    if (health.max_hit_points < 1) health.max_hit_points = 1;
    health.current_hit_points = health.max_hit_points;
    
    ai.state_major = IDLE;
    ai.state_timer = 0;
    
    // Hair/etc. this should be made more realistic one day!
    const int ethnic_roll = game_engine->rng.roll_dice(1,4);
    switch (ethnic_roll) {
      case 1 : species.skin_color = skin_color_t::CAUCASIAN; break;
      case 2 : species.skin_color = skin_color_t::ASIAN; break;
      case 3 : species.skin_color = skin_color_t::INDIAN; break;
      case 4 : species.skin_color = skin_color_t::AFRICAN; break;
    }
    
    species.bearded = false;
    if (species.gender == gender_t::MALE) {
	const int beard_roll = game_engine->rng.roll_dice(1,20);
	if (beard_roll < 7) {
	    species.bearded = true;
	} else {
	    species.bearded = false;
	}
    }
    
    const int hair_color_roll = game_engine->rng.roll_dice(1,4);
    switch (hair_color_roll) {
      case 1 : species.hair_color = hair_color_t::BLACK; break;
      case 2 : species.hair_color = hair_color_t::BLONDE; break;
      case 3 : species.hair_color = hair_color_t::BROWN; break;
      case 4 : species.hair_color = hair_color_t::WHITE; break;      
    }
    
    species.hair_style = hair_style_t::BALD;
    if (species.gender == gender_t::MALE) {
	const int style_roll = game_engine->rng.roll_dice(1,5);
	switch (style_roll) {
	  case 1 : species.hair_style = hair_style_t::BALD; break;
	  case 2 : species.hair_style = hair_style_t::BALDING; break;
	  case 3 : species.hair_style = hair_style_t::MOHAWK; break;
	  case 4 : species.hair_style = hair_style_t::SHORT; break;
	  case 5 : species.hair_style = hair_style_t::LONG; break;
	}
    } else {
	const int style_roll = game_engine->rng.roll_dice(1,4);
	switch (style_roll) {
	  case 1 : species.hair_style = hair_style_t::SHORT; break;
	  case 2 : species.hair_style = hair_style_t::LONG; break;
	  case 3 : species.hair_style = hair_style_t::PIGTAILS; break;
	  case 4 : species.hair_style = hair_style_t::TRIANGLE; break;
	}
    }
    
    game_engine->ecs->add_component(test, ai);
    game_engine->ecs->add_component(test, stats);
    game_engine->ecs->add_component(test, health);
    game_engine->ecs->add_component(test, species);
    
    // Make clothing for the settler
    //std::cout << "Generating: " << profession.first << "\n";
    for (const std::unique_ptr<raws::base_raw> &raw : profession.second->children) {
	if (raw->type == raws::CLOTHING) {
	    raws::raw_clothing * rawc = static_cast<raws::raw_clothing *>(raw.get());
	    //std::cout << "Clothing: (" << rawc->slot << "): " << rawc->item << ", gender " << rawc->gender << "\n";
	    if ( rawc->gender == 0 or ( species.gender == gender_t::FEMALE and rawc->gender==2 ) or ( species.gender != gender_t::FEMALE and rawc->gender != 2 ) ) {
		//std::cout << " ----- " << rawc->item << " - " << rawc->slot << " - " << rawc->type << "\n";
		const int item_entity_id = raws::create_item_from_raws( rawc->item );
		int position = 0;
		
		if (rawc->slot == "Head") position = 1;
		if (rawc->slot == "Torso") position = 2;
		if (rawc->slot == "Legs") position = 3;
		if (rawc->slot == "Shoes") position = 4;
		
		item_carried_component carried( test.handle, position );
		game_engine->ecs->add_component<item_carried_component>( *game_engine->ecs->get_entity_by_handle( item_entity_id ), carried );
		
		item_component * item = game_engine->ecs->find_entity_component<item_component>( item_entity_id );
		item->clothing_slot = position;
	    }
	}
    }
    
    return test;
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
    
    // TODO: Hollow out the landing zone
    
    // Add ship hull, superstructure, doors, power, sensors, cordex
    add_cordex ( start_x, start_y, start_z, planet_idx );
    // Solar collector at (x-1,y-1), (x+1,y-1), (x-1,y+1), (x+1,y+1)
    add_solar_collector(static_cast<uint8_t>(start_x-1), static_cast<uint8_t>(start_y-1), static_cast<uint8_t>(start_z+1), planet_idx);
    add_solar_collector(static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y-1), static_cast<uint8_t>(start_z+1), planet_idx);
    add_solar_collector(static_cast<uint8_t>(start_x-1), static_cast<uint8_t>(start_y+1), static_cast<uint8_t>(start_z+1), planet_idx);
    add_solar_collector(static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y+1), static_cast<uint8_t>(start_z+1), planet_idx);
    
    // Console constructions at (x-1,y), (x+1,y), (x,y-1), (x,y+1)
    raws::create_structure_from_raws("Education Console", location_t {planet_idx, static_cast<uint8_t>(start_x-1), start_y, start_z});
    raws::create_structure_from_raws("Scanner Console", location_t {planet_idx, static_cast<uint8_t>(start_x+1), start_y, start_z});
    raws::create_structure_from_raws("Defense Console", location_t {planet_idx, start_x, static_cast<uint8_t>(start_y-1), start_z});
    raws::create_structure_from_raws("Communications Console", location_t {planet_idx, start_x, static_cast<uint8_t>(start_y+1), start_z});
    raws::create_structure_from_raws("Water Purifier", location_t {planet_idx, static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y-2), start_z} );
    raws::create_structure_from_raws("Food Dispenser", location_t {planet_idx, static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y+2), start_z} );
    hollow( location_t {planet_idx, static_cast<uint8_t>(start_x-1), start_y, start_z} );
    hollow( location_t {planet_idx, static_cast<uint8_t>(start_x+1), start_y, start_z} );
    hollow( location_t {planet_idx, start_x, static_cast<uint8_t>(start_y-1), start_z} );
    hollow( location_t {planet_idx, start_x, static_cast<uint8_t>(start_y+1), start_z} );
    hollow( location_t {planet_idx, static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y-2), start_z} );
    hollow( location_t {planet_idx, static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y+2), start_z} );
    
    // Refridgerator/Food Replicator at (x+4,y)
    add_food_replicator(start_x+4, start_y, start_z, planet_idx);
    
    // Storage unit at (x+4,y-1) and (x+4,y+1)
    add_storage_unit(start_x+4, start_y-1, start_z, planet_idx);
    add_storage_unit(start_x+4, start_y+1, start_z, planet_idx);
    
    // Escape pod structure
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-4), static_cast<uint8_t>(start_y-3), start_z}, 201); // TL Corner
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-4), static_cast<uint8_t>(start_y+3), start_z}, 200); // BL Corner
    for (int i=0; i<6; ++i) {
	if (i != 3) {
	add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-3+i), static_cast<uint8_t>(start_y-3), start_z}, 205); // Upper hull wall
	add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-3+i), static_cast<uint8_t>(start_y+3), start_z}, 205); // Lower hull wall
	}
    }
    for (int i=0; i<5; ++i) {
      if (i != 2) {
	add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-4), static_cast<uint8_t>(start_y-2+i), start_z}, 186); // Add left wall
      } else {
	// TODO: Door goes here!
      }
    }
    
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y-3), start_z}, 16); // Front
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+4), static_cast<uint8_t>(start_y-2), start_z}, 16); // Front
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+5), static_cast<uint8_t>(start_y-1), start_z}, 16); // Front
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+6), static_cast<uint8_t>(start_y), start_z}, 16); // Front
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+5), static_cast<uint8_t>(start_y+1), start_z}, 16); // Front
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+4), static_cast<uint8_t>(start_y+2), start_z}, 16); // Front
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y+3), start_z}, 16); // Front
    
    for (int i=0; i<5; ++i) {
      add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-3), static_cast<uint8_t>(start_y-2+i), start_z}, '.', false);
      add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-2), static_cast<uint8_t>(start_y-2+i), start_z}, '.', false);
      add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+2), static_cast<uint8_t>(start_y-2+i), start_z}, '.', false);
      if (i>0 and i<4) add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+3), static_cast<uint8_t>(start_y-2+i), start_z}, '.', false);
    }
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-1), static_cast<uint8_t>(start_y-2), start_z}, '.', false);
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x), static_cast<uint8_t>(start_y-2), start_z}, '.', false);
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y-2), start_z}, '.', false);
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x-1), static_cast<uint8_t>(start_y+2), start_z}, '.', false);
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x), static_cast<uint8_t>(start_y+2), start_z}, '.', false);
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y+2), start_z}, '.', false);
    add_structural_element(location_t{ planet_idx, static_cast<uint8_t>(start_x+5), static_cast<uint8_t>(start_y), start_z}, 219);
    
    // Add random settlers    
    make_settler( location_t{ planet_idx, start_x, static_cast<uint8_t>(start_y-2), start_z});
    make_settler( location_t{ planet_idx, static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y-2), start_z});
    make_settler( location_t{ planet_idx, static_cast<uint8_t>(start_x+2), static_cast<uint8_t>(start_y-2), start_z});
    make_settler( location_t{ planet_idx, static_cast<uint8_t>(start_x), static_cast<uint8_t>(start_y+2), start_z});
    make_settler( location_t{ planet_idx, static_cast<uint8_t>(start_x+1), static_cast<uint8_t>(start_y+2), start_z});
    make_settler( location_t{ planet_idx, static_cast<uint8_t>(start_x+2), static_cast<uint8_t>(start_y+2), start_z});
    
    add_camera( start_x, start_y, start_z, planet_idx );
    world::stored_power = 25;
  
    planet->save_region( planet_idx );
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
