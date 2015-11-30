#include "obstruction_system.h"
#include "../components/obstruction_component.h"
#include "../components/position_component.h"
#include "../world/world.h"
#include "../game.h"

void obstruction_system::tick ( const double& duration_ms )
{
     bool changes = false;
     vector<walkability_changed_message> * change_list = game_engine->messaging->get_messages_by_type<walkability_changed_message>();
     for ( walkability_changed_message &msg : *change_list ) {
          changes = true;
          msg.deleted = true;
     }
     if ( world::walk_blocked.size() == 0 or changes ) {
	  std::fill( world::walk_blocked.begin(), world::walk_blocked.end(), false );
	  std::fill( world::view_blocked.begin(), world::view_blocked.end(), false );

          const vector<obstruction_component> * blockages = game_engine->ecs->find_components_by_type<obstruction_component>();
          for ( const obstruction_component &block : *blockages ) {

               position_component * pos = game_engine->ecs->find_entity_component<position_component> ( block.entity_id );
               if ( pos != nullptr ) {
                    const int idx = world::current_region->idx ( pos->x, pos->y );
                    if ( block.blocks_entry ) world::walk_blocked[idx] = true;
                    if ( block.blocks_visibility ) world::view_blocked[idx] = true;
               }
          }
     }
}
