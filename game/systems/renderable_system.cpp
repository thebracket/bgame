#include "renderable_system.h"
#include <iostream>
#include <tuple>
#include "../game.h"

void renderable_system::tick ( const double &duration_ms )
{
     bool changes = false;
     vector<entity_moved_message> * change_list = game_engine->messaging->get_messages_by_type<entity_moved_message>();
     for ( entity_moved_message &msg : *change_list ) {
          if ( !msg.deleted ) changes = true;
          msg.deleted = true;
     }

     if ( changes ) {

          // Clear the visible entity system
          std::fill ( world::entity_render_list.begin(), world::entity_render_list.end(), world::render_layers_t() );

          const position_component * camera_pos = game_engine->ecs->find_entity_component<position_component> ( world::camera_handle );
          const int top_y = camera_pos->y - 23;
          const int left_x = camera_pos->x - 32;
          const int bottom_y = top_y + 45;
          const int right_x = left_x + 64;

          const vector<position_component> * positions = game_engine->ecs->find_components_by_type<position_component> ();
          for ( const position_component &pos : *positions ) {

               if ( pos.x >= left_x and pos.x <= right_x and pos.y >= top_y and pos.y <= bottom_y ) {

                    renderable_component * render_info = game_engine->ecs->find_entity_component<renderable_component> ( pos.entity_id );
                    if ( render_info != nullptr ) {
                         const int idx = world::current_region->idx ( pos.x, pos.y );
                         world::render_info_t target { render_info->tile_idx, render_info->glyph, render_info->foreground, render_info->background, render_info->translucent, render_info->composite, render_info->entity_id, render_info->extra_tall };
                         switch ( render_info->layer ) {
                         case 0 :
                              world::entity_render_list[idx].floor = target;
                              break;
                         case 1 :
                              world::entity_render_list[idx].building = target;
                              break;
                         case 2 :
                              world::entity_render_list[idx].top = target;
                              break;
                         }
                    }
               }
          }
     }
}
