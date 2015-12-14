#include "world_gen_biomes.hpp"
#include "world_height_map_marching_squares.hpp"
#include <utility>
#include <vector>
#include <iostream>
#include "../../../engine/png_writer.h"

using std::pair;
using std::make_pair;
using std::vector;
using std::unique_ptr;

biome_map_t make_biome_map ( heightmap_t* heightmap, engine::random_number_generator* rng, water_level_map_t* water )
{
    unique_ptr<heightmap_t> vmap_p = get_height_map();
    const int number_of_biomes = WORLD_HEIGHT*WORLD_WIDTH*8;
    biome_map_t result;

    
    std::cout << "Making points list.\n";
    vector<pair<uint16_t,uint16_t>> points;
    for (int i=0; i<number_of_biomes; ++i) {
	const int x = rng->roll_dice( 1, (WORLD_WIDTH*REGION_WIDTH)-1 );
	const int y = rng->roll_dice( 1, (WORLD_HEIGHT*REGION_HEIGHT)-1 );
	points.push_back ( make_pair( x, y ) );
	result.biomes.push_back( biome_t() );
    }
    
    png_writer png("world/voronoi.png", WORLD_WIDTH*REGION_WIDTH, WORLD_HEIGHT*REGION_HEIGHT);
    std::cout << "Building voronoi diagram.\n";
    // Iterate through and mark cell membership by nearest neighbor (voronoi)
    for (int y=0; y<WORLD_HEIGHT*REGION_HEIGHT; ++y) {
	for (int x = 0; x<WORLD_WIDTH*REGION_WIDTH; ++x) {
	    int distance = 1000000;
	    int closest_point = -1;
	    
	    for (int j=0; j<points.size(); ++j) {
		const int dx = std::abs(x - points[j].first);
		const int dy = std::abs(y - points[j].second);
		const int dist = (dx*dx)+(dy*dy);
		if (dist < distance) {
		    distance = dist;
		    closest_point = j;
		}
	    }
	    vmap_p->operator[]( height_map_idx( x, y ) ) = closest_point;
	    png.setPixel( x, y, closest_point, 255-closest_point, 0, 255 );
	    result.biomes [ closest_point ].points.push_back ( make_pair( x, y ) );
	}
    }
    png.save();
    
    std::cout << "Building biome types\n";
    const int max_altitude = max_heightmap_height( heightmap );
    const int min_altitude = min_heightmap_height( heightmap );
    const int average_altitude = average_heightmap_height( heightmap );
    const int start_x = ( WORLD_WIDTH * REGION_WIDTH ) / 2;
    const int start_y = ( WORLD_HEIGHT * REGION_HEIGHT ) - REGION_HEIGHT/2;
    
    for (int i=0; i<number_of_biomes; ++i) {
	int TOTAL_ALT = 0;
	double TOTAL_WET = 0;
	int COUNT = 0;
	for ( const std::pair<int,int> &point : result.biomes[i].points) {
	          TOTAL_ALT += heightmap->operator[] ( height_map_idx( point.first, point.second ) );
		  TOTAL_WET += water->operator[] ( height_map_idx( point.first, point.second ) );
	    ++COUNT;
	}
	const int BIOME_AVERAGE_ALTITUDE = TOTAL_ALT/COUNT;
	const double AVERAGE_WETNESS = TOTAL_WET / COUNT;
	const int dx = std::abs ( points[i].first - start_x );
	const int dy = std::abs ( points[i].second - start_y );
	const double distance_from_start = std::sqrt( (dx*dx) + (dy*dy) );
	// Determine climate
	// Determine distance from starting point (difficulty)
	
	std::cout << i << " ... Altitude Average: " << BIOME_AVERAGE_ALTITUDE << "/" << average_altitude << ". Wetness: " << AVERAGE_WETNESS << ". Distance: " << distance_from_start << "\n";
    }
    
    result.biome_map = std::move( vmap_p );
    
    return result;
}
