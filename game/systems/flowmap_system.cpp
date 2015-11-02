#include "flowmap_system.h"
#include "../components/provisions_component.h"
#include "../components/position_component.h"
#include "../../engine/globals.h"
#include <set>

using std::make_pair;
using std::pair;
using std::set;

namespace flowmaps {

vector<short> food_flow_map;
vector<short> water_flow_map;

inline void erase_flow_map ( vector<short> &flow_map )
{
     std::fill ( flow_map.begin(), flow_map.end(), 32000 );
}

void spread( vector< pair< pair<int,int>, short > > &new_cells, set<int> &visited, const int &x, const int &y, const short distance) {
    const int idx = world::current_region->idx(x,y);
    if (world::current_region->revealed[idx] == false) return;
    if (visited.find(idx) != visited.end() ) return;
    if (world::walk_blocked.find(idx) != world::walk_blocked.end()) return;
    if (x < 0) return;
    if (x > landblock_width-1) return;
    if (y < 0) return;
    if (y > landblock_height-1) return;
    
    visited.insert( idx );
    new_cells.push_back( { { x, y }, distance } );
}

void build_flow_map ( vector<short> &map, vector<pair<int,int>> &sources )
{
     erase_flow_map(map);
     set<int> visited;
     
     vector< pair< pair<int,int>, short > > active_cells;
     vector< pair< pair<int,int>, short > > new_cells;
     for (const pair<int,int> &source : sources) {
       active_cells.push_back( make_pair(make_pair(source.first, source.second), 0) );
     }
     
     while ( active_cells.size() > 0 ) {
	for ( pair< pair<int,int>, short > cell : active_cells ) {
	    const int x = cell.first.first;
	    const int y = cell.first.second;
	    const short distance = cell.second;
	    const int idx = world::current_region->idx( x, y  );
	    map[idx] = distance;
	    
	    spread ( new_cells, visited, x+1, y, distance+1 );
	    spread ( new_cells, visited, x-1, y, distance+1 );
	    spread ( new_cells, visited, x, y+1, distance+1 );
	    spread ( new_cells, visited, x, y-1, distance+1 );
	}
	std::swap(active_cells, new_cells);
	new_cells.clear();
     }
}

}

void flowmap_system::tick ( const double& duration_ms )
{
    if (world::paused) return;
  
     vector<provisions_component> * provisions = engine::globals::ecs->find_components_by_type<provisions_component>();
     vector<pair<int,int>> food_sources;
     vector<pair<int,int>> water_sources;
     for ( const provisions_component provision : *provisions ) {
          position_component * pos = engine::globals::ecs->find_entity_component<position_component> ( provision.entity_id );
          if ( provision.provided_resource == 1 ) food_sources.push_back ( { pos->x, pos->y } );
          if ( provision.provided_resource == 2 ) water_sources.push_back ( { pos->x, pos->y } );
     }

     flowmaps::build_flow_map ( flowmaps::food_flow_map, food_sources );
     flowmaps::build_flow_map ( flowmaps::water_flow_map, water_sources );
}
