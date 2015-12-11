#include "world_gen_fault_lines.hpp"
#include "world_height_map_marching_squares.hpp"
#include <utility>
#include <vector>
#include <iostream>
#include "../../engine/png_writer.h"

using std::pair;
using std::make_pair;
using std::vector;
using std::unique_ptr;

void add_fault_lines ( heightmap_t* heightmap, engine::random_number_generator * rng )
{
    unique_ptr<heightmap_t> vmap_p = get_height_map();
    
    std::cout << "Making points list.\n";
    vector<pair<uint16_t,uint16_t>> points;
    vector<int> point_heights;
    for (int i=0; i<254; ++i) {
	points.push_back ( make_pair( rng->roll_dice( 1, (WORLD_WIDTH*REGION_WIDTH)-1 ), rng->roll_dice( 1, (WORLD_HEIGHT*REGION_HEIGHT)-1 ) ) );
	point_heights.push_back( rng->roll_dice(4,64) );
    }
    
    png_writer png("world/voronoi.png", WORLD_WIDTH*REGION_WIDTH, WORLD_HEIGHT*REGION_HEIGHT);
    std::cout << "Spreading regions.\n";
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
	}
    }
    png.save();
    
    /*
    // Then marching squares to find the edge cells; these are raised as fault lines.
    std::cout << "Elevating edges.\n";
    for ( int i=0; i<points.size(); ++i ) {
      std::unique_ptr< marching_squares_map_t > edges = marching_squares( vmap_p.get(), [i] (const int n) {
	if ( n == i ) {
	    return true;
	} else {
	    return false;
	}
      } );
      for ( int j=0; j<NUMBER_OF_TILES_IN_THE_WORLD; ++j ) {
	  if ( edges->operator[](j)>0 and edges->operator[](j) < 15 ) {
	    uint16_t h = heightmap->operator[] ( j );
	    heightmap->operator[] ( j ) = h + 1000;
	  }
      }
    };*/
    
    // Rather than elevating edges, we're going to set altitude by mask entry. This should result in more flatter areas.
    for ( int i=0; i<heightmap->size(); ++i ) {
	heightmap->operator[]( i ) = point_heights[vmap_p->operator[] ( i )] + rng->roll_dice(3,6);
    }
}
