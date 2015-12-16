#include "path_finding.h"
#include "../world/world.h"
#include <unordered_set>
#include <tuple>
#include <map>
#include "../components/position_component3d.hpp"

using std::unordered_set;
using std::tuple;
using std::make_tuple;
using std::fill;
using std::map;
using std::get;
using std::swap;
using std::make_shared;
using std::make_pair;

namespace ai {

  //FIXME: This is awful
void spread_flow ( vector<tuple<short,int,int,int>> &new_cells, unordered_set<int> &visited, const int &x, const int &y, const int &z, const short &distance, vector<bool> &walkable )
{
     const int index = get_tile_index ( x,y,z );
     if ( world::current_region->revealed[index]==false ) {
          return;
     }
     if ( visited.find ( index ) != visited.end() ) {
          return;
     }
     if ( walkable[ index ] != false ) {
          return;
     }
     if ( world::current_region->tiles[ index].base_tile_type == tile_type::WATER ) {
          return;
     }
     if ( x < 0 ) {
          return;
     }
     if ( x > landblock_width-1 ) {
          return;
     }
     if ( y < 0 ) {
          return;
     }
     if ( y > landblock_height-1 ) {
          return;
     }

     visited.insert ( index );
     new_cells.push_back ( make_tuple ( distance, x, y, z ) );
}

shared_ptr< navigation_path > find_path ( tuple< int, int, int > start, tuple< int, int, int > end, vector<bool> &walkable )
{
     // For now, we're just using a really inefficient brute-force approach just to get going; this will be optimized later!

     // If we start at the end, then return null - nothign to do!
     if ( std::get<0>(start) == std::get<0>(end) and std::get<1>(start) == std::get<1>(end) and std::get<2>(start) == std::get<2>(end) ) {
          return nullptr;
     }

     vector<short> flow_map;
     flow_map.resize ( REGION_TILES );
     fill ( flow_map.begin(), flow_map.end(), 32000 );
     unordered_set<int> visited;
     vector<tuple<short,int,int,int>> active_cells; // (distance, x, y)
     vector<tuple<short,int,int,int>> new_cells;

     bool found_it = false;
     active_cells.push_back ( make_tuple ( 0, std::get<0>(end), std::get<1>(end), std::get<2>(end) ) );
     while ( active_cells.size() > 0 ) {
          for ( tuple<short,int,int,int> &cell : active_cells ) {
               const short distance = get<0> ( cell );
               const int x = get<1> ( cell );
               const int y = get<2> ( cell );
	       const int z = get<3> ( cell );
               if ( x == std::get<0>(start) and y == std::get<1>(start) and z == std::get<2>(start) ) {
                    found_it = true;
               }
               flow_map[ get_tile_index ( x, y, z ) ] = distance;

               spread_flow ( new_cells, visited, x-1, y, z, distance+1, walkable );
               spread_flow ( new_cells, visited, x+1, y, z, distance+1, walkable );
               spread_flow ( new_cells, visited, x, y-1, z, distance+1, walkable );
               spread_flow ( new_cells, visited, x, y+1, z, distance+1, walkable );
          }
          swap ( active_cells, new_cells );
          new_cells.clear();
     }

     if ( !found_it ) {
          return nullptr;
     }

     shared_ptr<navigation_path> result = make_shared<navigation_path>();
     result->destination = end;

     tuple<int,int,int> current = start;
     int n = 0;
     char last_direction = -1;
     while ( (std::get<0>(current) != std::get<0>(end) or std::get<1>(current) != std::get<1>(end) or std::get<2>(current) != std::get<2>(end)) and result->steps.size()<100 ) {
          map<short,char> options;
          if ( last_direction!= 2 ) {
               options[ flow_map[ get_tile_index( std::get<0>(current)-1, std::get<1>(current), std::get<2>(current) ) ] ] = 1;
          }
          if ( last_direction!= 1 ) {
               options[ flow_map[ get_tile_index( std::get<0>(current)+1, std::get<1>(current), std::get<2>(current) ) ] ] = 2;
          }
          if ( last_direction!= 4 ) {
               options[ flow_map[ get_tile_index( std::get<0>(current), std::get<1>(current)-1, std::get<2>(current) ) ] ] = 3;
          }
          if ( last_direction!= 3 ) {
               options[ flow_map[ get_tile_index( std::get<0>(current), std::get<1>(current)+1, std::get<2>(current) ) ] ] = 4;
          }

          auto it = options.begin();
          int next_x = std::get<0>(current);
          int next_y = std::get<1>(current);
	  int next_z = std::get<2>(current);
          switch ( it->second ) {
          case 1:
               --next_x ;
               break;
          case 2:
               ++next_x ;
               break;
          case 3:
               --next_y ;
               break;
          case 4:
               ++next_y ;
               break;
          }
          last_direction = it->second;
          result->steps.push ( make_tuple ( next_x, next_y, next_z ) );
          std::get<0>(current) = next_x;
          std::get<1>(current) = next_y;
          std::get<2>(current) = next_z;
          ++n;
     }

     return result;
}

}
