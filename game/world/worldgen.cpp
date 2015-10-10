#include "worldgen.h"

#include "world.h"
#include "landblock.h"
#include "geometry.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <tuple>
#include "../../engine/rng.h"

using std::vector;
using std::map;

namespace worldgen {

constexpr int worldgen_width = world::world_width*landblock_width;
constexpr int worldgen_height = world::world_height*landblock_height;
constexpr int worldgen_size = worldgen_height * worldgen_width;
typedef vector<short> height_map_t;

/* Index of the world grid. Constexpr shouldn't help here, but for some reason in
 * measured performance, it did. */
constexpr int idx ( const int x, const int y )
{
     return ( y * worldgen_width ) + x;
}

/*
 * Creates a height-map vector, sized to hold the world, and initialized with 1.
 */ 
height_map_t make_altitude_map()
{
     height_map_t altitude_map;
     altitude_map.resize ( worldgen_size );
     std::fill ( altitude_map.begin(), altitude_map.end(), 1 );
     return altitude_map;
}

/*
 * Performs a smoothing run on altitude_map. Each cell becomes
 * the average of its neighbors height. To avoid bias, the
 * smoothing copies into a new vector, which is then copied
 * back.
 */
void smooth_altitude_map ( height_map_t &altitude_map )
{
     height_map_t new_map;
     new_map.resize(worldgen_size);
  
     for ( int y=1; y<worldgen_height-1; ++y ) {
          for ( int x=1; x<worldgen_width-1; ++x ) {
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
     
     altitude_map.clear();
     std::copy(new_map.begin(), new_map.end(), std::back_inserter(altitude_map));
}

/*
 * Simulates a fault-line (more realistic than just adding blobs). Given a
 * starting and ending point, it iterates a line between the two and adds
 * (adjustment) to the point's height.
 */
void apply_fault_line_to_altitude_map ( height_map_t &altitude_map, const int start_x,
                                        const int start_y, const int end_x, const int end_y,
                                        const int adjustment )
{
     geometry::line_func ( start_x, start_y, end_x, end_y, [&altitude_map,adjustment] ( int tx, int ty ) {
          const int index = idx ( tx,ty );
	  const int old_altitude = altitude_map[index];
	  if ((old_altitude + adjustment) < 32000 and (old_altitude + adjustment) > -32000)
	      altitude_map[index] += adjustment;
     } );
}

/*
 * Iterates the whole height map, and returns the minimum and maximum heights. A
 * frequency map is populated, containing how frequently each height occurs.
 * Returned as a tuple.
 */
std::tuple<short,short,map<short,int>> min_max_heights ( const height_map_t & altitude_map )
{
     short min_height = 32767;
     short max_height = -32768;
     map<short,int> frequency_map;
     for ( const short &h : altitude_map ) {
          if ( h < min_height ) min_height = h;
          if ( h > max_height ) max_height = h;
	  
          auto finder = frequency_map.find ( h );
          if ( finder == frequency_map.end() ) {
               frequency_map[h] = 1;
          } else {
               int n = finder->second;
	       ++n;
	       frequency_map[h] = n;	    
	  }
     }
     
     return std::make_tuple ( min_height, max_height, frequency_map );
}

/*
 * Used to store the height levels at which each type of terrain
 * occurs.
 */
struct altitude_map_levels {
     short water_level;
     short plains_level;
     short hills_level;
     short mountains_level;
};

/*
 * Utility function; iterates through a frequency map until it has reached a level in which
 * the sum of previous levels equals (or exceeds) target. This is used to specify, e.g. "1/3
 * of the map should be water" - send in worldgen_size/3 as the target, and it will find
 * the altitude that satisfies this.
 */
short find_layer_level ( const map<short,int> &levels, const int &target, const short &min_layer )
{
     int total = 0;
     short level = min_layer;
     while ( total < target ) {
	  auto finder = levels.find(level);
	  if (finder != levels.end()) {
	      total += finder->second;
	  }
          ++level;
     }
     return level;
}

/*
 * Determine the altitudes at which water, plains, hills and mountains are found.
 */
altitude_map_levels determine_levels ( const std::tuple<short,short,map<short,int>> &min_max_freq )
{
     altitude_map_levels result;

     // The goal is to have 1/3rd of the map be water.
     const int one_third_total = worldgen_size / 3;
     result.water_level = find_layer_level ( std::get<2> ( min_max_freq ), one_third_total, std::get<0> ( min_max_freq ) );

     // The second third should be flat terrain
     const int two_third_total = one_third_total*2;
     result.plains_level = find_layer_level ( std::get<2> ( min_max_freq ), two_third_total, std::get<0> ( min_max_freq ) );

     // The next third is half hills, half mountains
     const int one_sixth_total = worldgen_size / 6;
     const int hills_target = two_third_total + one_sixth_total;
     result.hills_level = find_layer_level ( std::get<2> ( min_max_freq ), hills_target, std::get<0> ( min_max_freq ) );
     result.mountains_level = std::get<1> ( min_max_freq ) +1;
     
     return result;
}

/*
 * Convert the basic height map into tile types on a simple altitude basis.
 * Performed per landblock.
 */
void create_base_tile_types(const int region_x, const int region_y, land_block &region,
      const height_map_t &altitude_map, const altitude_map_levels &levels) {
      for ( int y=0; y<landblock_height; ++y ) {
	for ( int x=0; x<landblock_width; ++x ) {
	      const int amp_x = region_x + x;
	      const int amp_y = region_y + y;
	      const short altitude = altitude_map[idx ( amp_x, amp_y )];
	      const int tile_idx = region.idx(x,y);
	      if ( altitude < levels.water_level ) {
		  region.tiles[tile_idx].base_tile_type = water;
	      } else if ( altitude < levels.plains_level ) {
		  region.tiles[tile_idx].base_tile_type = flat;
	      } else if ( altitude < levels.hills_level ) {
		  region.tiles[tile_idx].base_tile_type = hill;
	      } else {
		  region.tiles[tile_idx].base_tile_type = mountain;
	      }
	      region.tiles[tile_idx].altitude = altitude;
	}
    }
}

/*
 * Converts the entire world altitude map into landblocks, and
 * saves them to disk.
 */
void convert_altitudes_to_tiles ( height_map_t &altitude_map )
{
     const std::tuple<short,short,map<short,int>> min_max_freq = min_max_heights ( altitude_map );
     const altitude_map_levels levels = determine_levels ( min_max_freq );

     for ( int wy=0; wy<world::world_height; ++wy ) {
          for ( int wx=0; wx<world::world_width; ++wx ) {
               // Create the world tile object
               land_block region;
               region.index = world::world_idx ( wx,wy );
               const int region_x = wx*landblock_width;
               const int region_y = wy*landblock_height;

	       create_base_tile_types(region_x, region_y, region, altitude_map, levels);
	       
               // Serialize it to disk
               region.save();
          }
     }
}

/*
 * Basic heightmap generation algorithm. Calls the previously
 * defined functions.
 */
void create_heightmap_world()
{
     height_map_t altitude_map = make_altitude_map();
     const int num_fault_lines = worldgen_size/10000;

     for ( int i=0; i<num_fault_lines; ++i ) {
	  int sx = engine::roll_dice(1, worldgen_width-1 );
	  int sy = engine::roll_dice(1, worldgen_height-1 );
	  int ex = engine::roll_dice(1, worldgen_width-1 );
	  int ey = engine::roll_dice(1, worldgen_height-1 );
	  
          apply_fault_line_to_altitude_map ( altitude_map,
                                             sx,
                                             sy,
                                             ex,
                                             ey,
                                             1000
                                           );
     }
     for ( int i=0; i<32; ++i ) {
          smooth_altitude_map ( altitude_map );
     }
     convert_altitudes_to_tiles ( altitude_map );
}

/*
 * External interface to world creation. Eventually, this needs to
 * be extended to provide some feedback fo the user.
 */
void build_world()
{
     create_heightmap_world();
     // TODO: Walk the map creating boundary regions (e.g. beaches, cliffs)
     // TODO: Walk each world block with marching squares to create fake hills
     // TODO: For the far future, biomes, history and goodies.
     
     // TODO: Find a place to crash the ship to let us start having something playable
}

}
