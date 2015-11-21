#include "find_nearest_provision.h"
#include "../game.h"
#include <map>
#include <vector>

using std::map;
using std::make_pair;
using std::vector;

namespace ai {

int find_nearest_provision ( const int& provision_type, const pair<int,int> &start )
{
     map<int,int> options;
     vector<provisions_component> * provisions = game_engine->ecs->find_components_by_type< provisions_component >();
     for ( const provisions_component &provision : *provisions ) {
          if ( provision.provided_resource == provision_type ) {
               position_component * pos = game_engine->ecs->find_entity_component<position_component> ( provision.entity_id );
               float distance = std::sqrt ( ( std::abs ( start.first - pos->x ) * std::abs ( start.first - pos->x ) ) +
                                            std::abs ( start.second - pos->y ) * std::abs ( start.second - pos->y ) );
               options[distance] = pos->entity_id;
          }
     }

     if ( options.empty() ) {
          return -1;
     } else {
          return options.begin()->second;
     }
}

}