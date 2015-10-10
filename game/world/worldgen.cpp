#include "worldgen.h"

#include "world.h"
#include "landblock.h"
#include "geometry.h"
#include <vector>
#include <algorithm>
#include <memory>
#include "../../engine/rng.h"

using std::vector;

namespace worldgen {

constexpr int worldgen_size = ( world::world_height ) * ( world::world_width ) * ( landblock_height * landblock_width );
typedef vector<short> height_map_t;

int idx ( const int x, const int y )
{
     return ( y * world::world_height ) + x;
}

unique_ptr<height_map_t> make_altitude_map()
{
     unique_ptr<height_map_t> altitude_map = std::make_unique<height_map_t>();
     altitude_map->resize ( worldgen_size );
     std::fill ( altitude_map->begin(), altitude_map->end(), 0 );
     return std::move ( altitude_map );
}

void smooth_altitude_map ( height_map_t &altitude_map )
{
     unique_ptr<height_map_t> new_map_p = make_altitude_map();
     height_map_t new_map = *new_map_p;

     for ( int y=1; y<world::world_height-1; ++y ) {
          for ( int x=1; x<world::world_width-1; ++x ) {
               const int sum = altitude_map[idx ( x-1, y-1 )] +
                               altitude_map[idx ( x, y-1 )] +
                               altitude_map[idx ( x+1, y-1 )] +
                               altitude_map[idx ( x-1, y )] +
                               altitude_map[idx ( x, y )] +
                               altitude_map[idx ( x+1, y )] +
                               altitude_map[idx ( x-1, y+1 )] +
                               altitude_map[idx ( x, y+1 )] +
                               altitude_map[idx ( x+1, y+1 )];
               const short average = sum / 9;
               new_map[idx ( x,y )] = average;
          }
     }

     for ( auto i=0; i<altitude_map.size(); ++i ) {
          altitude_map[i] = new_map[i];
     }
}

void apply_fault_line_to_altitude_map ( height_map_t &altitude_map, const int start_x,
                                        const int start_y, const int end_x, const int end_y,
                                        const int adjustment )
{
     geometry::line_func ( start_x, start_y, end_x, end_y, [&altitude_map,adjustment] ( int tx, int ty ) {
          const int index = idx ( tx,ty );
          altitude_map[index] += adjustment;
     } );
}

void convert_altitudes_to_tiles ( height_map_t &altitude_map )
{
     for ( int wy=0; wy<world::world_height; ++wy ) {
          for ( int wx=0; wx<world::world_width; ++wx ) {
               // Create the world tile object
	       land_block region;
	       region.index = world::world_idx(wx,wy);
	       const int region_x = wx*landblock_width;
	       const int region_y = wy*landblock_height;
	    
               // Populate it from the height map
	       for (int y=0; y<landblock_height; ++y) {
		for (int x=0; x<landblock_width; ++x) {
		  const int amp_x = region_x + x;
		  const int amp_y = region_y + y;
		  // TODO: Do something with altitude_map[idx(amp_x, amp_y)]
		  region.tiles[region.idx(x,y)].base_tile_type = flat;
		}
	       }
	       
               // Serialize it to disk
               region.save();
          }
     }
}

void create_heightmap_world()
{
     unique_ptr<height_map_t> altitude_map = make_altitude_map();
     const int num_fault_lines = 100 + engine::roll_dice ( 10,10 );
     for ( int i=0; i<num_fault_lines; ++i ) {
          apply_fault_line_to_altitude_map ( *altitude_map,
                                             engine::roll_dice ( 1,world::world_width-1 ),
                                             engine::roll_dice ( 1,world::world_height ),
                                             engine::roll_dice ( 1,world::world_width-1 ),
                                             engine::roll_dice ( 1,world::world_height ),
                                             100 - engine::roll_dice ( 1,50 ) + engine::roll_dice ( 1,50 )
                                           );
     }
     for ( int i=0; i<16; ++i ) {
          smooth_altitude_map ( *altitude_map );
     }
     convert_altitudes_to_tiles ( *altitude_map );
}

void build_world()
{
     create_heightmap_world();
}

}
