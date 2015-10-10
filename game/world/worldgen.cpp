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

std::tuple<short,short,map<short,int>> min_max_heights ( const height_map_t & altitude_map )
{
     short min_height = 32767;
     short max_height = -32768;
     map<short,int> frequency_map;
     for ( const short &h : altitude_map ) {
          if ( h > min_height ) min_height = h;
          if ( h < max_height ) max_height = h;
          auto finder = frequency_map.find ( h );
          if ( finder == frequency_map.end() ) {
               frequency_map[h] = 1;
          } else {
               finder->second = finder->second+1;
          }
     }

     return std::make_tuple ( min_height, max_height, frequency_map );
}

struct altitude_map_levels {
     short water_level;
     short plains_level;
     short hills_level;
     short mountains_level;
};

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

               // Populate it from the height map
               for ( int y=0; y<landblock_height; ++y ) {
                    for ( int x=0; x<landblock_width; ++x ) {
                         const int amp_x = region_x + x;
                         const int amp_y = region_y + y;
                         const short altitude = altitude_map[idx ( amp_x, amp_y )];
                         if ( altitude < levels.water_level ) {
                              region.tiles[region.idx ( x,y )].base_tile_type = water;
                         } else if ( altitude < levels.plains_level ) {
                              region.tiles[region.idx ( x,y )].base_tile_type = flat;
                         } else if ( altitude < levels.hills_level ) {
                              region.tiles[region.idx ( x,y )].base_tile_type = hill;
                         } else {
                              region.tiles[region.idx ( x,y )].base_tile_type = mountain;
                         }
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
