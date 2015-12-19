#include "path_finding.h"
#include "../world/world.h"
#include <unordered_set>
#include <tuple>
#include <map>
#include "../components/position_component3d.hpp"
#include "../../engine/astar.hpp"
#include "../world/planet.hpp"

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

class map_search_node {
public:
  location_t pos;
  
  map_search_node() { }
  map_search_node( location_t loc ) : pos(loc) {}
  
  float GoalDistanceEstimate ( map_search_node & goal );
  bool IsGoal ( map_search_node &node_goal );
  bool GetSuccessors ( AStarSearch<map_search_node> * a_star_search, map_search_node * parent_node );
  float GetCost ( map_search_node &successor );
  bool IsSameState( map_search_node &rhs );
  
  void PrintNodeInfo();
};

bool map_search_node::IsSameState ( map_search_node& rhs )
{
    if ( pos.x==rhs.pos.x and pos.y==rhs.pos.y and pos.z==rhs.pos.z) {
	return true;
    } else {
	return false;
    }
}

void map_search_node::PrintNodeInfo()
{
    std::cout << "Node Position: " << pos.x << "," << pos.y << "," << pos.z << "\n";
}

bool map_search_node::IsGoal ( map_search_node& node_goal )
{
    return IsSameState( node_goal );
}

bool map_search_node::GetSuccessors ( AStarSearch< map_search_node >* a_star_search, map_search_node* parent_node )
{
    int parent_x = -1;
    int parent_y = -1;
    int parent_z = -1;
    if ( parent_node ) {
	parent_x = parent_node->pos.x;
	parent_y = parent_node->pos.y;
	parent_z = parent_node->pos.z;
    }
    
    map_search_node new_node;
    tile_t * tile = world::planet->get_tile( pos );
    if ( tile->flags.test( TILE_OPTIONS::CAN_GO_NORTH ) and parent_y != pos.y-1 ) { 
      new_node = map_search_node( location_t{ pos.region, pos.x, static_cast<uint8_t>(pos.y-1), pos.z } );
      a_star_search->AddSuccessor( new_node );        
    };
    if ( tile->flags.test( TILE_OPTIONS::CAN_GO_SOUTH ) and parent_y != pos.y+1 ) { 
      new_node = map_search_node( location_t{ pos.region, pos.x, static_cast<uint8_t>(pos.y+1), pos.z } );
      a_star_search->AddSuccessor( new_node );        
    };
    if ( tile->flags.test( TILE_OPTIONS::CAN_GO_EAST ) and parent_x != pos.x+1 ) { 
      new_node = map_search_node( location_t{ pos.region, static_cast<uint8_t>(pos.x+1), pos.y, pos.z } );
      a_star_search->AddSuccessor( new_node );        
    };
    if ( tile->flags.test( TILE_OPTIONS::CAN_GO_WEST ) and parent_x != pos.x-1 ) { 
      new_node = map_search_node( location_t{ pos.region, static_cast<uint8_t>(pos.x-1), pos.y, pos.z } );
      a_star_search->AddSuccessor( new_node );        
    };
    if ( tile->flags.test( TILE_OPTIONS::CAN_GO_UP ) and parent_z != pos.z+1 ) { 
      new_node = map_search_node( location_t{ pos.region, pos.x, pos.y, static_cast<uint8_t>(pos.z+1) } );
      a_star_search->AddSuccessor( new_node );        
    };
    if ( tile->flags.test( TILE_OPTIONS::CAN_GO_DOWN ) and parent_z != pos.z-1 ) { 
      new_node = map_search_node( location_t{ pos.region, pos.x, pos.y, static_cast<uint8_t>(pos.z-1) } );
      a_star_search->AddSuccessor( new_node );
    };

    return true;
}

float map_search_node::GetCost ( map_search_node &successor )
{
    return 1.0f;
}

float map_search_node::GoalDistanceEstimate ( map_search_node& goal )
{
    return std::abs( pos.x - goal.pos.x ) + std::abs( pos.y - goal.pos.y ) + std::abs( pos.z - goal.pos.z );
}


shared_ptr< navigation_path > find_path ( location_t start, location_t end )
{
    AStarSearch<map_search_node> a_star_search;
    map_search_node a_start(start);
    map_search_node a_end(end);
    
    a_star_search.SetStartAndGoalStates( a_start, a_end );
    unsigned int search_state;
    unsigned int search_steps;
    
    do {
      search_state = a_star_search.SearchStep();
      ++search_steps;
      std::cout << "Step: " << search_steps << "\n";
    } while ( search_state == AStarSearch< map_search_node >::SEARCH_STATE_SEARCHING );
      
    if ( search_state == AStarSearch<map_search_node>::SEARCH_STATE_SUCCEEDED ) {
      shared_ptr<navigation_path> result = make_shared<navigation_path>();
      result->destination = end;
      map_search_node *node = a_star_search.GetSolutionStart();
      result->steps.push( node->pos );
      for ( ; ; ) {
	  node = a_star_search.GetSolutionNext();
	  if (!node) break;
	  result->steps.push( node->pos );
      }
      a_star_search.FreeSolutionNodes();

      a_star_search.EnsureMemoryFreed();
      return result;

    }
    std::cout << "No search solution!\n";
    shared_ptr<navigation_path> result = make_shared<navigation_path>();
    a_star_search.EnsureMemoryFreed();
    return result;
}
  
}
