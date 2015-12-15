#include "world_gen_layer_cake.hpp"
#include "../planet.hpp"
#include "world_height_map_marching_squares.hpp"
#include <iostream>
#include "../../../engine/png_writer.h"
#include "tile_builder.hpp"

std::unique_ptr< planet_t > make_world_layers ( heightmap_t* base_map, engine::random_number_generator& rng, water_level_map_t * water, biome_map_t * biomes )
{    
    const int lowest_ground = min_heightmap_height( base_map );
    const int highest_ground = max_heightmap_height( base_map );
    const int n_bands = 20;
    const int range = highest_ground - lowest_ground;
    const float step = range / n_bands;
    
    std::cout << "Lowest: " << lowest_ground << ", highest: " << highest_ground << ", range: " << range << ", step: " << step << "\n";
    
    std::vector<int> bands ( NUMBER_OF_TILES_IN_THE_WORLD );
    for (int i=0; i<NUMBER_OF_TILES_IN_THE_WORLD; ++i) {
	const int altitude_at_point = base_map->operator[] ( i );
	bands[i] = ((altitude_at_point - lowest_ground) / step) + 32;
    }
    
    // Ramp detection
    std::vector<bool> ramps;
    ramps.resize( NUMBER_OF_TILES_IN_THE_WORLD );
    std::fill ( ramps.begin(), ramps.end(), false );
    
    for (int i=32; i<n_bands+32; ++i) {
	std::unique_ptr<marching_squares_map_t> ramps_tmp = marching_squares( &bands, [i] (int n) {
	  bool result = false;
	  if ( n > i ) result = true;
	  return result;
	} );
	int idx = 0;
	for (const uint8_t &tmp : *ramps_tmp) {
	    if (tmp==12 or tmp==6) {
	      ramps[idx] = true;
	    }
	    if (tmp==9) ramps[idx+1]=true;
	    if (tmp==3) ramps[idx+(WORLD_WIDTH*REGION_WIDTH)]=true;
	    ++idx;
	}
    }
    
    // Find the water line
    const int water_line = find_flood_level( base_map, 0.33 );
    const int water_z = ((water_line - lowest_ground) / step) + 32;
    std::cout << "Water Z:" << water_z << "\n";
    
    std::unique_ptr < planet_t > planet = std::make_unique < planet_t > ();
    
    // Save a PNG
    png_writer png("world/heights.png", (WORLD_WIDTH*REGION_WIDTH)+1, (WORLD_HEIGHT*REGION_HEIGHT)+1);
    
    // Make regions and cells
    for (int wy=0; wy<WORLD_HEIGHT; ++wy) {
	for (int wx=0; wx<WORLD_WIDTH; ++wx) {
	    const uint8_t region_index = planet->planet_idx ( wx, wy );
	    region_t * region_ptr = planet->create_region ( region_index );
	    
	    for (uint8_t y=0; y<REGION_HEIGHT; ++y) {
		for (uint8_t x=0; x<REGION_WIDTH; ++x) {
		    for (uint8_t Z=0; Z<REGION_DEPTH; ++Z) {
			tile_t * tile = planet->get_tile( location_t{ region_index, x, y, Z } );
			tile->solid = false;
			tile->base_tile_type = tile_type::EMPTY_SPACE;
			tile->ground = tile_ground::IGNEOUS;
			tile->climate = tile_climate::TEMPERATE;
			tile->covering = tile_covering::BARE;
			tile->water_level = 0;
		    }
		  
		    const int hidx = height_map_idx( (REGION_WIDTH * wx) + x, (REGION_HEIGHT * wy) + y );
		  
		    // Build upwards. Bottom level has to be solid. Then lava for min 3, biased by altitude. Then rock. Then water, if > 10.
		    tile_t * bottom = planet->get_tile( location_t{ region_index, x, y, 0 } );
		    bottom->solid = true;
		    bottom->base_tile_type = tile_type::FLAT;
		    bottom->ground = tile_ground::IGNEOUS;
		    bottom->covering = tile_covering::BARE;
		    bottom->climate = tile_climate::VERY_HOT;
		    
		    // Add magma
		    const int n_magma = 3 + bands [ hidx ] - 32;
		    uint8_t z = 1;
		    while ( z < n_magma+1) {
			tile_t * target = planet->get_tile( location_t{region_index, x, y, z} );
			  target->solid = true;
			  target->base_tile_type = tile_type::MAGMA;
			  target->covering = tile_covering::BARE;
			  target->climate = tile_climate::MOLTEN;
			++z;
		    }
		    
		    bottom = planet->get_tile( location_t{region_index, x, y, z} );
		    bottom->solid = true;
		    bottom->base_tile_type = tile_type::FLAT;
		    bottom->ground = tile_ground::IGNEOUS;
		    bottom->covering = tile_covering::BARE;
		    bottom->climate = tile_climate::VERY_HOT;
		    ++z;
		    
		    // Add solid rock
		    const int n_layers = rng.roll_dice(1, 6);
		    while ( z < bands[ hidx ]-n_layers ) {
			tile_t * target = planet->get_tile( location_t{region_index, x, y, z} );
			target->solid = true;
			target->base_tile_type = tile_type::FLAT;
			target->ground = tile_ground::IGNEOUS;
			target->covering = tile_covering::BARE;
			target->climate = tile_climate::SUBTERRANEAN;
			++z;
		    }
		    
		    while ( z < bands[ hidx ]-1 ) {
			tile_t * target = planet->get_tile( location_t{region_index, x, y, z} );
			target->solid = true;
			target->base_tile_type = tile_type::FLAT;
			target->ground = tile_ground::SEDIMENTARY;
			target->covering = tile_covering::BARE;
			target->climate = tile_climate::SUBTERRANEAN;
			++z;
		    }
		    
		    // The surface
		    tile_t * target = planet->get_tile( location_t{region_index, x, y, z} );
		    target->solid = false;
		    target->base_tile_type = tile_type::FLAT;
		    if ( ramps[ hidx ] == true ) target->base_tile_type = tile_type::RAMP;
		    target->ground = tile_ground::SEDIMENTARY;
		    target->covering = tile_covering::GRASS;
		    target->climate = tile_climate::TEMPERATE;
		    if (z >= water_z) png.setPixel( (wx*REGION_WIDTH)+x, (wy*REGION_HEIGHT)+y, 0, (z-128)*10, 0, 255 );
		    
		    if ( z >= water_z) {
			biome_t biome = biomes->biomes[biomes->biome_map->operator[]( hidx )];
			build_tile( biome, target );
		    }
		    
		    // Just add water
		    while ( z < water_z ) {
			target = planet->get_tile( location_t{region_index, x, y, z} );
			target->solid = false;
			if ( target->base_tile_type != tile_type::RAMP ) target->base_tile_type = tile_type::FLAT;
			target->covering = tile_covering::BARE;
			target->climate = tile_climate::TEMPERATE;
			target->water_level = 10;
			png.setPixel( (wx*REGION_WIDTH)+x, (wy*REGION_HEIGHT)+y, 0, 0, 255, 255 );
			++z;
		    }
		    
		    //std::cout << +z << "\n";
		}
	    }
	    
	    std::cout << "Saving region: " << wx << "x" << wy << "\n";
	    planet->save_region ( region_index );
	    planet->free_region ( region_index );
	}
    }
    std::cout << "Saving PNG\n";
    png.save();
    return std::move ( planet );
}
