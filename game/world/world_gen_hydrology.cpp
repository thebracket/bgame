#include "world_gen_hydrology.hpp"
#include <algorithm>

std::unique_ptr< rain_map_t > get_rain_map ( heightmap_t* heightmap )
{
    std::unique_ptr < rain_map_t > result_ptr = std::make_unique < rain_map_t > ( NUMBER_OF_TILES_IN_THE_WORLD );
    rain_map_t result = *result_ptr.get();
    
    std::fill ( result.begin(), result.end(), 100 );
    const int average_height = average_heightmap_height( heightmap );
    
    uint8_t running = 100;
    for ( int y = 0; y < (WORLD_HEIGHT * REGION_HEIGHT); ++y) {
	for ( int x = 0; x < ( WORLD_WIDTH * REGION_WIDTH); ++x) {
	    const int idx = height_map_idx( x, y );
	    result [ idx ] = running;
	    
	    if ( heightmap->operator[](idx) > average_height and running > 11 ) {
		running -= 10;
	    }
	    if (running < 100) running += 1;
	}
    }
    
    return std::move ( result_ptr );
}

std::unique_ptr<temperature_map_t> get_temperature_map ( heightmap_t * heightmap ) {
    std::unique_ptr < temperature_map_t > result_ptr = std::make_unique < temperature_map_t > ( NUMBER_OF_TILES_IN_THE_WORLD );
    temperature_map_t result = *result_ptr.get();
    
    constexpr float AVERAGE_EARTH_TEMPERATURE = 15;
    constexpr float MAX_EARTH_TEMPERATURE = 58;
    constexpr float MIN_EARTH_TEMPERATURE = -89;
    constexpr float TEMPERATURE_RANGE = MAX_EARTH_TEMPERATURE - MIN_EARTH_TEMPERATURE;
    constexpr float TEMPERATURE_GRADIENT = TEMPERATURE_RANGE / ( WORLD_HEIGHT * REGION_HEIGHT );
    
    // Average earth temperature
    std::fill ( result.begin(), result.end(), AVERAGE_EARTH_TEMPERATURE );
    uint16_t average_height = average_heightmap_height( heightmap );
    
    
    float temperature = MIN_EARTH_TEMPERATURE;
    for ( int y = 0; y < (WORLD_HEIGHT * REGION_HEIGHT); ++y) {
	for ( int x = 0; x < ( WORLD_WIDTH * REGION_WIDTH); ++x) {
	    const int idx = height_map_idx( x, y );
	    const uint16_t altitude = heightmap->operator[] ( idx );
	    if ( altitude > average_height ) {
		result [ idx ] = temperature - (( altitude - average_height  )/100);
	    }
	}
	temperature += TEMPERATURE_GRADIENT;
    }
    
    return std::move ( result_ptr );
}

std::unique_ptr<water_level_map_t> perform_hydrology ( heightmap_t * heightmap, engine::random_number_generator * rng ) {
    std::unique_ptr<water_level_map_t> water_ptr = std::make_unique<water_level_map_t>(NUMBER_OF_TILES_IN_THE_WORLD);
    water_level_map_t water = *water_ptr.get();
    std::fill ( water.begin(), water.end(), 10 );
    
    for (int pass = 0; pass < 100; ++pass ) {
	std::unique_ptr<rain_map_t> rainfall = get_rain_map( heightmap );
	std::unique_ptr<temperature_map_t> temperature = get_temperature_map( heightmap );
	
	// Add rain-fall
	for ( int i=0; i<NUMBER_OF_TILES_IN_THE_WORLD; ++i ) {
	    const uint8_t rain_probability = rainfall->operator[] ( i );
	    const int rain_roll = rng->roll_dice(1, 110);
	    if (rain_roll < rain_probability) water[ i ] += 10;
	}
	
	std::unique_ptr<rain_map_t> water_tmp;
	std::copy ( water.begin(), water.end(), std::back_inserter ( *water_tmp ) );
	std::unique_ptr<heightmap_t> height_tmp;
	std::copy ( heightmap->begin(), heightmap->end(), std::back_inserter( *height_tmp.get() ) );
	
	// Move water downhill
	for ( int y = 1; y < (WORLD_HEIGHT * REGION_HEIGHT)-1; ++y) {
	    for ( int x = 1; x < ( WORLD_WIDTH * REGION_WIDTH)-1; ++x) {
		const int hidx = height_map_idx( x, y );
		if ( water[hidx]>0 ) {
		    const uint16_t altitude = heightmap->operator[] ( hidx );
		    
		    // Determine lowest neighbouring 
		    const int north_idx = height_map_idx( x, y-1 );
		    const int south_idx = height_map_idx( x, y+1 );
		    const int east_idx = height_map_idx( x+1, y );
		    const int west_idx = height_map_idx( x-1, y );
		    
		    int destination = hidx;
		    int alt_buf = altitude;
		    if ( heightmap->operator[]( north_idx ) < alt_buf ) { destination = north_idx; alt_buf = heightmap->operator[]( north_idx ); }
		    if ( heightmap->operator[]( south_idx ) < alt_buf ) { destination = south_idx; alt_buf = heightmap->operator[]( south_idx ); }
		    if ( heightmap->operator[]( east_idx ) < alt_buf ) { destination = east_idx; alt_buf = heightmap->operator[]( east_idx ); }
		    if ( heightmap->operator[]( west_idx ) < alt_buf ) { destination = west_idx; alt_buf = heightmap->operator[]( west_idx ); }
		    
		    if (destination != hidx) {
			// Erode altitude down (water) units
			height_tmp->operator[]( hidx ) -= water [ hidx ];
		      
			// Move water there
			water_tmp->operator[]( destination ) += water[hidx];
			if ( water_tmp->operator[] ( destination) > 100 ) water_tmp->operator[]( destination ) = 100;
			water[hidx] = 0;
		    }
		}
	    }
	}
	
	// Copy the altitude and water maps back
	std::copy ( water_tmp->begin(), water_tmp->end(), std::back_inserter( water ) );
	std::copy ( height_tmp->begin(), height_tmp->end(), std::back_inserter( *heightmap ) );
    }
    
    return std::move ( water_ptr );
}