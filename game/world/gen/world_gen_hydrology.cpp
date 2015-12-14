#include "world_gen_hydrology.hpp"
#include <algorithm>
#include <iostream>
#include <map>

std::unique_ptr<heightmap_t> get_minimum_erosion( heightmap_t * heightmap ) {
    std::unique_ptr<heightmap_t> erosion_minimum = get_height_map();
     int i=0;
     for (const int &h : *heightmap) {
	erosion_minimum->operator[] ( i ) = h/2;
	++i;
     }
     return std::move ( erosion_minimum );
}

class water_sorter {
public:
  //water_sorter() { std::cout <<  "------ Start Sort\n"; }
  //~water_sorter() { std::cout << "------ End Sort\n"; }
  void add_destination(const int &dest, const int &level) {
      //std::cout << level << " .. " << dest << "\n";
      if (level < current_lowest) {
	  current_lowest = level;
	  current_lowest_direction = dest;
      }
  }
  int get() { 
    //std::cout << " = " << current_lowest_direction << "\n";
    return current_lowest_direction;     
  }
private:
  int current_lowest_direction = -1;
  int current_lowest = 60000;
};

int downhill_target ( const int &x, const int &y, const int &hidx, heightmap_t * heightmap, water_level_map_t * water ) {    
    const int north_idx = height_map_idx( x, y-1 );
    const int east_idx = height_map_idx( x+1, y );
    const int south_idx = height_map_idx( x, y+1 );
    const int west_idx = height_map_idx( x-1, y );
    
    const int northwest_idx = height_map_idx( x-1, y-1 );
    const int southwest_idx = height_map_idx( x-1, y+1 );
    const int northeast_idx = height_map_idx( x+1, y-1 );
    const int southeast_idx = height_map_idx( x+1, y+1 );
    
    water_sorter sorter;
    sorter.add_destination( hidx, heightmap->operator[]( hidx ) );
    sorter.add_destination( north_idx, heightmap->operator[]( north_idx ) );
    sorter.add_destination( east_idx, heightmap->operator[]( east_idx ) );
    sorter.add_destination( south_idx, heightmap->operator[]( south_idx ) );
    sorter.add_destination( west_idx, heightmap->operator[]( west_idx ) );
    sorter.add_destination( northwest_idx, heightmap->operator[]( northwest_idx ) );
    sorter.add_destination( southwest_idx, heightmap->operator[]( southwest_idx ) );
    sorter.add_destination( northeast_idx, heightmap->operator[]( northeast_idx ) );
    sorter.add_destination( southeast_idx, heightmap->operator[]( southeast_idx ) );
        
    const int result = sorter.get();
    return result;
}

std::unique_ptr<water_level_map_t> perform_hydrology ( heightmap_t * heightmap, engine::random_number_generator * rng )
{
    constexpr int width = WORLD_WIDTH * REGION_WIDTH;
    constexpr int height = WORLD_HEIGHT * REGION_HEIGHT;
  
     // We never erode below half original height
     auto min_heights = get_minimum_erosion( heightmap );
     std::unique_ptr<water_level_map_t> water_ptr = std::make_unique<water_level_map_t>();
     water_ptr->resize ( NUMBER_OF_TILES_IN_THE_WORLD );
     
     for (int pass=0; pass<1; ++pass) {
	std::fill ( water_ptr->begin(), water_ptr->end(), 200 ); // TODO: Replace with a rainfall map
	
	bool stable = false;
	
	int pass_count = 0;
	int changes = 0;
	int dry = 0;
	int stuck = 0;
	while (!stable) {
	    stable = true;
	    auto heightmap_copy = copy_height_map( heightmap );
	    auto water_copy = std::make_unique<water_level_map_t>();
	    for (const uint8_t &w : *water_ptr) water_copy->push_back ( w );
	    for (int y=1; y<height-1; ++y) {
	      for (int x=1; x<width-1; ++x) {
		
		const int hidx = height_map_idx( x, y );
		if ( water_ptr->operator[](hidx) > 0 ) {
		  // Tile is wet - can the water move?
		  const int destination_idx = downhill_target( x, y, hidx, heightmap, water_ptr.get() );
		  if (destination_idx == hidx or destination_idx==-1) {
		      // There's nowhere to go from here.
		      if ( heightmap_copy->operator[] ( hidx ) > min_heights->operator[] ( hidx ) ) {
			heightmap_copy->operator[] ( hidx ) = heightmap_copy->operator[] ( hidx ) - 1;
		      }
		      ++stuck;
		  } else {
		    // Erode downwards
		    if ( heightmap_copy->operator[] ( hidx ) > min_heights->operator[] ( hidx )+1 ) {
			heightmap_copy->operator[] ( hidx ) = heightmap_copy->operator[] ( hidx )-1;
			stable = false;
			++changes;
		    }
		    
		    if (water_copy->operator[] ( destination_idx ) < 253)
			water_copy->operator[] ( destination_idx ) = water_copy->operator[] ( destination_idx ) + 1;
		    
		    if ( water_copy->operator[] ( hidx ) > 1 )
			water_copy->operator[] ( hidx ) = water_copy->operator[] ( hidx ) -1;					


		  }
		} else { ++dry; } // if
	      } // x
	    } // y
	    
	    heightmap->clear();
	    water_ptr->clear();
		    
	    heightmap->clear();
	    std::copy ( heightmap_copy->begin(), heightmap_copy->end(), std::back_inserter ( *heightmap ) );
	    water_ptr->clear();
	    for ( uint8_t w : *water_copy) {
	      if (w > 0) --w;
	      water_ptr->push_back(w);
	    }
	    
	    ++pass_count;
	    std::cout << "Pass " << pass << "~" << pass_count << " (changes: " << changes << ", dry: " << dry << ", unmoving: " << stuck << ") \n";
	    changes = 0;
	    dry = 0;
	}
     }     
     return std::move ( water_ptr );
}