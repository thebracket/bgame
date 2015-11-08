#include "flowmap_system.h"
#include "../components/provisions_component.h"
#include "../components/position_component.h"
#include "../game.h"
#include <set>
#include <tuple>

using std::make_pair;
using std::pair;
using std::set;
using std::tuple;
using std::make_tuple;

namespace flowmaps {

vector<short> food_flow_map;
vector<short> water_flow_map;
vector<int> food_flow_map_entity_id;
vector<int> water_flow_map_entity_id;

inline void erase_flow_map ( vector<short> &flow_map )
{
     std::fill ( flow_map.begin(), flow_map.end(), 32000 );
}

void spread( vector< pair< tuple<int,int,int>, short > > &new_cells, set<int> &visited, const int &x, const int &y, const short distance, const int &entity_id) {
    const int idx = world::current_region->idx(x,y);
    if (world::current_region->revealed[idx] == false) return;
    if (visited.find(idx) != visited.end() ) return;
    if (world::walk_blocked.find(idx) != world::walk_blocked.end()) return;
    if (x < 0) return;
    if (x > landblock_width-1) return;
    if (y < 0) return;
    if (y > landblock_height-1) return;
    
    visited.insert( idx );
    new_cells.push_back( { make_tuple( x, y, entity_id ), distance } );
}

void build_flow_map ( vector<short> &map, vector<tuple<int,int,int>> &sources, vector<int> &flow_map_entity_id )
{
     erase_flow_map(map);
     set<int> visited;
     
     vector< pair< tuple<int,int,int>, short > > active_cells;
     vector< pair< tuple<int,int,int>, short > > new_cells;
     for (const tuple<int,int,int> &source : sources) {
       active_cells.push_back( make_pair(make_tuple(std::get<0>(source), std::get<1>(source), std::get<2>(source)), 0) );
     }
     
     while ( active_cells.size() > 0 ) {
	for ( pair< tuple<int,int,int>, short > cell : active_cells ) {
	    const int x = std::get<0>(cell.first);
	    const int y = std::get<1>(cell.first);
	    const int entity_id = std::get<2>(cell.first);
	    const short distance = cell.second;
	    const int idx = world::current_region->idx( x, y  );
	    map[idx] = distance;
	    flow_map_entity_id[idx] = entity_id;
	    
	    spread ( new_cells, visited, x+1, y, distance+1, entity_id );
	    spread ( new_cells, visited, x-1, y, distance+1, entity_id );
	    spread ( new_cells, visited, x, y+1, distance+1, entity_id );
	    spread ( new_cells, visited, x, y-1, distance+1, entity_id );
	}
	std::swap(active_cells, new_cells);
	new_cells.clear();
     }
}

}

void flowmap_system::tick ( const double& duration_ms )
{
    if (world::paused) return;
  
     vector<provisions_component> * provisions = game_engine->ecs->find_components_by_type<provisions_component>();
     vector<tuple<int,int,int>> food_sources;
     vector<tuple<int,int,int>> water_sources;
     for ( const provisions_component provision : *provisions ) {
          position_component * pos = game_engine->ecs->find_entity_component<position_component> ( provision.entity_id );
          if ( provision.provided_resource == 1 ) food_sources.push_back ( make_tuple( pos->x, pos->y, provision.entity_id ) );
          if ( provision.provided_resource == 2 ) water_sources.push_back ( make_tuple( pos->x, pos->y, provision.entity_id ) );
     }

     flowmaps::build_flow_map ( flowmaps::food_flow_map, food_sources, flowmaps::food_flow_map_entity_id );
     flowmaps::build_flow_map ( flowmaps::water_flow_map, water_sources, flowmaps::water_flow_map_entity_id );
}
