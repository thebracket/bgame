#include "world_height_map_marching_squares.hpp"

using std::vector;

std::unique_ptr< marching_squares_map_t > marching_squares ( const heightmap_t* height_map, std::function< bool(int) > comparator )
{
    std::unique_ptr<vector<bool>> threshold = std::make_unique<vector<bool>>( NUMBER_OF_TILES_IN_THE_WORLD );
    int idx = 0;
    for ( const int &altitude : *height_map ) {
	threshold->operator[] ( idx ) = comparator(altitude);
	++idx;
    }
    
    std::unique_ptr < marching_squares_map_t > result_ptr = std::make_unique<marching_squares_map_t>( NUMBER_OF_TILES_IN_THE_WORLD );
    
    for ( int y = 0; y < ( WORLD_HEIGHT * REGION_HEIGHT )-1; ++y ) {
	for (int x = 0; x < ( WORLD_WIDTH * REGION_WIDTH )-1; ++x ) {
	    const int TL = height_map_idx ( x, y );
	    const int TR = height_map_idx ( x+1, y );
	    const int BR = height_map_idx ( x+1, y+1 );
	    const int BL = height_map_idx ( x, y+1 );
	    
	    uint8_t value = 0;
	    if (threshold->operator[](TL)) value += 1;
	    if (threshold->operator[](TR)) value += 2;
	    if (threshold->operator[](BR)) value += 4;
	    if (threshold->operator[](BL)) value += 8;
	    
	    result_ptr->operator[](TL) = value;
	}
    }
    
    return std::move(result_ptr);
}
