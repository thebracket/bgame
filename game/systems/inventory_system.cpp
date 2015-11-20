#include "inventory_system.h"
#include "../game.h"
#include "../world/world.h"

using world::inventory;
using world::available_item;

void inventory_system::tick ( const double& duration_ms )
{
     inventory.clear();
     vector<item_storage_component> * stored = game_engine->ecs->find_components_by_type<item_storage_component>();
     for ( const item_storage_component &store : *stored ) {

          if ( !store.claimed ) {

               debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component> ( store.entity_id );
               position_component * pos = game_engine->ecs->find_entity_component<position_component> ( store.container_id );

               auto finder = inventory.find ( name->debug_name );
               if ( finder == inventory.end() ) {
                    available_item item { name->debug_name, { pos->x, pos->y }, store.entity_id };
                    inventory[name->debug_name] = vector<available_item> {item};
               } else {
                    available_item item { name->debug_name, { pos->x, pos->y }, store.entity_id };
                    finder->second.push_back ( item );
               }
          }
     }

}

