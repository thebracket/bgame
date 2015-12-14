#include "world_gen_hydrology.hpp"
#include <algorithm>
#include <iostream>

std::unique_ptr< rain_map_t > get_rain_map ( heightmap_t* heightmap )
{
     std::unique_ptr < rain_map_t > result = std::make_unique < rain_map_t > ( NUMBER_OF_TILES_IN_THE_WORLD );

     std::fill ( result->begin(), result->end(), 100 );

     int running = 50;
     int average_height = average_heightmap_height( heightmap );
     for ( int y = 0; y < ( WORLD_HEIGHT * REGION_HEIGHT ); ++y ) {
          for ( int x = 0; x < ( WORLD_WIDTH * REGION_WIDTH ); ++x ) {
               const int idx = height_map_idx ( x, y );
               result->operator[] ( idx ) = running;

               if ( heightmap->operator[] ( idx ) > average_height and running > 11 ) {
                    running -= 5;
               }
               if ( running < 50 ) running += 5;
          }
     }

     return std::move ( result );
}

std::unique_ptr<temperature_map_t> get_temperature_map ( heightmap_t * heightmap )
{
     std::unique_ptr < temperature_map_t > result = std::make_unique < temperature_map_t > ( NUMBER_OF_TILES_IN_THE_WORLD );

     constexpr float AVERAGE_EARTH_TEMPERATURE = 15;
     constexpr float MAX_EARTH_TEMPERATURE = 58;
     constexpr float MIN_EARTH_TEMPERATURE = -89;
     constexpr float TEMPERATURE_RANGE = MAX_EARTH_TEMPERATURE - MIN_EARTH_TEMPERATURE;
     constexpr float TEMPERATURE_GRADIENT = TEMPERATURE_RANGE / ( WORLD_HEIGHT * REGION_HEIGHT );

     // Average earth temperature
     std::fill ( result->begin(), result->end(), AVERAGE_EARTH_TEMPERATURE );
     uint16_t average_height = average_heightmap_height ( heightmap );


     float temperature = MIN_EARTH_TEMPERATURE;
     for ( int y = 0; y < ( WORLD_HEIGHT * REGION_HEIGHT ); ++y ) {
          for ( int x = 0; x < ( WORLD_WIDTH * REGION_WIDTH ); ++x ) {
               const int idx = height_map_idx ( x, y );
               const uint16_t altitude = heightmap->operator[] ( idx );
               if ( altitude > average_height ) {
                    result->operator[] ( idx ) = temperature - ( ( altitude - average_height ) /100 );
               }
          }
          temperature += TEMPERATURE_GRADIENT;
     }

     return std::move ( result );
}

