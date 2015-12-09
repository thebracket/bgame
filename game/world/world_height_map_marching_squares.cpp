#include "world_height_map_marching_squares.hpp"

using std::vector;

std::unique_ptr< marching_squares_map_t > marching_squares ( const heightmap_t* height_map, std::function< bool(int) > comparator )
{
    std::unique_ptr<vector<bool>> threshold_ptr = std::make_unique<vector<bool>>( NUMBER_OF_TILES_IN_THE_WORLD );
    vector<bool> threshold = *threshold_ptr.get();
    heightmap_t heightmap = *height_map;
    int idx = 0;
    for ( const int &altitude : heightmap ) {
	threshold [ idx ] = comparator(altitude);
	++idx;
    }
    
    std::unique_ptr < marching_squares_map_t > result_ptr = std::make_unique<marching_squares_map_t>( NUMBER_OF_TILES_IN_THE_WORLD );
    marching_squares_map_t result = *result_ptr.get();
    
    for ( int y = 0; y < ( WORLD_HEIGHT * REGION_HEIGHT )-1; ++y ) {
	for (int x = 0; x < ( WORLD_WIDTH * REGION_WIDTH )-1; ++x ) {
	    const int TL = height_map_idx ( x, y );
	    const int TR = height_map_idx ( x+1, y );
	    const int BR = height_map_idx ( x+1, y+1 );
	    const int BL = height_map_idx ( x, y+1 );
	    
	    uint8_t value = 0;
	    if (threshold[TL]) value += 1;
	    if (threshold[TR]) value += 2;
	    if (threshold[BR]) value += 4;
	    if (threshold[BL]) value += 8;
	    
	    result [ TL ] = value;
	}
    }
    
    return std::move(result_ptr);
}
