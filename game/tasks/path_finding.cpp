#include "path_finding.h"
#include "../world/world.h"
#include <unordered_set>
#include <tuple>
#include <map>

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

void spread_flow ( vector<tuple<short,int,int>> &new_cells, unordered_set<int> &visited, const int &x, const int &y, const short &distance, unordered_map<int, bool> &walkable )
{
     const int index = world::current_region->idx ( x,y );
     if ( world::current_region->revealed[index]==false ) {
          return;
     }
     if ( visited.find ( index ) != visited.end() ) {
          return;
     }
     if ( walkable.find ( index ) != walkable.end() ) {
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
     new_cells.push_back ( make_tuple ( distance, x, y ) );
}

shared_ptr< navigation_path > find_path ( pair< int, int > start, pair< int, int > end, unordered_map<int, bool> &walkable )
{
     // For now, we're just using a really inefficient brute-force approach just to get going; this will be optimized later!

     // If we start at the end, then return null - nothign to do!
     if ( start.first == end.first and start.second == end.second ) {
          return nullptr;
     }

     vector<short> flow_map;
     flow_map.resize ( landblock_height * landblock_width );
     fill ( flow_map.begin(), flow_map.end(), 32000 );
     unordered_set<int> visited;
     vector<tuple<short,int,int>> active_cells; // (distance, x, y)
     vector<tuple<short,int,int>> new_cells;

     bool found_it = false;
     active_cells.push_back ( make_tuple ( 0, end.first, end.second ) );
     while ( active_cells.size() > 0 ) {
          for ( tuple<short,int,int> &cell : active_cells ) {
               const short distance = get<0> ( cell );
               const int x = get<1> ( cell );
               const int y = get<2> ( cell );
               if ( x == start.first and y == start.second ) {
                    found_it = true;
               }
               flow_map[ world::current_region->idx ( x, y ) ] = distance;

               spread_flow ( new_cells, visited, x-1, y, distance+1, walkable );
               spread_flow ( new_cells, visited, x+1, y, distance+1, walkable );
               spread_flow ( new_cells, visited, x, y-1, distance+1, walkable );
               spread_flow ( new_cells, visited, x, y+1, distance+1, walkable );
          }
          swap ( active_cells, new_cells );
          new_cells.clear();
     }

     if ( !found_it ) {
          return nullptr;
     }

     shared_ptr<navigation_path> result = make_shared<navigation_path>();
     result->destination = end;

     pair<int,int> current = start;
     int n = 0;
     char last_direction = -1;
     while ( current.first != end.first or current.second != end.second ) {
          map<short,char> options;
          if ( last_direction!= 2 ) {
               options[ flow_map[ world::current_region->idx ( current.first-1, current.second ) ] ] = 1;
          }
          if ( last_direction!= 1 ) {
               options[ flow_map[ world::current_region->idx ( current.first+1, current.second ) ] ] = 2;
          }
          if ( last_direction!= 4 ) {
               options[ flow_map[ world::current_region->idx ( current.first, current.second-1 ) ] ] = 3;
          }
          if ( last_direction!= 3 ) {
               options[ flow_map[ world::current_region->idx ( current.first, current.second+1 ) ] ] = 4;
          }

          auto it = options.begin();
          int next_x = current.first;
          int next_y = current.second;
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
          result->steps.push ( make_pair ( next_x, next_y ) );
          current.first = next_x;
          current.second = next_y;
          ++n;
     }

     return result;
}

}