std::unique_ptr<water_level_map_t> perform_hydrology ( heightmap_t * heightmap, engine::random_number_generator * rng )
{
     std::unique_ptr<water_level_map_t> water_ptr = std::make_unique<water_level_map_t>();
     water_ptr->resize ( NUMBER_OF_TILES_IN_THE_WORLD );
     std::fill ( water_ptr->begin(), water_ptr->end(), 10 );

     for ( int pass = 0; pass < WORLD_WIDTH*WORLD_HEIGHT; ++pass ) {
          // Re-generate height-temperature based upon the new map
          std::unique_ptr<rain_map_t> rainfall = get_rain_map ( heightmap );
          std::unique_ptr<temperature_map_t> temperature = get_temperature_map ( heightmap );

          // Add rain-fall
          for ( int i=0; i<NUMBER_OF_TILES_IN_THE_WORLD; ++i ) {
               const uint8_t rain_probability = rainfall->operator[] ( i );
               water_ptr->operator[] ( i ) += rain_probability/10;
          }

          std::unique_ptr<rain_map_t> water_tmp = std::make_unique<rain_map_t>( );
          std::copy ( water_ptr->begin(), water_ptr->end(), std::back_inserter ( *water_tmp.get() ) );
          std::unique_ptr<heightmap_t> height_tmp = std::make_unique<heightmap_t>( );
          std::copy ( heightmap->begin(), heightmap->end(), std::back_inserter ( *height_tmp.get() ) );

          bool settled = false;
          int n_passes = 0;
          int changes = 0;
          while ( !settled ) {
               ++n_passes;
               settled = true;
               changes = 0;
               // Move water downhill
               for ( int y = 1; y < ( WORLD_HEIGHT * REGION_HEIGHT )-1; ++y ) {
                    for ( int x = 1; x < ( WORLD_WIDTH * REGION_WIDTH )-1; ++x ) {
                         const int hidx = height_map_idx ( x, y );
                         if ( water_ptr->operator[] ( hidx ) >0 ) {
                              const uint16_t altitude = heightmap->operator[] ( hidx );

                              // Determine lowest neighbouring
                              const int north_idx = height_map_idx ( x, y-1 );
                              const int south_idx = height_map_idx ( x, y+1 );
                              const int east_idx = height_map_idx ( x+1, y );
                              const int west_idx = height_map_idx ( x-1, y );

                              int destination = hidx;
                              int alt_buf = altitude;
                              if ( heightmap->operator[] ( north_idx ) < alt_buf ) {
                                   destination = north_idx;
                                   alt_buf = heightmap->operator[] ( north_idx );
                              }
                              if ( heightmap->operator[] ( south_idx ) < alt_buf ) {
                                   destination = south_idx;
                                   alt_buf = heightmap->operator[] ( south_idx );
                              }
                              if ( heightmap->operator[] ( east_idx ) < alt_buf ) {
                                   destination = east_idx;
                                   alt_buf = heightmap->operator[] ( east_idx );
                              }
                              if ( heightmap->operator[] ( west_idx ) < alt_buf ) {
                                   destination = west_idx;
                                   alt_buf = heightmap->operator[] ( west_idx );
                              }
			   
                              if ( destination != hidx ) {
                                   //std::cout << "<";
                                   // Erode altitude down (water) units
				   if ( height_tmp->operator[] ( hidx ) > 2 ) {
				      height_tmp->operator[] ( hidx ) -= 2;
				   }

                                   // Move water there
                                   if ( water_tmp->operator[] ( destination ) < 200 ) water_tmp->operator[] ( destination ) += 1;
                                   if ( water_tmp->operator[] ( destination ) > 200 ) water_tmp->operator[] ( destination ) = 200;
                                   water_tmp->operator[] ( hidx ) -= 1;
				   if (water_tmp->operator[] ( hidx ) < 1) water_tmp->operator[] ( hidx ) = 0;
                                   settled = false;
                                   ++changes;
                              }
                         }
                    }
               }

               // Copy the altitude and water maps back
               water_ptr->clear();
               std::copy ( water_tmp->begin(), water_tmp->end(), std::back_inserter ( *water_ptr ) );
               heightmap->clear();
               std::copy ( height_tmp->begin(), height_tmp->end(), std::back_inserter ( *heightmap ) );

               for ( int i=0; i<NUMBER_OF_TILES_IN_THE_WORLD; ++i ) {
                    if (water_ptr->operator[]( i ) > 5) {
		      water_ptr->operator[] ( i ) -= 5; // Evaporation
		    } else {
		      water_ptr->operator[] ( i ) = 0;
		    }
                    if ( water_ptr->operator[] ( i ) < 1 ) water_ptr->operator[] ( i ) = 0;
		    if ( water_ptr->operator[] ( i ) > 250 ) water_ptr->operator[] ( i ) = 0;
               }
               
               std::cout << "Pass " << pass << " .. sub-pass " << n_passes << " (" << changes << ") \n";
	       if (changes < 17000) settled = true;
          }
          /*for (int i=0; i<16; ++i)
	    smooth_height_map ( heightmap );
	  */

          rainfall.reset();
          temperature.reset();
          water_tmp.reset();
          height_tmp.reset();

          //smooth_height_map( heightmap );
     }

     return std::move ( water_ptr );
}